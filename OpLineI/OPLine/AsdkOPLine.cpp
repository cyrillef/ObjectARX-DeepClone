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

#include "StdAfx.h"
#include "StdArx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"

#include "AsdkOPLineStyle.h"

#define VERSION_ASDKOPLINE 1

//{{AFX_ARX_MACRO
/*ACRX_DXF_DEFINE_MEMBERS(AsdkOPLine, AcDbCurve,
                        AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyEntity::kNoOperation,
                        ASDKOPLINE, AsdkOPLine
);*/
//}}AFX_ARX_MACRO

//-------------------
bool bPEdit =false ;

void set_bPEdit (bool flag) {
	bPEdit =flag ;
}

//------------------------------------------
#define MY_ACRX_DEFINE_MEMBERS(CLASS_NAME) \
AcRxClass* CLASS_NAME::desc() \
{ \
    return (AcRxClass*)((AcRxDictionary*)acrxSysRegistry() \
		->at(ACRX_CLASS_DICTIONARY))->at(bPEdit ? "AcDbPolyline" : "AsdkOPLine"); \
} \
AcRxClass* CLASS_NAME::isA() const \
{ \
    return (AcRxClass*)((AcRxDictionary*)acrxSysRegistry() \
		->at(ACRX_CLASS_DICTIONARY))->at(bPEdit ? "AcDbPolyline" : "AsdkOPLine"); \
} \
AcRxClass* CLASS_NAME::gpDesc = NULL

//----------------------------------------------------------------------
#define MY_ACRX_DXF_DEFINE_MEMBERS(CLASS_NAME,PARENT_CLASS,DWG_VERSION,\
              MAINTENANCE_VERSION,PROXY_FLAGS,DXF_NAME,APP) \
