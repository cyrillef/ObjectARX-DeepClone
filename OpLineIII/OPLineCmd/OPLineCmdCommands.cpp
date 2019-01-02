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
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"

// This is command 'OPL'
void Asdkopl()
{
	// TODO: Implement the command
	ads_name en ;
	ads_point pt ;
	if ( acedEntSel (NULL, en, pt) != RTNORM )
		return ;

	AcDbObjectId id ;
	acdbGetObjectId (id, en) ;

	AsdkOPLine *p =new AsdkOPLine ;
	p->makeFrom (id) ;

	AcDbDictionary *pNOD ;
	acdbHostApplicationServices ()->workingDatabase ()->getNamedObjectsDictionary (pNOD, AcDb::kForRead) ;
	AcDbDictionary *pDct ;
	if ( pNOD->getAt (ASDK_OPLINESTYLE, (AcDbObject *&)pDct, AcDb::kForRead) == Acad::eKeyNotFound ) {
		pNOD->close () ;
		Asdkopls () ;
		acdbHostApplicationServices ()->workingDatabase ()->getNamedObjectsDictionary (pNOD, AcDb::kForRead) ;
		pNOD->getAt (ASDK_OPLINESTYLE, (AcDbObject *&)pDct, AcDb::kForRead) ;
	}
	pNOD->close () ;
	pDct->getAt (ASDK_OPLINESTYLE_STD, id) ;
	pDct->close () ;
	p->setOPLineStyle (id) ;
	p->close () ;
}

// This is command 'OPLS'
void Asdkopls()
{
	// TODO: Implement the command
	AcDbObjectId ids ;
	AcDbSymbolUtilities::getLinetypeId (ids, _T("BYLAYER"), acdbHostApplicationServices ()->workingDatabase ()) ;

	AsdkOPLineStyle *style =new AsdkOPLineStyle ;
	style->appendOffset (2, ids, 1, AcDb::kLnWtByLwDefault) ;
	style->appendOffset (-2, ids, 3, AcDb::kLnWtByLwDefault) ;

	AcDbDictionary *pNOD ;
	acdbHostApplicationServices ()->workingDatabase ()->getNamedObjectsDictionary (pNOD, AcDb::kForWrite) ;
	AcDbDictionary *pDct =new AcDbDictionary ;
	pNOD->setAt (ASDK_OPLINESTYLE, pDct, ids) ;
	pNOD->close () ;
	pDct->setAt (ASDK_OPLINESTYLE_STD, style, ids) ;
	pDct->close () ;
	style->close () ;
}

