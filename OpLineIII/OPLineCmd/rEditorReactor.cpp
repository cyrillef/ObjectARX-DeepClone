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
// AcEditorReactor reactors.

#include "StdAfx.h"
#include "StdArx.h"

//-----------------------------------------------------------------------------
void ToStdAcadEntity (AcDbBlockTableRecord *pBTR, AcDbEntity *&pEnt) {
	AsdkOPLine *pLine =AsdkOPLine::cast (pEnt) ;
	if ( pLine == NULL )
		return ;

	if ( pLine->upgradeOpen () != Acad::eOk )
		return ;

	AsdkOPLineFiler filer ;
	pLine->dwgOut (&filer) ;

	AcDbVoidPtrArray ents ;
	pLine->explode (ents) ;
	if ( ents.length () == 0 )
		return ;

	//----- The first entity in ents is the main entity...
	//----- We keep the reactor as is...
	AcDbEntity *pMain =(AcDbEntity *)ents.at (0) ;
	if ( pLine->handOverTo (pMain, true, true) == Acad::eObjectToBeDeleted ) {
		delete pLine ;
		pEnt =pMain ;
	} else {
		delete pMain ;
		return ;
	}

	//----- Append others sub-entity(ies)
	pBTR->upgradeOpen () ;
	filer.writeString (_T(/*NOXLATE*/"SUB_ENTITY_MARKER")) ;
	for ( int i =1 ; i < ents.length () ; i++ ) {
		AcDbObjectId id ;
		pBTR->appendAcDbEntity (id, (AcDbEntity *)ents.at (i)) ;
		filer.writeAcDbHandle (id.handle ()) ;
	}

	//----- Attach XData to all...
	for ( i =0 ; i < ents.length () ; i++ ) {
		((AcDbEntity *)ents.at (i))->setXData (filer.getResbuf ()) ;
		if ( i != 0 )
			((AcDbEntity *)ents.at (i))->close () ;
	}
}

void BackToOPLine (AcDbBlockTableRecord *pBTR, AcDbEntity *&pEnt) {
	AcDbPolyline *pLine =AcDbPolyline::cast (pEnt) ;
	if ( pLine == NULL )
		return ;

	resbuf *pRb =pLine->xData (_T(/*NOXLATE*/"ASDKOPLINE")) ;
	if ( pRb == NULL )
		return ;

	if ( pLine->upgradeOpen () != Acad::eOk )
		return ;

	//----- pMain should arrive first (because sub-entity(ies) were appended
	//----- but in case someone did erase it, we still have the sub-ent(s) around
	//----- we are going to use the first one which arrive and erase others...
	AsdkOPLine *pMain =new AsdkOPLine ;

	AsdkOPLineFiler filer (pRb) ;
	pMain->dwgIn (&filer) ;

	if ( pLine->handOverTo (pMain, true, true) == Acad::eObjectToBeDeleted ) {
		delete pLine ;
		pEnt =pMain ;
	} else {
		delete pMain ;
		return ;
	}

	resbuf *pRbClear =acutNewRb (AcDb::kDxfRegAppName) ;
	pRbClear->resval.rstring =strdup (_T(/*NOXLATE*/"MKRDWGFILER")) ;
	pEnt->setXData (pRbClear) ;
	acutRelRb (pRbClear) ;

	//----- We just need to erase sub-entity(ies) if needed...
	resbuf *pStc =pRb ;
	for ( ; pStc != NULL ; pStc =pStc->rbnext ) {
		if ( pStc->restype != AcDb::kDxfXdAsciiString )
			continue ;
		if ( _tcscmp (_T(/*NOXLATE*/"SUB_ENTITY_MARKER"), pStc->resval.rstring) != 0 )
			continue ;
		//----- All the next are sub-entity(ies) to erase...
		for ( pStc =pStc->rbnext ; pStc != NULL ; pStc =pStc->rbnext ) {
			AcDbHandle hd (pStc->resval.rstring) ;
			AcDbObjectId id ;
			pMain->database ()->getAcDbObjectId (id, false, hd) ;
			if ( id.isValid () && id != pMain->objectId () ) {
				AcDbEntity *p ;
				if ( acdbOpenAcDbEntity (p, id, AcDb::kForWrite) == Acad::eOk ) {
					p->erase (true) ;
					p->close () ;
				}
			}
		}
		return ;
	}

}

typedef void (*ApplyFunc) (AcDbBlockTableRecord *pBTR, AcDbEntity *&pEnt) ;

void forEach (AcDbDatabase *pDwg, AcRxClass *pClass, ApplyFunc pFunc) {
	if ( acDocManager->lockDocument (acDocManager->document (pDwg)) != Acad::eOk )
		return ;

	AcDbBlockTable *pBT ;
	if ( pDwg->getBlockTable (pBT, AcDb::kForRead) != Acad::eOk )
		return ;
	AcDbBlockTableIterator *pBTi ;
	if ( pBT->newIterator (pBTi) != Acad::eOk ) {
		pBT->close () ;
		return ;
	}

	for ( ; !pBTi->done () ; pBTi->step () ) {
		AcDbBlockTableRecord *pBTR ;
		if ( pBTi->getRecord (pBTR, AcDb::kForRead) != Acad::eOk )
			continue ;

		AcDbBlockTableRecordIterator *pBTRi ;
		if ( pBTR->newIterator (pBTRi) != Acad::eOk ) {
			pBTR->close () ;
			continue ;
		}

		for ( ; !pBTRi->done () ; pBTRi->step () ) {
			AcDbEntity *pEnt ;
			if ( pBTRi->getEntity (pEnt, AcDb::kForRead) != Acad::eOk )
				continue ;
			if ( pEnt->isKindOf (pClass) )
				(*pFunc) (pBTR, pEnt) ;
			pEnt->close () ;
		}
		delete pBTRi ;
		pBTR->close () ;
	}
	delete pBTi ;
	pBT->close () ;

	acDocManager->unlockDocument (acDocManager->document (pDwg)) ;
}

//-----------------------------------------------------------------------------
AsdkEdReact::AsdkEdReact(const bool autoInitAndRelease)
{
	m_autoInitAndRelease = autoInitAndRelease;
	if (m_autoInitAndRelease)
		if (NULL != acedEditor)
			acedEditor->addReactor(this);
		else
			m_autoInitAndRelease = false;
}

AsdkEdReact::~AsdkEdReact()
{
	if (m_autoInitAndRelease)
		if (NULL != acedEditor)
			acedEditor->removeReactor(this);
}

void AsdkEdReact::dwgFileOpened(AcDbDatabase* x0,
                                char* fileName)
{
	// TODO: implement this function.
    saveComplete (x0, fileName) ;
}

void AsdkEdReact::beginSave(AcDbDatabase* x0,
                            const char* pIntendedName)
{
	// TODO: implement this function.
	x0->disableUndoRecording (true) ;
	acdbRegApp (_T(/*NOXLATE*/"ASDKOPLINE")) ;
	forEach (x0, AsdkOPLine::desc (), ToStdAcadEntity) ;
	x0->disableUndoRecording (false) ;
}

void AsdkEdReact::saveComplete(AcDbDatabase* x0,
                               const char* pActualName)
{
	// TODO: implement this function.
	x0->disableUndoRecording (true) ;
	forEach (x0, AcDbPolyline::desc (), BackToOPLine) ;
	x0->disableUndoRecording (false) ;
}

void AsdkEdReact::abortSave(AcDbDatabase* x0)
{
	// TODO: implement this function.
    saveComplete (x0, _T(/*NOXLATE*/"whatever")) ;
}