MY_ACRX_DEFINE_MEMBERS(CLASS_NAME); \
static AcRxObject * make##CLASS_NAME() { return new CLASS_NAME(); } \
void CLASS_NAME::rxInit() { \
    if (CLASS_NAME::gpDesc != NULL) \
        return; \
    CLASS_NAME::gpDesc = newAcRxClass(#CLASS_NAME, #PARENT_CLASS, \
        DWG_VERSION,MAINTENANCE_VERSION,PROXY_FLAGS, \
        &make##CLASS_NAME, #DXF_NAME, #APP); \
}

//-----------------------------------------------------------------
MY_ACRX_DXF_DEFINE_MEMBERS(AsdkOPLine, AcDbCurve,
                        AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyEntity::kNoOperation,
                        ASDKOPLINE, AsdkOPLine
);

// Constructor / Destructor
AsdkOPLine::AsdkOPLine(AcDbObjectId id)
	: AcDbPolyline (), m_OPLineStyle(id), mbDrawWithExplode(false)
{
	// TODO: do your initialization.

}

AsdkOPLine::AsdkOPLine(unsigned int num_verts /*=0*/, AcDbObjectId id /*=AcDbObjectId::kNull*/)
	: AcDbPolyline (num_verts), m_OPLineStyle(id), mbDrawWithExplode(false)
{
}

AsdkOPLine::~AsdkOPLine()
{
	// TODO: clean up.

}

Adesk::Boolean AsdkOPLine::worldDraw(AcGiWorldDraw* mode)
{
	assertReadEnabled();
	// TODO: implement this function.
	//----- To workaround AcDbPolyline::saveAs() bug
	if ( !mbDrawWithExplode ) {
		bPEdit =true ;
		if ( AcDbPolyline::worldDraw (mode) != Adesk::kTrue ) {
			bPEdit =false ;
			return (Adesk::kFalse) ;
		}
		bPEdit =false ;
	} else {
		AcDbVoidPtrArray ents ;
		AcDbPolyline::explode (ents) ;
		for ( int j =ents.length () - 1 ; j >= 0 ; j-- ) {
			((AcDbEntity *)ents.at (j))->worldDraw (mode) ;
			delete (AcDbEntity *)ents.at (j) ;
		}
		ents.setLogicalLength (0) ;
	}

	char *pltName =plotStyleName () ;
	AcDbDictionary *pNOD ;
	acdbHostApplicationServices ()->workingDatabase ()->getNamedObjectsDictionary (pNOD, AcDb::kForRead) ;
	AcDbDictionary *pPltStyleName ;
	pNOD->getAt (_T(/*NOXLATE*/"ACAD_PLOTSTYLENAME"), (AcDbObject *&)pPltStyleName, AcDb::kForRead) ;
	pNOD->close () ;
	AcDbObjectId pltStyleNameId ;
	pPltStyleName->getAt (pltName, pltStyleNameId) ;
	pPltStyleName->close () ;
	mode->subEntityTraits ().setPlotStyleName (AcDb::kPlotStyleNameById, pltStyleNameId) ;

	AsdkOPLineStyle *pStyle ;
	if ( m_OPLineStyle != AcDbObjectId::kNull ) {
		ARXOK ( acdbOpenObject (pStyle, m_OPLineStyle, AcDb::kForRead) ) ;

		for ( Adesk::UInt32 i =0 ; i < pStyle->numOffsets () ; i++ ) {
			//----- Create offset
			AcDbVoidPtrArray vpArr ;
			double off ;
			pStyle->offsetOf (i, off) ;
			if ( off == 0 )
				continue ;
			if ( AcDbPolyline::getOffsetCurves (off, vpArr) != Acad::eOk )
				continue ;

			for ( int j =0 ; j < vpArr.length () ; j++ ) {
				AcDbEntity *pCopy =(AcDbEntity *)vpArr [j] ;
				//----- Entity
				pCopy->setLayer (layerId ()) ;
				pCopy->setLinetypeScale (linetypeScale ()) ;
				pCopy->setVisibility (visibility ()) ;
				//----- Set properties based on style
				Adesk::UInt16 color ;
				AcDbObjectId ltId ;
				AcDb::LineWeight lwt ;
				pStyle->colorOf (i, color) ;
				pStyle->linetypeOf (i, ltId) ;
				pStyle->lineWeightOf (i, lwt) ;
				pCopy->setColorIndex (color) ;
				pCopy->setLinetype (ltId) ;
				pCopy->setLineWeight (lwt) ;
				//----- Polyline
				if ( pCopy->isKindOf (AcDbPolyline::desc ()) )
					((AcDbPolyline *)pCopy)->setElevation (elevation ()) ;

				pCopy->setPlotStyleName (pltName) ;

				mode->subEntityTraits ().setColor (color) ;
				mode->subEntityTraits ().setThickness (thickness ()) ;
				mode->subEntityTraits ().setLayer (layerId ()) ;
				mode->subEntityTraits ().setLineType (ltId) ;
				mode->subEntityTraits ().setLineTypeScale (linetypeScale ()) ;
				mode->subEntityTraits ().setLineWeight (lwt) ;
				mode->subEntityTraits ().setPlotStyleName (AcDb::kPlotStyleNameById, pltStyleNameId) ;

				//----- To workaround AcDbPolyline::saveAs() bug
				if ( !mbDrawWithExplode ) {
					pCopy->worldDraw (mode) ;
				} else {
					AcDbVoidPtrArray ents ;
					pCopy->explode (ents) ;
					for ( int j =ents.length () - 1 ; j >= 0 ; j-- ) {
						((AcDbEntity *)ents.at (j))->worldDraw (mode) ;
						delete (AcDbEntity *)ents.at (j) ;
					}
					ents.setLogicalLength (0) ;
				}

				delete pCopy ;
			}
		}
		pStyle->close () ;
	}

	delete [] pltName ;

	return Adesk::kTrue ;
}

Acad::ErrorStatus AsdkOPLine::dwgInFields(AcDbDwgFiler* filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	// Call dwgInFields from AcDbPolyline
	if ((es = AcDbPolyline::dwgInFields(filer)) != Acad::eOk) {
		return es;
	}

	// Read version number.
	Adesk::UInt16 version;
	filer->readItem(&version);
	if (version > VERSION_ASDKOPLINE)
		return Acad::eMakeMeProxy;

	// Read the data members.
	switch (version)
	{
	case (1):
		filer->readItem(&m_OPLineStyle);
		// TODO: here you can file datamembers not
		//       created by the ObjectARX Add-In.

		break;
	}


	return filer->filerStatus();
}

Acad::ErrorStatus AsdkOPLine::dwgOutFields(AcDbDwgFiler* filer) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbPolyline
	if ((es = AcDbPolyline::dwgOutFields(filer)) != Acad::eOk) {
		return es;
	}

	// Write version number.
	filer->writeItem((Adesk::UInt16) VERSION_ASDKOPLINE);

	// Write the data members.
	filer->writeItem(m_OPLineStyle);
	// TODO: here you can file datamembers not
	//       created by the ObjectARX Add-In.


	return filer->filerStatus();
}

