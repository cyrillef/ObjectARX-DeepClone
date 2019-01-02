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
// AdcgMyLine custom object.

#include "StdAfx.h"
#include "StdArx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"

#define VERSION_ADCGMYLINE 1


//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AdcgMyLine, AcDbLine,
                        AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyEntity::kNoOperation,
                        ADCGMYLINE, AdcgCustObj
);
//}}AFX_ARX_MACRO


// Constructor / Destructor

AdcgMyLine::AdcgMyLine()
{
	// TODO: do your initialization.

}

AdcgMyLine::~AdcgMyLine()
{
	// TODO: clean up.

}


Acad::ErrorStatus AdcgMyLine::deepClone(AcDbObject* pOwnerObject,
                                        AcDbObject*& pClonedObject,
                                        AcDbIdMapping& idMap,
                                        Adesk::Boolean isPrimary) const
{
	assertReadEnabled () ;

	//----- (1) Make a clone of ourself (this is the default implementation)
	Acad::ErrorStatus es ;
	es =AcDbObject::deepClone (pOwnerObject, pClonedObject, idMap, isPrimary) ;

	//----- If successful, make a clone of the dictionary
	AdcgMyDictObj *pClone =NULL ;
	AcDbSoftPointerId idl1, idl2 ;
	switch ( idMap.deepCloneContext () ) {
		case AcDb::kDcCopy: //----- (2)
		case AcDb::kDcObjects:
		case AcDb::kDcExplode:
			//----- (3) If the dict object is not yet cloned, clone it!
			if ( !idMap.compute (AcDbIdPair (m_MyObj, AcDbObjectId::kNull, true)) ) {
				//----- (4) Open the orignal dictionary object we want to clone
				AdcgMyDictObj *pObj ;
				acdbOpenAcDbObject ((AcDbObject *&)pObj, m_MyObj, AcDb::kForRead) ;
				//----- (5) Open the dictionary owner of the original dictionary object
				AcDbDictionary *pDict ;
				acdbOpenAcDbObject ((AcDbObject *&)pDict, pObj->ownerId (), AcDb::kForWrite) ;
				//----- (6) Clone the dictionary object
				pObj->deepClone (pDict, (AcDbObject *&)pClone, idMap, isPrimary) ;
				//----- (7) Assign its owner ourself with an anonymous key
				AcDbObjectId idc ;
				//pDict->setAt ("*", pClone, idc) ;
				//----- (8) Close opened objects
				pDict->close () ;
				pObj->close () ;
			}
			//----- Then clone Line1
			pClone->toLine1 (idl1) ;
			if ( !idMap.compute (AcDbIdPair (idl1, AcDbObjectId::kNull, true)) ) {
				//----- Open the orignal line we want to clone
				AcDbLine *pl ;
				acdbOpenAcDbObject ((AcDbObject *&)pl, idl1, AcDb::kForRead) ;
				//----- Clone the line
				AcDbLine *pcl1 ;
				pl->deepClone (pOwnerObject, (AcDbObject *&)pcl1, idMap, isPrimary) ;
				//----- Close opened object
				pl->close () ;
				//----- We *must* close the cloned line ourself
				pcl1->close () ;
			}
			//----- Then clone Line2
			pClone->toLine2 (idl2) ;
			if ( !idMap.compute (AcDbIdPair (idl2, AcDbObjectId::kNull, true)) ) {
				//----- Open the orignal line we want to clone
				AcDbLine *pl ;
				acdbOpenAcDbObject ((AcDbObject *&)pl, idl2, AcDb::kForRead) ;
				//----- Clone the line
				AcDbLine *pcl2 ;
				pl->deepClone (pOwnerObject, (AcDbObject *&)pcl2, idMap, isPrimary) ;
				//----- Close opened object
				pl->close () ;
				//----- We *must* close the cloned line ourself
				pcl2->close () ;
			}
			//----- (9) We *must* close the cloned dictionary object ourself
			if ( pClone != NULL )
				pClone->close () ;
			break ;
		case AcDb::kDcInsertCopy:
		case AcDb::kDcInsert:
			if ( !idMap.compute (AcDbIdPair (m_MyObj, AcDbObjectId::kNull, true)) ) {
				//----- Open the orignal dictionary object we want to clone
				AdcgMyDictObj *pObj ;
				acdbOpenAcDbObject ((AcDbObject *&)pObj, m_MyObj, AcDb::kForRead) ;
				//----- Clone the dictionary owner if necessary, if not open it for write
				AcDbDatabase *pDestDb =NULL ;
				idMap.destDb (pDestDb) ;
				AcDbDictionary *pNOD =NULL, *pClonedDict =NULL ;
				pDestDb->getNamedObjectsDictionary (pNOD, AcDb::kForRead) ;
				if ( pNOD->has ("MyDict") ) {
					//----- It is there
					es =pNOD->getAt ("MyDict", (AcDbObject *&)pClonedDict, AcDb::kForWrite) ;
					//----- Clone the dictionary object now
					if ( pObj->deepClone (pClonedDict, (AcDbObject *&)pClone, idMap, isPrimary) == Acad::eOk ) {
						//----- Assign its owner ourself with an anonymous key
						AcDbObjectId idc ;
						//es =pClonedDict->setAt ("*", pClone, idc) ;
						if ( pClone != NULL && pClone != pObj )
							pClone->close () ;
					}
					pClonedDict->close () ;
				} else {
					//----- Clone it
					//----- Open the dictionary owner of the original dictionary object
					AcDbDictionary *pDict =NULL ;
					acdbOpenAcDbObject ((AcDbObject *&)pDict, pObj->ownerId (), AcDb::kForRead) ;
					pNOD->upgradeOpen () ;
					pDict->deepClone (pNOD, (AcDbObject *&)pClonedDict, idMap, isPrimary) ;
					AcDbObjectId idcd ;
					pNOD->setAt ("MyDict", pClonedDict, idcd) ;
					idMap.assign (AcDbIdPair (pDict->ownerId (), pClonedDict->ownerId (), false, false, false)) ;
					pNOD->downgradeOpen () ;
					//----- We *must* close the clones ourself
					if ( pClonedDict != NULL && pClonedDict != pDict)
						pClonedDict->close () ;
					pDict->close () ;
					//----- We *must* close the clones ourself
					if ( pClonedDict != NULL && pClonedDict != pDict)
						pClonedDict->close () ;
					pDict->close () ;
				}
				pNOD->close () ;
				//----- Close opened objects
				pObj->close () ;
			}
			break ;
	}

	return (es) ;
}

