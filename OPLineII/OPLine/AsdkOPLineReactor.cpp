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

//-----------------------------------------------------------------------------
//----- AsdkOPLineReactor custom object.
#include "StdAfx.h"
#include "StdArx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"

//-----------------------------------------------------------------------------
#define VERSION_ASDKOPLINEREACTOR 1

//-----------------------------------------------------------------------------
//----- When closing we will get an exception because a persistent reactor
//----- will require to be re-open'ed. Here we are in read mode already, hence
//----- it will fail. The following trick insures we can achieve our goal...
#define ARX_REACTOR_CLOSE(a,b); \
	{\
		AcDbObjectId idr =a->objectId () ;\
		a->close () ;\
		b->close () ;\
		b =NULL ;\
		AcDbObject *po ;\
		acdbOpenAcDbObject (po, idr, AcDb::kForRead, Adesk::kTrue) ;\
	}

void AsdkOPLineReactorActive (bool flag /*=true*/) {
	AsdkOPLineReactor::m_bActive =flag ;
}

//-----------------------------------------------------------------------------
//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AsdkOPLineReactor, AcDbObject,
                        AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
                        ASDKOPLINEREACTOR, AsdkOPLine
);
//}}AFX_ARX_MACRO

bool AsdkOPLineReactor::m_bActive =true ;
AcArray<AcDbObjectId> m_pLinesModified ;

int AsdkGetPLinesModifiedLength () {
	return (m_pLinesModified.length ()) ;
}

AcDbObjectId AsdkGetPLinesModified (int i) {
	return (m_pLinesModified.at (i)) ;
}

void AsdkGetPLinesModifiedReset () {
	m_pLinesModified.setLogicalLength (0) ;
}

//-----------------------------------------------------------------------------
//----- Constructor / Destructor
AsdkOPLineReactor::AsdkOPLineReactor () {
	// TODO: do your initialization.
}

AsdkOPLineReactor::~AsdkOPLineReactor () {
	// TODO: clean up.
}

//-----------------------------------------------------------------------------
//----- Filing protocol
Acad::ErrorStatus AsdkOPLineReactor::dwgInFields(AcDbDwgFiler* pFiler)
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
	if (version > VERSION_ASDKOPLINEREACTOR)
		return Acad::eMakeMeProxy;

	// Read the data members.
	switch (version)
	{
	case (1):
		// TODO: here you can file datamembers not
		//       created by the ObjectARX Add-In.
		pFiler->readHardPointerId (&m_OPLineStyle) ;
		Adesk::UInt32 numOffsets;
		pFiler->readUInt32 (&numOffsets) ;
		AcDbSoftPointerId eId ;
		m_entsToUpdate.setLogicalLength (0) ;
		for ( Adesk::UInt32 i =0 ; i < numOffsets ; i++ ) {
			pFiler->readSoftPointerId (&eId) ;
			m_entsToUpdate.append (eId) ;
		}
		break;
	}


	return pFiler->filerStatus();
}

Acad::ErrorStatus AsdkOPLineReactor::dwgOutFields(AcDbDwgFiler* pFiler) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbObject
	if ((es = AcDbObject::dwgOutFields(pFiler)) != Acad::eOk) {
		return es;
	}

	// Write version number.
	pFiler->writeItem((Adesk::UInt16) VERSION_ASDKOPLINEREACTOR);

	// Write the data members.
	// TODO: here you can file datamembers not
	//       created by the ObjectARX Add-In.
	pFiler->writeHardPointerId (m_OPLineStyle) ;
	pFiler->writeUInt32 (m_entsToUpdate.length ()) ;
	for ( int i =0 ; i < m_entsToUpdate.length () ; i++ )
		pFiler->writeSoftPointerId (m_entsToUpdate.at (i)) ;

	return pFiler->filerStatus();
}