Acad::ErrorStatus AsdkOPLine::dxfInFields(AcDbDxfFiler* filer)
{
	assertWriteEnabled();
	struct resbuf rb;

	if ((AcDbPolyline::dxfInFields(filer) != Acad::eOk) ||
	    !filer->atSubclassData(_T(/*NOXLATE*/"AsdkOPLine")))
		return filer->filerStatus();

	// Read version number.
	filer->readItem(&rb);
	if (rb.restype != AcDb::kDxfInt16) {
		filer->pushBackItem();
		filer->setError(Acad::eInvalidDxfCode,
		                "nError: expected object version group code %d",
		                 AcDb::kDxfInt16);
		return filer->filerStatus();
	} else {
		Adesk::UInt16 version = rb.resval.rint;
		if (version > VERSION_ASDKOPLINE)
			return Acad::eMakeMeProxy;
	}

	filer->readItem(&rb);
	if (rb.restype == AcDb::kDxfHardPointerId) {
		m_OPLineStyle.setFromOldId(rb.resval.rlong);
	} else {
		filer->pushBackItem();
		filer->setError(Acad::eInvalidDxfCode,
		                "nError: expected group code %d",
		                AcDb::kDxfHardPointerId);
		return filer->filerStatus();
	}

	// TODO: here you can file datamembers not
	//       created by the ObjectARX Add-In.

	return filer->filerStatus();
}

Acad::ErrorStatus AsdkOPLine::dxfOutFields(AcDbDxfFiler* filer) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	if ((es = AcDbPolyline::dxfOutFields(filer)) != Acad::eOk)
		return es;

	// Write subclass marker.
	filer->writeItem(AcDb::kDxfSubclass, _T(/*NOXLATE*/"AsdkOPLine"));

	// Write version number.
	filer->writeItem(AcDb::kDxfInt16, (Adesk::UInt16) VERSION_ASDKOPLINE);

	filer->writeItem(AcDb::kDxfHardPointerId, m_OPLineStyle);

	// TODO: here you can file datamembers not
	//       created by the ObjectARX Add-In.

	return es;
}

Acad::ErrorStatus AsdkOPLine::oPLineStyle(AcDbHardPointerId& OPLineStyle)
{
	assertReadEnabled();
	OPLineStyle = m_OPLineStyle;
	return Acad::eOk;
}

Acad::ErrorStatus AsdkOPLine::setOPLineStyle(const AcDbHardPointerId OPLineStyle)
{
	assertWriteEnabled();
	m_OPLineStyle = OPLineStyle;
	return Acad::eOk;
}

Acad::ErrorStatus AsdkOPLine::makeFrom (AcDbObjectId id) {
	AcDbPolyline *pLine ;
	ARXOK ( acdbOpenObject (pLine, id, AcDb::kForRead) ) ;
	ARXOK2 ( makeFrom (pLine) ) ;
	return (Acad::eOk) ;
}


Acad::ErrorStatus AsdkOPLine::makeFrom (AcDbPolyline *pLine) {
	//----- Entity
	setLayer (pLine->layerId ()) ;
	setLinetype (pLine->linetypeId ()) ;
	setLinetypeScale (pLine->linetypeScale ()) ;
	setLineWeight (pLine->lineWeight ()) ;
	setColorIndex (pLine->colorIndex ()) ;
	setVisibility (pLine->visibility ()) ;
	//----- Pline
	double d, w0, w1 ;
	AcGePoint2d pt ;
	for ( unsigned int i =0 ; i < pLine->numVerts () ; i++ ) {
		pLine->getBulgeAt (i, d) ;
		pLine->getWidthsAt (i, w0, w1) ;
		pLine->getPointAt (i, pt) ;
		addVertexAt (i, pt, d, w0, w1) ;
	}
	setClosed (pLine->isClosed ()) ;
	if ( pLine->getConstantWidth (d) == Acad::eOk )
		setConstantWidth (d) ;
	setElevation (pLine->elevation ()) ;
	setNormal (pLine->normal ()) ;
	setPlinegen (pLine->hasPlinegen ()) ;
	setThickness (pLine->thickness ()) ;

	if ( !pLine->isWriteEnabled () )
		ARXOK ( pLine->upgradeOpen () ) ;
	ARXOK2 ( pLine->handOverTo (this, true, true) ) ;
	delete pLine ;
	return (Acad::eOk) ;
}

void AsdkOPLine::saveAs(AcGiWorldDraw* mode, AcDb::SaveType st) {
	assertReadEnabled();
	// TODO: implement this function.
	if ( st == AcDb::kR12Save ) {
		AcDbPolyline::saveAs (mode, st) ;
	} else {
		mbDrawWithExplode =true ;
		worldDraw (mode) ;
		mbDrawWithExplode =false ;
	}
}