Acad::ErrorStatus AdcgMyLine::wblockClone(AcRxObject* pOwnerObject,
                                          AcDbObject*& pClonedObject,
                                          AcDbIdMapping& idMap,
                                          Adesk::Boolean isPrimary) const
{
	assertReadEnabled();

	//----- Make a clone of ourself (this is the default implementation)
	Acad::ErrorStatus es =AcDbObject::wblockClone (pOwnerObject, pClonedObject, idMap, isPrimary) ;

	//----- Open the cloned dictionary objects
	AdcgMyDictObj *pObj ;
	acdbOpenAcDbObject ((AcDbObject *&)pObj, m_MyObj, AcDb::kForRead) ;
	//----- Collect the lines ID's
	AcDbSoftPointerId idl1, idl2 ;
	pObj->toLine1 (idl1) ;
	pObj->toLine2 (idl2) ;
	pObj->close () ;
	//----- Clone the lines
	AcDbLine *pl, *pcl ;
	acdbOpenAcDbObject ((AcDbObject *&)pl, idl1, AcDb::kForRead) ;
	pl->wblockClone (pOwnerObject, (AcDbObject *&)pcl, idMap, isPrimary) ;
	pcl->close () ;
	pl->close () ;

	acdbOpenAcDbObject ((AcDbObject *&)pl, idl2, AcDb::kForRead) ;
	pl->wblockClone (pOwnerObject, (AcDbObject *&)pcl, idMap, isPrimary) ;
	pcl->close () ;
	pl->close () ;

	return (es) ;
}

Acad::ErrorStatus AdcgMyLine::myObj(AcDbHardPointerId& MyObj)
{
	assertReadEnabled();
	MyObj = m_MyObj;
	return Acad::eOk;
}

Acad::ErrorStatus AdcgMyLine::setMyObj(const AcDbHardPointerId MyObj)
{
	assertWriteEnabled();
	m_MyObj = MyObj;
  return Acad::eOk;
}

Acad::ErrorStatus AdcgMyLine::dwgInFields(AcDbDwgFiler* pFiler)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	// Call dwgInFields from AcDbLine
	if ((es = AcDbLine::dwgInFields(pFiler)) != Acad::eOk) {
		return es;
	}

	// Read version number.
	Adesk::UInt16 version;
	pFiler->readItem(&version);
	if (version > VERSION_ADCGMYLINE)
		return Acad::eMakeMeProxy;

	// Read the data members.
	switch (version)
	{
	case (1):
		pFiler->readItem(&m_MyObj);
		// TODO: here you can file datamembers not
		//       created by the ObjectARX Add-In.

		break;
	}


	return pFiler->filerStatus();
}

Acad::ErrorStatus AdcgMyLine::dwgOutFields(AcDbDwgFiler* pFiler) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbLine
	if ((es = AcDbLine::dwgOutFields(pFiler)) != Acad::eOk) {
		return es;
	}

	// Write version number.
	pFiler->writeItem((Adesk::UInt16) VERSION_ADCGMYLINE);

	// Write the data members.
	pFiler->writeItem(m_MyObj);
	// TODO: here you can file datamembers not
	//       created by the ObjectARX Add-In.


	return pFiler->filerStatus();
}

Acad::ErrorStatus AdcgMyLine::dxfInFields(AcDbDxfFiler* pFiler)
{
	assertWriteEnabled();
	struct resbuf rb;

	if ((AcDbLine::dxfInFields(pFiler) != Acad::eOk) ||
	    !pFiler->atSubclassData("AdcgMyLine"))
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
		if (version > VERSION_ADCGMYLINE)
			return Acad::eMakeMeProxy;
	}

	pFiler->readItem(&rb);
	if (rb.restype == AcDb::kDxfHardPointerId) {
		m_MyObj.setFromOldId(rb.resval.rlong);
	} else {
		pFiler->pushBackItem();
		pFiler->setError(Acad::eInvalidDxfCode,
		                "nError: expected group code %d",
		                AcDb::kDxfHardPointerId);
		return pFiler->filerStatus();
	}

	// TODO: here you can file datamembers not
	//       created by the ObjectARX Add-In.

	return pFiler->filerStatus();
}

Acad::ErrorStatus AdcgMyLine::dxfOutFields(AcDbDxfFiler* pFiler) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	if ((es = AcDbLine::dxfOutFields(pFiler)) != Acad::eOk)
		return es;

	// Write version number.
	pFiler->writeItem(AcDb::kDxfInt16, (Adesk::UInt16) VERSION_ADCGMYLINE);

	pFiler->writeItem(AcDb::kDxfSubclass, "AdcgMyLine");
	pFiler->writeItem(AcDb::kDxfHardPointerId, m_MyObj);

	// TODO: here you can file datamembers not
	//       created by the ObjectARX Add-In.

	return es;
}