Acad::ErrorStatus AsdkOPLineReactor::dxfInFields(AcDbDxfFiler* pFiler)
{
	assertWriteEnabled();
	struct resbuf rb;

	if ((AcDbObject::dxfInFields(pFiler) != Acad::eOk) ||
	    !pFiler->atSubclassData(_T(/*NOXLATE*/"AsdkOPLineReactor")))
		return pFiler->filerStatus();

	// Read version number.
	pFiler->readItem(&rb);
	if (rb.restype != AcDb::kDxfInt16) {
		pFiler->pushBackItem();
		pFiler->setError(Acad::eInvalidDxfCode,
		                "nError: expected object version group code %d",
		                 AcDb::kDxfInt16);
		return pFiler->filerStatus();
	}
	Adesk::UInt16 version = rb.resval.rint;
	if (version > VERSION_ASDKOPLINEREACTOR)
		return Acad::eMakeMeProxy;

	// TODO: here you can file datamembers not
	//       created by the ObjectARX Add-In.
	switch (version)
	{
	case (1):
		pFiler->readItem (&rb) ;
		if ( rb.restype != AcDb::kDxfHardPointerId ) {
			pFiler->pushBackItem () ;
			pFiler->setError (Acad::eInvalidDxfCode, "\nError: expected offset style id code %d", AcDb::kDxfHardPointerId) ;
			return pFiler->filerStatus () ;
		}
		acdbGetObjectId (m_OPLineStyle, rb.resval.rlname) ;

		pFiler->readItem (&rb) ;
		if ( rb.restype != AcDb::kDxfInt32 ) {
			pFiler->pushBackItem () ;
			pFiler->setError (Acad::eInvalidDxfCode, "\nError: expected number of offsets code %d", AcDb::kDxfInt32) ;
			return pFiler->filerStatus () ;
		}
		int numOffsets =rb.resval.rint ;

		for ( int i =0 ; i < numOffsets ; i++ ) {
			pFiler->readItem (&rb) ;
			if ( rb.restype != AcDb::kDxfSoftPointerId ) {
				pFiler->pushBackItem () ;
				pFiler->setError (Acad::eInvalidDxfCode, "\nError: expected offset id code %d", AcDb::kDxfHardPointerId) ;
				return pFiler->filerStatus () ;
			}
			AcDbObjectId objId ;
			acdbGetObjectId (objId, rb.resval.rlname) ;
			m_entsToUpdate.append (objId) ;
		}
		break;
	}

	return pFiler->filerStatus();
}

Acad::ErrorStatus AsdkOPLineReactor::dxfOutFields(AcDbDxfFiler* pFiler) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	if ((es = AcDbObject::dxfOutFields(pFiler)) != Acad::eOk)
		return es;

	// Write subclass marker.
	pFiler->writeItem(AcDb::kDxfSubclass, _T(/*NOXLATE*/"AsdkOPLineReactor"));

	// Write version number.
	pFiler->writeItem(AcDb::kDxfInt16, (Adesk::UInt16) VERSION_ASDKOPLINEREACTOR);


	// TODO: here you can file datamembers not
	//       created by the ObjectARX Add-In.
	pFiler->writeItem (AcDb::kDxfHardPointerId, m_OPLineStyle) ;
	pFiler->writeItem (AcDb::kDxfInt32, m_entsToUpdate.length ()) ;
	for ( int i =0 ; i < m_entsToUpdate.length () ; i++ )
		pFiler->writeItem (AcDb::kDxfSoftPointerId, m_entsToUpdate.at (i)) ;

	return es;
}

