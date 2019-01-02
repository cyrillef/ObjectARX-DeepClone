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
// AcRxEventReactor reactors.

#include "StdAfx.h"
#include "StdArx.h"


AsdkRxEvent::AsdkRxEvent(const bool autoInitAndRelease)
{
	m_autoInitAndRelease = autoInitAndRelease;
	if (m_autoInitAndRelease)
		if (NULL != acrxEvent)
			acrxEvent->addReactor(this);
		else
			m_autoInitAndRelease = false;
}

AsdkRxEvent::~AsdkRxEvent()
{
	if (m_autoInitAndRelease)
		if (NULL != acrxEvent)
			acrxEvent->removeReactor(this);
}

void AsdkRxEvent::beginDeepCloneXlation(AcDbIdMapping& x0,
                                        Acad::ErrorStatus* x1)
{
	// TODO: implement this function.
	if ( x0.deepCloneContext () != AcDb::kDcWblkObjects ) {
		//----- If a sub-pline is copied, make sure to copy all
		//----- Do it first for optimization...
		AcDbIdMappingIter iter2 (x0) ;
		AcDbIdPair idPair2 ;
		for ( iter2.start () ; !iter2.done () ; iter2.next () ) {
			if ( !iter2.getMap (idPair2) )
				continue ;
			if ( !idPair2.isCloned () )
				continue ;

			AcDbPolyline *pLine, *pLineClone ;
			if ( acdbOpenObject (pLine, idPair2.key (), AcDb::kForRead) != Acad::eOk )
				continue ;
			acdbOpenObject (pLineClone, idPair2.value (), AcDb::kForRead) ;

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

					AcDbIdPair pair (mainEntId, AcDbObjectId::kNull, true) ;
					if ( !x0.compute (pair) ) {
						AcDbPolyline *pMainEnt, *pMainClone ;
						acdbOpenObject (pMainEnt, mainEntId, AcDb::kForRead) ;

						AcDbObject *pOwner ;
						if ( acdbOpenAcDbObject (pOwner, pLineClone->ownerId (), AcDb::kForWrite) == Acad::eOk ) {
							if ( x0.deepCloneContext () == AcDb::kDcWblock )
								pMainEnt->wblockClone (pOwner, (AcDbObject *&)pMainClone, x0, true) ;
							else
								pMainEnt->deepClone (pOwner, (AcDbObject *&)pMainClone, x0, true) ;
							pOwner->close () ;
							pMainClone->close () ;
						}
						pMainEnt->close () ;
					}
					pReac->close () ;
					break ;
				}
			}
			//----- Do not do the following...
			//delete [] ptr ;

			pLineClone->close () ;
			pLine->close () ;
		}

		//----- We need to copy all sub-pline(s)
		AcDbIdMappingIter iter (x0) ;
		AcDbIdPair idPair ;
		for ( iter.start () ; !iter.done () ; iter.next () ) {
			if ( !iter.getMap (idPair) )
				continue ;
			if ( !idPair.isCloned () )
				continue ;
			
			AsdkOPLineReactor *pReac ;
			if ( acdbOpenObject (pReac, idPair.key (), AcDb::kForRead) != Acad::eOk )
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

			AcDbIdPair idp (mainEntId, AcDbObjectId::kNull, true) ;
			x0.compute (idp) ;
			AcDbObject *pMainClone ;
			acdbOpenAcDbObject (pMainClone, idp.value (), AcDb::kForRead) ;

			AcDbObjectId id =pMainClone->ownerId () ;
			pMainClone->close () ;

			AcDbObject *pOwnerClone ;
			acdbOpenAcDbObject (pOwnerClone, id, AcDb::kForWrite) ;

			//----- In all cases force a clone of linked plines
			//----- We can use IDs from the clone since there are not yet XLATED
			for ( int i =0 ; i < pReac->entsToUpdate ().length () ; i++ ) {
				if ( !x0.compute (AcDbIdPair (pReac->entsToUpdate ().at (i), AcDbObjectId::kNull, true)) ) {
					//----- Open original object
					AcDbPolyline *pLine ;
					acdbOpenAcDbObject ((AcDbObject *&)pLine, pReac->entsToUpdate ().at (i), AcDb::kForRead) ;

					AcDbPolyline *pLineClone ;
					if ( x0.deepCloneContext () == AcDb::kDcWblock )
						pLine->wblockClone (pOwnerClone, (AcDbObject *&)pLineClone, x0, true) ;
					else
						pLine->deepClone (pOwnerClone, (AcDbObject *&)pLineClone, x0, true) ;
					pLine->close () ;
					pLineClone->close () ;
				}
			}
			pOwnerClone->close () ;
		
			pReac->close () ;
		}
	}
}
