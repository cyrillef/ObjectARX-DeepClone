//
// (C) Copyright 1994-1999 by Autodesk, Inc. 
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

/////////////////////////////////////////////
// AcEditorReactor reactors.

#include "StdAfx.h"
#include "StdArx.h"


AdcgCMyEdReactor::AdcgCMyEdReactor(const bool autoInitAndRelease)
{
	m_autoInitAndRelease = autoInitAndRelease;
	if (m_autoInitAndRelease)
		if (NULL != acedEditor)
			acedEditor->addReactor(this);
		else
			m_autoInitAndRelease = false;
}

AdcgCMyEdReactor::~AdcgCMyEdReactor()
{
	if (m_autoInitAndRelease)
		if (NULL != acedEditor)
			acedEditor->removeReactor(this);
}

void AdcgCMyEdReactor::beginDeepCloneXlation(AcDbIdMapping& x0,
                                             Acad::ErrorStatus* x1)
{
	switch ( x0.deepCloneContext () ) {
		case AcDb::kDcCopy:
			acutPrintf ("DeepClone: kDcCopy\n") ;
			break ;
		case AcDb::kDcExplode:
			acutPrintf ("DeepClone: kDcExplode\n") ;
			break ;
		case AcDb::kDcBlock:
			acutPrintf ("DeepClone: kDcBlock\n") ;
			break ;
		case AcDb::kDcXrefBind:
			acutPrintf ("DeepClone: kDcXrefBind\n") ;
			break ;
		case AcDb::kDcSymTableMerge:
			acutPrintf ("DeepClone: kDcSymTableMerge\n") ;
			break ;
		case AcDb::kDcInsert:
			acutPrintf ("DeepClone: kDcInsert\n") ;
			break ;
		case AcDb::kDcWblock:
			acutPrintf ("DeepClone: kDcWblock\n") ;
			break ;
		case AcDb::kDcObjects:
			acutPrintf ("DeepClone: kDcObjects\n") ;
			break ;
		case AcDb::kDcXrefInsert:
			acutPrintf ("DeepClone: kDcXrefInsert\n") ;
			break ;
		case AcDb::kDcInsertCopy:
			acutPrintf ("DeepClone: kDcInsertCopy\n") ;
			break ;
		case AcDb::kDcWblkObjects:
			acutPrintf ("DeepClone: kDcWblkObjects\n") ;
			break ;
	}

}