//-----------------------------------------------------------------------------
Acad::ErrorStatus AsdkOPLineReactor::deepClone(AcDbObject* pOwnerObject,
                                               AcDbObject*& pClonedObject,
                                               AcDbIdMapping& idMap,
                                               Adesk::Boolean isPrimary) const
{
	assertReadEnabled();
	// TODO: implement this function.
	Acad::ErrorStatus es =AcDbObject::deepClone (pOwnerObject, pClonedObject, idMap, isPrimary) ;

	switch ( idMap.deepCloneContext () ) {
		case AcDb::kDcInsert:
		case AcDb::kDcInsertCopy:
			//----- We should force the copy of the style
			if ( !idMap.compute (AcDbIdPair (m_OPLineStyle, AcDbObjectId::kNull, true)) ) {
				//----- Open the orignal dictionary object we want to clone
				AsdkOPLineStyle *pObj, *pClone ;
				acdbOpenAcDbObject ((AcDbObject *&)pObj, m_OPLineStyle, AcDb::kForRead) ;
				//----- Open the dictionary owner of the original dictionary object
				AcDbDictionary *pDict ;
				acdbOpenAcDbObject ((AcDbObject *&)pDict, pObj->ownerId (), AcDb::kForRead) ;
				//----- Clone the dictionary owner if necessary, if not open it for write
				AcDbDatabase *pDestDb =NULL ;
				idMap.destDb (pDestDb) ;
				AcDbDictionary *pNOD, *pClonedDict ;
				pDestDb->getNamedObjectsDictionary (pNOD, AcDb::kForRead) ;
				if ( pNOD->has (ASDK_OPLINESTYLE) ) {
					//----- It is there
					pNOD->getAt (ASDK_OPLINESTYLE, (AcDbObject *&)pClonedDict, AcDb::kForWrite) ;
					//----- Clone the dictionary object now
					pObj->deepClone (pClonedDict, (AcDbObject *&)pClone, idMap, true) ;
					//----- Assign its owner ourself with an anonymous key
					AcDbObjectId idc ;
					pClonedDict->setAt ("*", pClone, idc) ;
					pClone->close () ;
				} else {
					//----- Clone it
					pNOD->upgradeOpen () ;
					pDict->deepClone (pNOD, (AcDbObject *&)pClonedDict, idMap, true) ;
					AcDbObjectId idcd ;
					pNOD->setAt (ASDK_OPLINESTYLE, pClonedDict, idcd) ;
					pNOD->downgradeOpen () ;
				}
				pNOD->close () ;
				pDict->close () ;
				//----- Close opened objects
				pObj->close () ;
				//----- We *must* close the clones ourself
				pClonedDict->close () ;
			}

			break ;
		default:
			break ;
	}

	return (es) ;
}

