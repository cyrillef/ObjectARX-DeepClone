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
// AsdkOPLine custom object.

#if !defined(ARX__ASDKOPLINE_H__19991119_131300)
#define ARX__ASDKOPLINE_H__19991119_131300

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

#include "dbpl.h"

class DLLIMPEXP AsdkOPLine : public AcDbPolyline
{
public:

	ACRX_DECLARE_MEMBERS(AsdkOPLine);

	// Constructor / Destructor
	AsdkOPLine(AcDbObjectId id);
	AsdkOPLine(unsigned int num_verts =0, AcDbObjectId id =AcDbObjectId::kNull);
	virtual ~AsdkOPLine();

	Acad::ErrorStatus makeFrom (AcDbObjectId id) ;
	Acad::ErrorStatus makeFrom (AcDbPolyline *pLine) ;

	//{{AFX_ARX_METHODS(AsdkOPLine)
	virtual Acad::ErrorStatus deepClone(AcDbObject* pOwnerObject,
	                                    AcDbObject*& pClonedObject,
	                                    AcDbIdMapping& idMap,
	                                    Adesk::Boolean isPrimary = true) const;
	virtual Acad::ErrorStatus explode(AcDbVoidPtrArray& entitySet) const;
	virtual void saveAs(AcGiWorldDraw* mode, AcDb::SaveType st);
	virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler* filer) const;
	virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler* filer);
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* filer) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* filer);
	virtual Adesk::Boolean worldDraw(AcGiWorldDraw* mode);
	//}}AFX_ARX_METHODS


	//{{AFX_ARX_DATA_ACCESS(AsdkOPLine)
	Acad::ErrorStatus setOPLineStyle(const AcDbHardPointerId OPLineStyle);
	Acad::ErrorStatus oPLineStyle(AcDbHardPointerId& OPLineStyle);
	//}}AFX_ARX_DATA_ACCESS


private:
	//{{AFX_ARX_DATA(AsdkOPLine)
	AcDbHardPointerId m_OPLineStyle;
	//}}AFX_ARX_DATA

private:

	void *operator new[](unsigned nSize) { return 0; }
	void operator delete[](void *p) {};
	void *operator new[](unsigned nSize, const char *file, int line) { return 0; }

};



#endif // !defined(ARX__ASDKOPLINE_H__19991119_131300)