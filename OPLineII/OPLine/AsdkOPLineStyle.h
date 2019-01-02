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
// AsdkOPLineStyle custom object.

#if !defined(ARX__ASDKOPLINESTYLE_H__19991119_131513)
#define ARX__ASDKOPLINESTYLE_H__19991119_131513

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


#define ASDK_OPLINESTYLE		_T(/*NOXLATE*/"Asdk_OPLineStyle")
#define ASDK_OPLINESTYLE_STD	_T(/*NOXLATE*/"Standard")

#include "dbmain.h"


class DLLIMPEXP AsdkOPLineStyle : public AcDbObject
{
public:

	ACRX_DECLARE_MEMBERS(AsdkOPLineStyle);

	// Constructor / Destructor
	AsdkOPLineStyle();
	virtual ~AsdkOPLineStyle();

	//{{AFX_ARX_METHODS(AsdkOPLineStyle)
	virtual Acad::ErrorStatus wblockClone(AcRxObject* pOwnerObject,
	                                      AcDbObject*& pClonedObject,
	                                      AcDbIdMapping& idMap,
	                                      Adesk::Boolean isPrimary = true) const;
	virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler* pFiler) const;
	virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler* pFiler);
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS


	//{{AFX_ARX_DATA_ACCESS(AsdkOPLineStyle)
	//}}AFX_ARX_DATA_ACCESS
	virtual Acad::ErrorStatus setName (const char *name) ;
	virtual char *getName () ;
	virtual Acad::ErrorStatus setOffsetOf (Adesk::UInt32 index, double off) ;
	virtual Acad::ErrorStatus offsetOf (Adesk::UInt32 index, double &off) ;
	virtual Acad::ErrorStatus setLinetypeOf (Adesk::UInt32 index, AcDbObjectId eId) ;
	virtual Acad::ErrorStatus linetypeOf (Adesk::UInt32 index, AcDbObjectId &eId) ;
	virtual Acad::ErrorStatus setColorOf (Adesk::UInt32 index, Adesk::UInt16 color) ;
	virtual Acad::ErrorStatus colorOf (Adesk::UInt32 index, Adesk::UInt16 &color) ;
	virtual Acad::ErrorStatus setLineWeightOf (Adesk::UInt32 index, AcDb::LineWeight lw) ;
	virtual Acad::ErrorStatus lineWeightOf (Adesk::UInt32 index, AcDb::LineWeight &lw) ;

	virtual Acad::ErrorStatus setStyleFrom (AcDbObjectId sourceId) ;
	virtual Acad::ErrorStatus appendOffset (double off) ;
	virtual Acad::ErrorStatus appendOffset (double off, AcDbObjectId ltId, Adesk::UInt16 color, AcDb::LineWeight lwt) ;
	virtual Acad::ErrorStatus deleteOffset (Adesk::UInt32 index) ;
	virtual Adesk::UInt32 numOffsets () ;

private:
	bool isEquivalentDuplicate (AcDbObjectId otherId, AcDb::DuplicateRecordCloning drc) const ;

	//{{AFX_ARX_DATA(AsdkOPLineStyle)
	//}}AFX_ARX_DATA
	Adesk::UInt32 m_iNumOffsets;
	AcArray<double> m_offsetArray;
	AcDbObjectIdArray m_ltypeArray;
	AcArray<Adesk::UInt16> m_colorArray;
	AcArray<AcDb::LineWeight> m_lweightArray;

private:

	void *operator new[](unsigned nSize) { return 0; }
	void operator delete[](void *p) {};
	void *operator new[](unsigned nSize, const char *file, int line) { return 0; }

};



#endif // !defined(ARX__ASDKOPLINESTYLE_H__19991119_131513)