//-----------------------------------------------------------------------------
void AsdkOPLineReactor::MakeOPLine (const AcDbObject *dbObj) {
	bool b =AsdkOPLineReactor::m_bActive ;
	AsdkOPLineReactor::m_bActive =false ;

	AcDbPolyline *pLine =AcDbPolyline::cast (dbObj) ;
	if ( pLine == NULL ) {
		AsdkOPLineReactor::m_bActive =b ;
		return ;
	}
	//----- Iterate through all of the offsets defined in m_OPLineStyle
	AsdkOPLineStyle *pStyle ;
	if ( acdbOpenObject (pStyle, m_OPLineStyle, AcDb::kForRead, Adesk::kTrue) != Acad::eOk ) {
		AsdkOPLineReactor::m_bActive =b ;
		return ;
	}
	//----- Update (and create) pline(s) for the OPLine
	AcDbPolyline *pLine2 =NULL ;
	try {
		for ( int i =0 ; i < (int)pStyle->numOffsets () ; i++ ) {
			//----- Create offset
			AcDbPolyline *pCopy ;
			double off ;
			pStyle->offsetOf (i, off) ;
			if ( off == 0 )
				continue ;
			AcDbVoidPtrArray vpArr ;
			if (   pLine->getOffsetCurves (off, vpArr) != Acad::eOk
				|| vpArr.length () == 0
			)
				continue ;
			for ( int j =1 ; j < vpArr.length () ; j++ )
				delete ((AcDbObject *)vpArr.at (j)) ;
			vpArr.setLogicalLength (1) ;

			pCopy =AcDbPolyline::cast ((AcRxObject *)vpArr [0]) ;
			if ( pCopy == NULL ) {
				delete ((AcDbObject *)vpArr.at (0)) ;
				continue ;
			}
			//----- Get an offset pline object
			AcDbObjectId id =(i < m_entsToUpdate.length () ? m_entsToUpdate.at (i) : AcDbObjectId::kNull) ;
			if ( id == AcDbObjectId::kNull ) {
				pLine2 =new AcDbPolyline ;
				pLine2->setDatabaseDefaults (database ()) ;
				AcDbBlockTableRecord *pRec ;
				acdbOpenObject (pRec, dbObj->ownerId (), AcDb::kForWrite, Adesk::kTrue) ;
				pRec->appendAcDbEntity (id, pLine2) ;
				pRec->close () ;
				//----- Reactor assignement
				pLine2->addPersistentReactor (this->objectId ()) ;
				m_entsToUpdate.append (id) ;
			} else {
				if ( acdbOpenObject (pLine2, id, AcDb::kForWrite, Adesk::kTrue) != Acad::eOk ) {
					delete ((AcDbObject *)vpArr.at (0)) ;
					continue ;
				}
			}
			//----- Reset and properties/vertex assignments
			pLine2->reset (false, 0) ;
			double d, w0, w1 ;
			AcGePoint2d pt ;
			for ( unsigned int k =0 ; k < pCopy->numVerts () ; k++ ) {
				pCopy->getBulgeAt (k, d) ;
				pCopy->getWidthsAt (k, w0, w1) ;
				pCopy->getPointAt (k, pt) ;
				pLine2->addVertexAt (k, pt, d, w0, w1) ;
			}
			pLine2->setClosed (pCopy->isClosed ()) ;
			if ( pCopy->getConstantWidth (d) == Acad::eOk )
				pLine2->setConstantWidth (d) ;
			pLine2->setPlinegen (pCopy->hasPlinegen ()) ;
			delete pCopy ;
			//----- Set properties based on style
			Adesk::UInt16 color ;
			AcDbObjectId ltId ;
			AcDb::LineWeight lwt ;
			pStyle->colorOf (i, color) ;
			pStyle->lineWeightOf (i, lwt) ;
			pLine2->setColorIndex (color) ;
			pLine2->setLineWeight (lwt) ;

			ARX_REACTOR_CLOSE (this, pLine2) ;
		}
		//----- Erase unused pline(s)
		for ( i =m_entsToUpdate.length () - 1 ; i > (int)pStyle->numOffsets () - 1 ; i-- ) {
			if ( acdbOpenObject (pLine2, m_entsToUpdate.at (i), AcDb::kForWrite, Adesk::kTrue) != Acad::eOk )
				continue ;
			pLine2->removePersistentReactor (this->objectId ()) ;
			this->upgradeOpen () ;
			m_entsToUpdate.removeAt (i) ;
			if ( !pLine2->isErased () )
				pLine2->erase (true) ;

			ARX_REACTOR_CLOSE (this, pLine2) ;
		}
		m_entsToUpdate.setLogicalLength (pStyle->numOffsets ()) ;
	} catch (...) {
		if ( pLine2 != NULL ) {
			pLine2->close () ;
		} else {
			m_pLinesModified.append (this->objectId ()) ;
			m_pLinesModified.append (m_entsToUpdate.at (0)) ;
		}
	}
	pStyle->close () ;

	AsdkOPLineReactor::m_bActive =b ;
}

