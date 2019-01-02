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
// AsdkOPLineReactor custom object.

#if !defined(ARX__ASDKOPLINEREACTOR_H__19991123_115752)
#define ARX__ASDKOPLINEREACTOR_H__19991123_115752

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4275 4251)

#ifdef _OPLINE_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ASDKOPLINE_DBXSERVICE
#define ASDKOPLINE_DBXSERVICE "ASDKOPLINE_DBXSERVICE"
#endif

#define ASDK_OPLINEREACTOR _T(/*NOXLATE*/"ASDK_OPLINEREACTOR")


#include "dbmain.h"

class DLLIMPEXP AsdkOPLineReactor : public AcDbObject
{
public:

	ACRX_DECLARE_MEMBERS(AsdkOPLineReactor);

	// Constructor / Destructor
	AsdkOPLineReactor();
	virtual ~AsdkOPLineReactor();

	//{{AFX_ARX_METHODS(AsdkOPLineReactor)
	virtual void erased(const AcDbObject* dbObj,
	                    Adesk::Boolean pErasing = true);
	virtual Acad::ErrorStatus deepClone(AcDbObject* pOwnerObject,
	                                    AcDbObject*& pClonedObject,
	                                    AcDbIdMapping& idMap,
	                                    Adesk::Boolean isPrimary = true) const;
	virtual void modified(const AcDbObject* dbObj);
	virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler* pFiler) const;
	virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler* pFiler);
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS


	//{{AFX_ARX_DATA_ACCESS(AsdkOPLineReactor)
	//}}AFX_ARX_DATA_ACCESS
	Acad::ErrorStatus setOPLineStyle(const AcDbHardPointerId OPLineStyle);
	Acad::ErrorStatus oPLineStyle(AcDbHardPointerId& OPLineStyle);
	AcArray<AcDbSoftPointerId> &entsToUpdate () { return (m_entsToUpdate) ; }

	CString printIt () ;

	void modify(const AcDbObject* dbObj) ;
	void MakeOPLine (const AcDbObject *dbObj) ;
	void ModifyOPLineFrom (AcDbObjectId mainId, const AcDbObject *dbObj) ;

private:
	//{{AFX_ARX_DATA(AsdkOPLineReactor)
	//}}AFX_ARX_DATA
	AcDbHardPointerId m_OPLineStyle;  // This is the objectId of the OffsetStyle
	AcArray<AcDbSoftPointerId> m_entsToUpdate; // This is the objectId of all children

public:
	static bool m_bActive ;
//	static AcArray<AcDbObjectId> m_pLinesModified ;

private:

	void *operator new[](unsigned nSize) { return 0; }
	void operator delete[](void *p) {};
	void *operator new[](unsigned nSize, const char *file, int line) { return 0; }

};

int DLLIMPEXP AsdkGetPLinesModifiedLength () ;
AcDbObjectId DLLIMPEXP AsdkGetPLinesModified (int i) ;
void DLLIMPEXP AsdkGetPLinesModifiedReset () ;

void DLLIMPEXP AsdkOPLineReactorActive (bool flag =true) ;

#endif // !defined(ARX__ASDKOPLINEREACTOR_H__19991123_115752)