//
// (C) Copyright 1994-2000 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//
// Cyrille Fauvel
// Autodesk Developer Consulting Group
// January 10th, 2000

/////////////////////////////////////////////
// AcApLongTransactionReactor reactors.

#include "StdAfx.h"
#include "StdArx.h"

AsdkLgTransReactor::AsdkLgTransReactor(const bool autoInitAndRelease)
{
	m_autoInitAndRelease = autoInitAndRelease;
	if (m_autoInitAndRelease)
		if (NULL != acapLongTransactionManager)
			acapLongTransactionManager->addReactor(this);
		else
			m_autoInitAndRelease = false;
}

AsdkLgTransReactor::~AsdkLgTransReactor()
{
	if (m_autoInitAndRelease)
		if (NULL != acapLongTransactionManager)
			acapLongTransactionManager->removeReactor(this);
}

void AsdkLgTransReactor::beginCheckOut(AcDbLongTransaction& x0,
                                       AcDbObjectIdArray& originList)
{
	// TODO: implement this function.
	for ( int j =0 ; j < originList.length () ; j++ ) {
		AcDbPolyline *pLine ;
		if ( acdbOpenObject (pLine, originList.at (j), AcDb::kForRead) != Acad::eOk )
			continue ;

		AcDbVoidPtrArray *ptr =pLine->reactors () ;
		for ( int i =0 ; i < ptr->length () ; i++ ) {
			if ( acdbIsPersistentReactor (ptr->at (i)) ) {
				//----- May be one of our reactor is in?
				AcDbObjectId idr =acdbPersistentReactorObjectId (ptr->at (i)) ;
				AsdkOPLineReactor *pReac ;
				if ( acdbOpenObject (pReac, idr, AcDb::kForRead) != Acad::eOk )
					continue ;

				//----- Main entity
				AcDbDictionary *pExtDct ;
				if ( acdbOpenObject (pExtDct, pReac->ownerId (), AcDb::kForRead) != Acad::eOk ) {
					pReac->close () ;
					continue ;
				}
				AcDbObjectId mainEntId ;
				mainEntId =pExtDct->ownerId () ;
				pExtDct->close () ;

				if ( !originList.contains (mainEntId) ) {
					originList.append (mainEntId) ;
					x0.addToWorkSet (mainEntId) ;
				}

				for ( int k =0 ; k < pReac->entsToUpdate ().length () ; k++ ) {
					if ( !originList.contains (pReac->entsToUpdate ().at (k)) ) {
						originList.append (pReac->entsToUpdate ().at (k)) ;
						x0.addToWorkSet (pReac->entsToUpdate ().at (k)) ;
					}
				}

				pReac->close () ;
				break ;
			}
		}
		//----- Do not do the following...
		//delete [] ptr ;

		pLine->close () ;
	}
}

void AsdkLgTransReactor::endCheckIn(AcDbLongTransaction& x0) 
{
	// TODO: implement this function.
	AcDbIdMapping &idMap =*x0.activeIdMap () ;
	AcDbDatabase *pOrigDb, *pDestDb ;
	idMap.origDb (pOrigDb) ;
	idMap.destDb (pDestDb) ;

	AcDbIdMappingIter iter (idMap) ;
	AcDbIdPair idPair ;
	for ( iter.start () ; !iter.done () ; iter.next () ) {
		if ( !iter.getMap (idPair) )
			continue ;
		if ( !idPair.isCloned () )
			continue ;
		AcDbObjectId idOrig =x0.originObject (idPair.key ()) ;
		if ( idOrig.isNull () )
			continue ; //----- Newly added objects need not be fixed
		AsdkOPLineReactor *pO ;
		if ( acdbOpenObject (pO, idPair.value (), AcDb::kForRead) == Acad::eOk ) {
			//----- Main entity
			AcDbDictionary *pDict ;
			AcDbEntity *pEnt ;
			if ( acdbOpenObject (pDict, pO->ownerId (), AcDb::kForRead) == Acad::eOk ) {
				if ( acdbOpenAcDbEntity (pEnt, pDict->ownerId (), AcDb::kForWrite) == Acad::eOk ) {
					//----- Make sure that the reactors are fired on the right object
					AcDbIdMapping idFixMap ;
					idFixMap.setDestDb (pDestDb) ;
					AcDbIdPair idFixPair ;
					idFixPair.setKey (idOrig) ;
					idFixPair.setValue (idPair.value ()) ;
					idFixMap.assign (idFixPair) ;
					pEnt->swapReferences (idFixMap) ;
					pEnt->close () ;
				}
				pDict->close () ;
			}
			//----- Sub entity(ies)
			for ( int i =0 ; i < pO->entsToUpdate ().length () ; i++ ) {
				if ( acdbOpenAcDbEntity (pEnt, pO->entsToUpdate ().at (i), AcDb::kForWrite) == Acad::eOk ) {
					//----- Make sure that the reactors are fired on the right object
					AcDbIdMapping idFixMap ;
					idFixMap.setDestDb (pDestDb) ;
					AcDbIdPair idFixPair ;
					idFixPair.setKey (idOrig) ;
					idFixPair.setValue (idPair.value ()) ;
					idFixMap.assign (idFixPair) ;
					pEnt->swapReferences (idFixMap) ;
					pEnt->close () ;
				}
			}

			pO->close () ;
		}
	}
}