void AsdkOPLineReactor::ModifyOPLineFrom (AcDbObjectId mainId, const AcDbObject *dbObj) {
	bool b =AsdkOPLineReactor::m_bActive ;
	AsdkOPLineReactor::m_bActive =false ;

	AcDbPolyline *pLineMain ;
	if ( acdbOpenObject (pLineMain, mainId, AcDb::kForWrite, Adesk::kTrue) != Acad::eOk ) {
		//----- Main entity already opened! It means the main entity is modifying all
		//----- its offset'ed pline(s). To not fo into an infinite loop, just return...
		AsdkOPLineReactor::m_bActive =b ;
		return ;
	}

	int i ;
	AcDbPolyline *pLine =AcDbPolyline::cast (dbObj) ;
	if ( pLine == NULL || m_entsToUpdate.find (dbObj->objectId (), i, 0) == false ) {
		ARX_REACTOR_CLOSE (this, pLineMain) ;
		AsdkOPLineReactor::m_bActive =b ;
		return ;
	}

	AsdkOPLineStyle *pStyle ;
	if ( acdbOpenObject (pStyle, m_OPLineStyle, AcDb::kForRead, Adesk::kTrue) != Acad::eOk ) {
		ARX_REACTOR_CLOSE (this, pLineMain) ;
		AsdkOPLineReactor::m_bActive =b ;
		return ;
	}
	double off ;
	pStyle->offsetOf (i, off) ;
	pStyle->close () ;
	if ( off == 0 ) {
		ARX_REACTOR_CLOSE (this, pLineMain) ;
		AsdkOPLineReactor::m_bActive =b ;
		return ;
	}

	try {
		AcDbVoidPtrArray vpArr ;
		if (   pLine->getOffsetCurves (-off, vpArr) != Acad::eOk
			|| vpArr.length () == 0
		) {
			ARX_REACTOR_CLOSE (this, pLineMain) ;
			AsdkOPLineReactor::m_bActive =b ;
			return ;
		}
		for ( int j =1 ; j < vpArr.length () ; j++ )
			delete vpArr [j] ;
		vpArr.setLogicalLength (1) ;

		AcDbPolyline *pCopy =AcDbPolyline::cast ((AcRxObject *)vpArr [0]) ;
		if ( pCopy == NULL ) {
			delete vpArr [0] ;
			ARX_REACTOR_CLOSE (this, pLineMain) ;
			AsdkOPLineReactor::m_bActive =b ;
			return ;
		}

		pLineMain->reset (false, 0) ;

		double d, w0, w1 ;
		AcGePoint2d pt ;
		for ( i =0 ; i < (int)pCopy->numVerts () ; i++ ) {
			pCopy->getBulgeAt (i, d) ;
			pCopy->getWidthsAt (i, w0, w1) ;
			pCopy->getPointAt (i, pt) ;
			pLineMain->addVertexAt (i, pt, d, w0, w1) ;
		}
		pLineMain->setClosed (pCopy->isClosed ()) ;
		if ( pCopy->getConstantWidth (d) == Acad::eOk )
			pLineMain->setConstantWidth (d) ;
		pLineMain->setPlinegen (pCopy->hasPlinegen ()) ;
		delete pCopy ;
	} catch (...) {
		m_pLinesModified.append (this->objectId ()) ;
		m_pLinesModified.append (mainId) ;
	}

	ARX_REACTOR_CLOSE (this, pLineMain) ;

	AsdkOPLineReactor::m_bActive =b ;
}

void AsdkOPLineReactor::modify(const AcDbObject* dbObj) {
	//----- First: Is it the main entity being modified or not?
	AcDbDictionary *pExtDct ;
	if ( acdbOpenObject (pExtDct, ownerId (), AcDb::kForRead, Adesk::kTrue) != Acad::eOk )
		return ;
	AcDbObjectId mainEntId ;
	mainEntId =pExtDct->ownerId () ;
	pExtDct->close () ;

	if ( mainEntId == dbObj->objectId () ) {
		//----- Yes it is! So cool...
		MakeOPLine (dbObj) ;
	} else {
		//----- No it isn't, the notification is coming from one of the counter-partner
		ModifyOPLineFrom (mainEntId, dbObj) ;
		AcDbObject *pObj ;
		if ( acdbOpenAcDbObject (pObj, mainEntId, AcDb::kForNotify, Adesk::kTrue) == Acad::eOk ) {
			modify (pObj) ;
			pObj->close () ;
		}
	}
}

