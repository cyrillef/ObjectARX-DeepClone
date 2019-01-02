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

#include "StdAfx.h"
#include "StdArx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"

#define VERSION_ADCGMYDICTOBJ 1


//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AdcgMyDictObj, AcDbObject,
                        AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
                        ADCGMYDICTOBJ, AdcgCustObj
);
//}}AFX_ARX_MACRO


// Constructor / Destructor

AdcgMyDictObj::AdcgMyDictObj()
{
	// TODO: do your initialization.

}

AdcgMyDictObj::~AdcgMyDictObj()
{
	// TODO: clean up.

}


Acad::ErrorStatus AdcgMyDictObj::deepClone(AcDbObject* pOwnerObject,
                                           AcDbObject*& pClonedObject,
                                           AcDbIdMapping& idMap,
                                           Adesk::Boolean isPrimary) const
{
	assertReadEnabled();

	//----- Make a clone of ourself (this is the default implementation)
	Acad::ErrorStatus es =Acad::eVetoed ;
	if ( idMap.compute (AcDbIdPair (m_MyLine, AcDbObjectId::kNull, true)) ) {
		es =AcDbObject::deepClone (pOwnerObject, pClonedObject, idMap, isPrimary) ;
	}

	return (es) ;
}

Acad::ErrorStatus AdcgMyDictObj::wblockClone(AcRxObject* pOwnerObject,
                                             AcDbObject*& pClonedObject,
                                             AcDbIdMapping& idMap,
                                             Adesk::Boolean isPrimary) const
{
	assertReadEnabled();

	//----- Make a clone of ourself (this is the default implementation)
	Acad::ErrorStatus es =Acad::eVetoed ;
	if ( idMap.compute (AcDbIdPair (m_MyLine, AcDbObjectId::kNull, true)) ) {
		es =AcDbObject::wblockClone (pOwnerObject, pClonedObject, idMap, isPrimary) ;
	}

	return (es) ;
}

Acad::ErrorStatus AdcgMyDictObj::myLine(AcDbSoftPointerId& MyLine)
{
	assertReadEnabled();
	MyLine = m_MyLine;
	return Acad::eOk;
}

Acad::ErrorStatus AdcgMyDictObj::setMyLine(const AcDbSoftPointerId MyLine)
{
	assertWriteEnabled();
	m_MyLine = MyLine;
  return Acad::eOk;
}

Acad::ErrorStatus AdcgMyDictObj::toLine1(AcDbSoftPointerId& ToLine1)
{
	assertReadEnabled();
	ToLine1 = m_ToLine1;
	return Acad::eOk;
}

Acad::ErrorStatus AdcgMyDictObj::setToLine1(const AcDbSoftPointerId ToLine1)
{
	assertWriteEnabled();
	m_ToLine1 = ToLine1;
  return Acad::eOk;
}

Acad::ErrorStatus AdcgMyDictObj::toLine2(AcDbSoftPointerId& ToLine2)
{
	assertReadEnabled();
	ToLine2 = m_ToLine2;
	return Acad::eOk;
}

Acad::ErrorStatus AdcgMyDictObj::setToLine2(const AcDbSoftPointerId ToLine2)
{
	assertWriteEnabled();
	m_ToLine2 = ToLine2;
  return Acad::eOk;
}

Acad::ErrorStatus AdcgMyDictObj::dwgInFields(AcDbDwgFiler* pFiler)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	// Call dwgInFields from AcDbObject
	if ((es = AcDbObject::dwgInFields(pFiler)) != Acad::eOk) {
		return es;
	}

	// Read version number.
	Adesk::UInt16 version;
	pFiler->readItem(&version);
	if (version > VERSION_ADCGMYDICTOBJ)
		return Acad::eMakeMeProxy;

	// Read the data members.
	switch (version)
	{
	case (1):
		pFiler->readItem(&m_MyLine);
		pFiler->readItem(&m_ToLine1);
		pFiler->readItem(&m_ToLine2);
		// TODO: here you can file datamembers not
		//       created by the ObjectARX Add-In.

		break;
	}


	return pFiler->filerStatus();
}