Acad::ErrorStatus AsdkOPLine::explode(AcDbVoidPtrArray& entitySet) const
{
	assertReadEnabled();
	// TODO: implement this function.
	AcDbPolyline *pCopy =new AcDbPolyline ;
	//----- Entity
	pCopy->setLayer (layerId ()) ;
	pCopy->setLinetype (linetypeId ()) ;
	pCopy->setLinetypeScale (linetypeScale ()) ;
	pCopy->setLineWeight (lineWeight ()) ;
	pCopy->setColorIndex (colorIndex ()) ;
	pCopy->setVisibility (visibility ()) ;
	//----- Pline
	double d, w0, w1 ;
	AcGePoint2d pt ;
	for ( unsigned int i =0 ; i < numVerts () ; i++ ) {
		getBulgeAt (i, d) ;
		getWidthsAt (i, w0, w1) ;
		getPointAt (i, pt) ;
		pCopy->addVertexAt (i, pt, d, w0, w1) ;
	}
	pCopy->setClosed (isClosed ()) ;
	if ( getConstantWidth (d) == Acad::eOk )
		pCopy->setConstantWidth (d) ;
	pCopy->setElevation (elevation ()) ;
	pCopy->setNormal (normal ()) ;
	pCopy->setPlinegen (hasPlinegen ()) ;
	pCopy->setThickness (thickness ()) ;

	char *pltName =plotStyleName () ;
	pCopy->setPlotStyleName (pltName, true) ;

	entitySet.append (pCopy) ;

	AsdkOPLineStyle *pStyle ;
	if ( m_OPLineStyle != AcDbObjectId::kNull ) {
		ARXOK ( acdbOpenObject (pStyle, m_OPLineStyle, AcDb::kForRead) ) ;

		for ( Adesk::UInt32 i =0 ; i < pStyle->numOffsets () ; i++ ) {
			//----- Create offset
			AcDbVoidPtrArray vpArr ;
			double off ;
			pStyle->offsetOf (i, off) ;
			if ( off == 0 )
				continue ;
			if ( AcDbPolyline::getOffsetCurves (off, vpArr) != Acad::eOk )
				continue ;

			for ( int j =0 ; j < vpArr.length () ; j++ ) {
				AcDbEntity *pCopy2 =AcDbEntity::cast ((AcRxObject *)vpArr [0]) ;
				//----- Entity
				pCopy2->setLayer (layerId ()) ;
				pCopy2->setLinetypeScale (linetypeScale ()) ;
				pCopy2->setVisibility (visibility ()) ;
				//----- Set properties based on style
				Adesk::UInt16 color ;
				AcDbObjectId ltId ;
				AcDb::LineWeight lwt ;
				pStyle->colorOf (i, color) ;
				pStyle->linetypeOf (i, ltId) ;
				pStyle->lineWeightOf (i, lwt) ;
				pCopy2->setColorIndex (color) ;
				pCopy2->setLinetype (ltId) ;
				pCopy2->setLineWeight (lwt) ;
				//----- Polyline
				if ( pCopy2->isKindOf (AcDbPolyline::desc ()) )
					((AcDbPolyline *)pCopy2)->setElevation (elevation ()) ;

				pCopy2->setPlotStyleName (pltName, true) ;

				entitySet.append (pCopy2) ;
			}
		}
		pStyle->close () ;
	}

	delete [] pltName ;

	return (/*AcDbPolyline::explode(entitySet)*/Acad::eOk) ;
}

Acad::ErrorStatus AsdkOPLine::deepClone(AcDbObject* pOwnerObject,
                                        AcDbObject*& pClonedObject,
                                        AcDbIdMapping& idMap,
                                        Adesk::Boolean isPrimary) const
{
	assertReadEnabled();
	// TODO: implement this function.
	Acad::ErrorStatus es =AcDbObject::deepClone (pOwnerObject, pClonedObject, idMap, isPrimary) ;

	if (   idMap.deepCloneContext () == AcDb::kDcInsert
		|| idMap.deepCloneContext () == AcDb::kDcInsertCopy
	) {
		//----- We should force the copy of the style
		if ( !idMap.compute (AcDbIdPair (m_OPLineStyle, AcDbObjectId::kNull, true)) ) {
			//----- Open the orignal dictionary object we want to clone
			AsdkOPLineStyle *pObj ;
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
				AsdkOPLineStyle *pClone =NULL ;
				pObj->deepClone (pClonedDict, (AcDbObject *&)pClone, idMap, isPrimary) ;
				//----- Assign its owner ourself with an anonymous key
				AcDbObjectId idc ;
				pClonedDict->setAt (_T("*"), pClone, idc) ;
				pClone->close () ;
			} else {
				//----- Clone it
				pNOD->upgradeOpen () ;
				pDict->deepClone (pNOD, (AcDbObject *&)pClonedDict, idMap, isPrimary) ;
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
	}
	return (es) ;
}