//-----------------------------------------------------------------------------
void AsdkOPLineReactor::modified(const AcDbObject* dbObj)
{
	assertNotifyEnabled();
	// TODO: implement this function.

	//----- We cannot do the following... We *must* wait that the command is ending...
#ifdef MAKE_IT_THERE
	if ( AsdkOPLineReactor::m_bActive == true ) {
		//----- First: Is it the main entity being modified or not?
		AcDbDictionary *pExtDct ;
		if ( acdbOpenObject (pExtDct, ownerId (), AcDb::kForRead, Adesk::kTrue) != Acad::eOk )
			return ;
		AcDbObjectId mainEntId ;
		mainEntId =pExtDct->ownerId () ;
		pExtDct->close () ;

		if ( mainEntId == dbObj->objectId () ) {
			//----- Yes it is! So cool...
			MakeOPLine (dbObj) ;
		} else {
			//----- No it isn't, the notification is coming from one of the counter-partner
			ModifyOPLineFrom (mainEntId, dbObj) ;
		}
	}
#endif
	//----- Instead we just store the Id of modified entities, and wait for commandEnded()
	//----- NB: We could do the same for erased, but for erased it is working that way too...
	if ( AsdkOPLineReactor::m_bActive == true ) {
		if ( !m_pLinesModified.contains (this->objectId ()) ) {
			//----- Always by pair...
			m_pLinesModified.append (this->objectId ()) ;
			m_pLinesModified.append (dbObj->objectId ()) ;
		}
	}

	AcDbObject::modified(dbObj);
}

void AsdkOPLineReactor::erased(const AcDbObject* dbObj,
                               Adesk::Boolean pErasing)
{
	assertNotifyEnabled();

	// TODO: implement this function.
	if ( AsdkOPLineReactor::m_bActive == true ) {
		//----- First: Is it the main entity being erased or not?
		AcDbDictionary *pExtDct ;
		if ( acdbOpenObject (pExtDct, ownerId (), AcDb::kForRead) != Acad::eOk )
			return ;
		AcDbObjectId mainEntId ;
		mainEntId =pExtDct->ownerId () ;
		pExtDct->close () ;

		AcDbObject *p ;
		if ( mainEntId != dbObj->objectId () ) {
			if ( acdbOpenAcDbObject (p, mainEntId, AcDb::kForWrite) == Acad::eOk ) {
				p->erase (pErasing) ;
				ARX_REACTOR_CLOSE (this, p) ;
			}
		}

		for ( int i =0 ; i < m_entsToUpdate.length () ; i++ ) {
			if ( acdbOpenAcDbObject (p, m_entsToUpdate.at (i), AcDb::kForWrite) != Acad::eOk)
				continue ;
			p->erase (pErasing) ;
			ARX_REACTOR_CLOSE (this, p) ;
		}
	}

	AcDbObject::erased(dbObj, pErasing);
}

//-----------------------------------------------------------------------------
Acad::ErrorStatus AsdkOPLineReactor::oPLineStyle(AcDbHardPointerId& OPLineStyle)
{
	assertReadEnabled();
	OPLineStyle = m_OPLineStyle;
	return Acad::eOk;
}

Acad::ErrorStatus AsdkOPLineReactor::setOPLineStyle(const AcDbHardPointerId OPLineStyle)
{
	assertWriteEnabled();
	m_OPLineStyle = OPLineStyle;
	return Acad::eOk;
}

//-----------------------------------------------------------------------------
CString AsdkOPLineReactor::printIt () {
	AcDbDictionary *pExtDct ;
	if ( acdbOpenObject (pExtDct, ownerId (), AcDb::kForRead) != Acad::eOk )
		return (_T("")) ;
	AcDbObjectId mainEntId ;
	mainEntId =pExtDct->ownerId () ;
	pExtDct->close () ;

	CString st ;
	st.Format (
		_T("\n"
		   "Reactor: %ld\n"
		   "	Style: %ld\n"
		   "	Main entity link: %ld\n"
		   "	Entity(ies) to update: "
		),
		objectId ().asOldId (),
		m_OPLineStyle.asOldId (),
		mainEntId.asOldId ()
	) ;
	CString temp ;
	for ( int i =0 ; i < m_entsToUpdate.length () ; i++ ) {
		temp.Format (_T(" %ld"), m_entsToUpdate.at (i).asOldId ()) ;
		st +=temp ;
	}
	st +=_T("\n") ;

	return (st) ;
}