Acad::ErrorStatus AdcgMyDictObj::dwgOutFields(AcDbDwgFiler* pFiler) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbObject
	if ((es = AcDbObject::dwgOutFields(pFiler)) != Acad::eOk) {
		return es;
	}

	// Write version number.
	pFiler->writeItem((Adesk::UInt16) VERSION_ADCGMYDICTOBJ);

	// Write the data members.
	pFiler->writeItem(m_MyLine);
	pFiler->writeItem(m_ToLine1);
	pFiler->writeItem(m_ToLine2);
	// TODO: here you can file datamembers not
	//       created by the ObjectARX Add-In.


	return pFiler->filerStatus();
}

Acad::ErrorStatus AdcgMyDictObj::dxfInFields(AcDbDxfFiler* pFiler)
{
	assertWriteEnabled();
	struct resbuf rb;

	if ((AcDbObject::dxfInFields(pFiler) != Acad::eOk) ||
	    !pFiler->atSubclassData("AdcgMyDictObj"))
		return pFiler->filerStatus();

	// Read version number.
	pFiler->readItem(&rb);
	if (rb.restype != AcDb::kDxfInt16) {
		pFiler->pushBackItem();
		pFiler->setError(Acad::eInvalidDxfCode,
		                "nError: expected object version group code %d",
		                 AcDb::kDxfInt16);
		return pFiler->filerStatus();
	} else {
		Adesk::UInt16 version = rb.resval.rint;
		if (version > VERSION_ADCGMYDICTOBJ)
			return Acad::eMakeMeProxy;
	}

	pFiler->readItem(&rb);
	if (rb.restype == AcDb::kDxfSoftPointerId) {
		m_MyLine.setFromOldId(rb.resval.rlong);
	} else {
		pFiler->pushBackItem();
		pFiler->setError(Acad::eInvalidDxfCode,
		                "nError: expected group code %d",
		                AcDb::kDxfSoftPointerId);
		return pFiler->filerStatus();
	}

	pFiler->readItem(&rb);
	if (rb.restype == AcDb::kDxfSoftPointerId + 1) {
		m_ToLine1.setFromOldId(rb.resval.rlong);
	} else {
		pFiler->pushBackItem();
		pFiler->setError(Acad::eInvalidDxfCode,
		                "nError: expected group code %d",
		                AcDb::kDxfSoftPointerId + 1);
		return pFiler->filerStatus();
	}

	pFiler->readItem(&rb);
	if (rb.restype == AcDb::kDxfSoftPointerId + 2) {
		m_ToLine2.setFromOldId(rb.resval.rlong);
	} else {
		pFiler->pushBackItem();
		pFiler->setError(Acad::eInvalidDxfCode,
		                "nError: expected group code %d",
		                AcDb::kDxfSoftPointerId + 2);
		return pFiler->filerStatus();
	}

	// TODO: here you can file datamembers not
	//       created by the ObjectARX Add-In.

	return pFiler->filerStatus();
}

Acad::ErrorStatus AdcgMyDictObj::dxfOutFields(AcDbDxfFiler* pFiler) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	if ((es = AcDbObject::dxfOutFields(pFiler)) != Acad::eOk)
		return es;

	// Write version number.
	pFiler->writeItem(AcDb::kDxfInt16, (Adesk::UInt16) VERSION_ADCGMYDICTOBJ);

	pFiler->writeItem(AcDb::kDxfSubclass, "AdcgMyDictObj");
	pFiler->writeItem(AcDb::kDxfSoftPointerId, m_MyLine);
	pFiler->writeItem(AcDb::kDxfSoftPointerId + 1, m_ToLine1);
	pFiler->writeItem(AcDb::kDxfSoftPointerId + 2, m_ToLine2);

	// TODO: here you can file datamembers not
	//       created by the ObjectARX Add-In.

	return es;
}

