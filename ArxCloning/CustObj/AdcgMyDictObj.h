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
// AdcgMyDictObj custom object.

#if !defined(ARX__ADCGMYDICTOBJ_H__19990504_093335)
#define ARX__ADCGMYDICTOBJ_H__19990504_093335

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4275 4251)

#ifdef _CUSTOBJ_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ADCGCUSTOBJ_DBXSERVICE
#define ADCGCUSTOBJ_DBXSERVICE "ADCGCUSTOBJ_DBXSERVICE"
#endif



#include "dbmain.h"


class DLLIMPEXP AdcgMyDictObj : public AcDbObject
{
public:

	ACRX_DECLARE_MEMBERS(AdcgMyDictObj);

	// Constructor / Destructor
	AdcgMyDictObj();
	virtual ~AdcgMyDictObj();

	//{{AFX_ARX_METHODS(AdcgMyDictObj)
	virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler* pFiler) const;
	virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler* pFiler);
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	virtual Acad::ErrorStatus wblockClone(AcRxObject* pOwnerObject,
	                                      AcDbObject*& pClonedObject,
	                                      AcDbIdMapping& idMap,
	                                      Adesk::Boolean isPrimary = true) const;
	virtual Acad::ErrorStatus deepClone(AcDbObject* pOwnerObject,
	                                    AcDbObject*& pClonedObject,
	                                    AcDbIdMapping& idMap,
	                                    Adesk::Boolean isPrimary = true) const;
	//}}AFX_ARX_METHODS


	//{{AFX_ARX_DATA_ACCESS(AdcgMyDictObj)
	Acad::ErrorStatus setToLine2(const AcDbSoftPointerId ToLine2);
	Acad::ErrorStatus toLine2(AcDbSoftPointerId& ToLine2);
	Acad::ErrorStatus setToLine1(const AcDbSoftPointerId ToLine1);
	Acad::ErrorStatus toLine1(AcDbSoftPointerId& ToLine1);
	Acad::ErrorStatus setMyLine(const AcDbSoftPointerId MyLine);
	Acad::ErrorStatus myLine(AcDbSoftPointerId& MyLine);
	//}}AFX_ARX_DATA_ACCESS


private:
	//{{AFX_ARX_DATA(AdcgMyDictObj)
	AcDbSoftPointerId m_ToLine2;
	AcDbSoftPointerId m_ToLine1;
	AcDbSoftPointerId m_MyLine;
	//}}AFX_ARX_DATA


private:

	void *operator new[](unsigned nSize) { return 0; }
	void operator delete[](void *p) {};
	void *operator new[](unsigned nSize, const char *file, int line) { return 0; }

};



#endif // !defined(ARX__ADCGMYDICTOBJ_H__19990504_093335)