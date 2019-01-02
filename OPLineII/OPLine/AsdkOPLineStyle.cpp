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

#include "StdAfx.h"
#include "StdArx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"

#define VERSION_ASDKOPLINESTYLE 1

char kMchr =_T('$') ; //----- MDI safe

//----- Does the unmangling specific to AcDb::kDrcUnmangleName, which
//----- removes the $0$ from the beginning of the name. Any other
//----- mangling, or usage of $ in a name is ignored. If the name
//----- does not start with $0$, it is not changed. Returns true
//----- if the name was unmangled.
bool wblockObjectsUnmangleName (CString &csName) {
	int namePos ;
	int nameLen ;

	if ( csName [0] == kMchr ) {
		nameLen =csName.GetLength () ;
		bool bTruncate =false ;
		bool bFoundNumber =false ;
		for ( namePos =1 ; namePos < nameLen ; namePos++ ) {
			if (   csName [namePos] >= _T('0')
				&& csName [namePos] <= _T('9')
			) {
				bFoundNumber =true ;
			} else {
				if ( bFoundNumber && csName [namePos] == kMchr )
					bTruncate =true ;
				break ;
			}
		}

		//----- Only do it if there is a name to the right 
		//----- of the $0$.
		if ( bTruncate && (++namePos < nameLen) ) {
			csName =csName.Right (nameLen - namePos) ;
			return (true) ;
		}
	}
	return (false) ;
}

typedef Acad::ErrorStatus (*fct_acedGetCurDwgXrefGraph)(AcDbXrefGraph&, Adesk::Boolean includeGhosts =Adesk::kFalse) ;

const char *xrefName (const AcDbDatabase *pDb) {
	AcDbXrefGraph graph ;
	//acedGetCurDwgXrefGraph (graph) ;
	fct_acedGetCurDwgXrefGraph fct ;
	fct =(fct_acedGetCurDwgXrefGraph)::GetProcAddress (
		::GetModuleHandle (_T(/*NOXLATE*/"acad.exe")),
		_T(/*NOXLATE*/"?acedGetCurDwgXrefGraph@@YA?AW4ErrorStatus@Acad@@AAVAcDbXrefGraph@@H@Z")
	) ;
	if ( fct == NULL )
		return (_T("")) ;
	(*fct) (graph) ;
	AcDbXrefGraphNode *pNode =graph.xrefNode (pDb) ;
	if ( pNode == NULL ) {
		ASSERT ( pNode != NULL ) ;
		return (NULL) ;
	}
	return (pNode->name ()) ;
}


//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AsdkOPLineStyle, AcDbObject,
                        AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
                        ASDKOPLINESTYLE, AsdkOPLine
);
//}}AFX_ARX_MACRO


// Constructor / Destructor

AsdkOPLineStyle::AsdkOPLineStyle()
{
	// TODO: do your initialization.
	m_iNumOffsets =0 ;
	m_offsetArray.setLogicalLength (0) ;
	m_ltypeArray.setLogicalLength (0) ;
	m_colorArray.setLogicalLength (0) ;
	m_lweightArray.setLogicalLength (0) ;

}

AsdkOPLineStyle::~AsdkOPLineStyle()
{
	// TODO: clean up.

}


Acad::ErrorStatus AsdkOPLineStyle::dwgInFields(AcDbDwgFiler* pFiler)
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
	if (version > VERSION_ASDKOPLINESTYLE)
		return Acad::eMakeMeProxy;

	// Read the data members.
	switch (version)
	{
	case (1):
		// TODO: here you can file datamembers not
		//       created by the ObjectARX Add-In.
		pFiler->readUInt32 (&m_iNumOffsets) ;
		AcDbHardPointerId eId ;
		double offset ;
		Adesk::UInt16 color ;
		short weight ;
		for ( Adesk::UInt32 i =0 ; i < m_iNumOffsets ; i++ ) {
			pFiler->readDouble (&offset) ;
			m_offsetArray.append (offset) ;
			pFiler->readHardPointerId (&eId) ;
			m_ltypeArray.append (eId) ;
			pFiler->readUInt16 (&color) ;
			m_colorArray.append (color) ;
			pFiler->readInt16 (&weight) ;
			m_lweightArray.append ((AcDb::LineWeight)(weight)) ;
		}

		break;
	}


	return pFiler->filerStatus();
}

Acad::ErrorStatus AsdkOPLineStyle::dwgOutFields(AcDbDwgFiler* pFiler) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbObject
	if ((es = AcDbObject::dwgOutFields(pFiler)) != Acad::eOk) {
		return es;
	}

	// Write version number.
	pFiler->writeItem((Adesk::UInt16) VERSION_ASDKOPLINESTYLE);

	// Write the data members.
	// TODO: here you can file datamembers not
	//       created by the ObjectARX Add-In.
	pFiler->writeUInt32 (m_iNumOffsets) ;
	for ( Adesk::UInt32 i =0 ; i < m_iNumOffsets ; i++ ) {
		pFiler->writeDouble (m_offsetArray [i]) ;
		pFiler->writeHardPointerId(m_ltypeArray [i]) ;
		pFiler->writeUInt16 (m_colorArray [i]) ;
		pFiler->writeInt16 ((short)m_lweightArray [i]) ;
	}

	return pFiler->filerStatus();
}

Acad::ErrorStatus AsdkOPLineStyle::dxfInFields(AcDbDxfFiler* pFiler)
{
	assertWriteEnabled();
	struct resbuf rb;

	if ((AcDbObject::dxfInFields(pFiler) != Acad::eOk) ||
	    !pFiler->atSubclassData(_T(/*NOXLATE*/"AsdkOPLineStyle")))
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
	if (version > VERSION_ASDKOPLINESTYLE)
		return Acad::eMakeMeProxy;

	// TODO: here you can file datamembers not
	//       created by the ObjectARX Add-In.
	Acad::ErrorStatus es;
	switch (version)
	{
	case (1):
		es = pFiler->readItem(&rb);
		if (rb.restype != AcDb::kDxfInt32)
		{
			pFiler->pushBackItem();
			pFiler->setError(Acad::eInvalidDxfCode,
							"\nError: expected offset count group code %d",
							AcDb::kDxfInt32);
			return pFiler->filerStatus();
		}
		m_iNumOffsets = rb.resval.rint;

		for (Adesk::UInt32 i=0; i < m_iNumOffsets; i++)
		{
			es = pFiler->readItem(&rb);
			if (rb.restype != AcDb::kDxfReal)
			{
				pFiler->pushBackItem();
				pFiler->setError(Acad::eInvalidDxfCode,
								"\nError: expected offset distance group code %d",
								AcDb::kDxfReal);
				return pFiler->filerStatus();
			}
			m_offsetArray.append(rb.resval.rreal);

			es = pFiler->readItem(&rb);
			if (rb.restype != AcDb::kDxfHardPointerId)
			{
				pFiler->pushBackItem();
				pFiler->setError(Acad::eInvalidDxfCode,
								"\nError: expected linetype object id group code %d", 
								AcDb::kDxfHardPointerId);
				return pFiler->filerStatus();
			}
			AcDbObjectId objId;
			acdbGetObjectId(objId, rb.resval.rlname);
			m_ltypeArray.append(objId);

			es = pFiler->readItem(&rb);
			if (rb.restype != AcDb::kDxfInt16)
			{
				pFiler->pushBackItem();
				pFiler->setError(Acad::eInvalidDxfCode,
								"\nError: expected offset color group code %d",
								AcDb::kDxfInt16);
				return pFiler->filerStatus();
			}
			m_colorArray.append(rb.resval.rint);		

			es = pFiler->readItem(&rb);
			if (rb.restype != AcDb::kDxfInt16)
			{
				pFiler->pushBackItem();
				pFiler->setError(Acad::eInvalidDxfCode,
								"\nError: expected offset lineweight group code %d",
								AcDb::kDxfInt16);
				return pFiler->filerStatus();
			}
			m_lweightArray.append((AcDb::LineWeight)(rb.resval.rint));
		}
		break;
	}

	return pFiler->filerStatus();
}

Acad::ErrorStatus AsdkOPLineStyle::dxfOutFields(AcDbDxfFiler* pFiler) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	if ((es = AcDbObject::dxfOutFields(pFiler)) != Acad::eOk)
		return es;

	// Write subclass marker.
	pFiler->writeItem(AcDb::kDxfSubclass, _T(/*NOXLATE*/"AsdkOPLineStyle"));

	// Write version number.
	pFiler->writeItem(AcDb::kDxfInt16, (Adesk::UInt16) VERSION_ASDKOPLINESTYLE);


	// TODO: here you can file datamembers not
	//       created by the ObjectARX Add-In.
	pFiler->writeItem (AcDb::kDxfInt32, m_iNumOffsets);
	for ( Adesk::UInt32 i =0 ; i < m_iNumOffsets ; i++ ) {
		pFiler->writeItem (AcDb::kDxfReal, m_offsetArray [i]) ;
		pFiler->writeItem (AcDb::kDxfHardPointerId, m_ltypeArray [i]) ;
		pFiler->writeItem (AcDb::kDxfInt16, m_colorArray [i]) ;
		pFiler->writeItem (AcDb::kDxfInt16, (short)m_lweightArray [i]) ;
	}

	return es;
}

Acad::ErrorStatus AsdkOPLineStyle::setName (const char *name) {
	//----- This only sets the name object's name if it's database resident
	AcDbObjectId eId =objectId () ;
	if ( eId == AcDbObjectId::kNull )
		return (Acad::eNoDatabase) ;
	AcDbDictionary *pDict ;
	Acad::ErrorStatus es =acdbOpenAcDbObject ((AcDbObject *&)pDict, ownerId (), AcDb::kForWrite) ;
	if ( es != Acad::eOk )
		return (es);
	char *oldName ;
	es =pDict->nameAt (eId, oldName) ;
	if ( es != Acad::eOk ) {
		pDict->close () ;
		return (es) ;
	}
	bool test =pDict->setName (oldName, name) ;
	acad_free (oldName) ;
	pDict->close () ;
	return (test ? Acad::eOk : Acad::eInvalidInput) ;
}

char *AsdkOPLineStyle::getName () {
	//----- This only returns the object's name if it's database resident
	AcDbObjectId eId =objectId () ;
	if ( eId == AcDbObjectId::kNull )
		return (NULL) ;
	AcDbDictionary *pDict ;
	Acad::ErrorStatus es =acdbOpenAcDbObject ((AcDbObject *&)pDict, ownerId (), AcDb::kForRead) ;
	if ( es != Acad::eOk )
		return (NULL) ;
	char *name =NULL ;
	pDict->nameAt (eId, name) ;
	pDict->close () ;
	return (name) ;
}

Acad::ErrorStatus AsdkOPLineStyle::setOffsetOf (Adesk::UInt32 index, double off) {
	if ( (index >= m_iNumOffsets) || (index < 0) )
		return (Acad::eInvalidIndex) ;
	m_offsetArray [index] =off ;
	return (Acad::eOk) ;
}

Acad::ErrorStatus AsdkOPLineStyle::offsetOf (Adesk::UInt32 index, double &off) {
	if ( (index >= m_iNumOffsets) || (index < 0) )
		return (Acad::eInvalidIndex) ;
	off =m_offsetArray [index] ;
	return (Acad::eOk) ;
}

Acad::ErrorStatus AsdkOPLineStyle::setLinetypeOf (Adesk::UInt32 index, AcDbObjectId eId) {
	if ( (index >= m_iNumOffsets) || (index < 0) )
		return (Acad::eInvalidIndex) ;
	if ( eId == AcDbObjectId::kNull )
		return (Acad::eNullObjectId) ;
	m_ltypeArray [index] =eId ;
	return (Acad::eOk) ;
}

Acad::ErrorStatus AsdkOPLineStyle::linetypeOf (Adesk::UInt32 index, AcDbObjectId &eId) {
	if ( index >= m_iNumOffsets )
		return (Acad::eInvalidIndex) ;
	eId =m_ltypeArray [index] ;
	return (Acad::eOk) ;
}

Acad::ErrorStatus AsdkOPLineStyle::setColorOf (Adesk::UInt32 index, Adesk::UInt16 color) {
	if ( index >= m_iNumOffsets )
		return (Acad::eInvalidIndex) ;
	if ( (color > 256) || (color < 0) )
		return (Acad::eInvalidInput) ;
	m_colorArray [index] =color ;
	return (Acad::eOk) ;
}

Acad::ErrorStatus AsdkOPLineStyle::colorOf (Adesk::UInt32 index, Adesk::UInt16 &color) {
	if ( index >= m_iNumOffsets )
		return (Acad::eInvalidIndex) ;
	color =m_colorArray [index] ;
	return (Acad::eOk) ;
}

Acad::ErrorStatus AsdkOPLineStyle::setLineWeightOf (Adesk::UInt32 index, AcDb::LineWeight lw) {
	AcDbDatabase *pDb =database () ;
	if ( pDb == NULL )
		pDb =acdbHostApplicationServices ()->workingDatabase () ;
	if ( index >= m_iNumOffsets ) 
		return (Acad::eInvalidIndex) ;
	if ( !pDb->isValidLineWeight (lw) )
		return (Acad::eInvalidInput) ;
	m_lweightArray [index] =lw ;
	return (Acad::eOk) ;
}

Acad::ErrorStatus AsdkOPLineStyle::lineWeightOf (Adesk::UInt32 index, AcDb::LineWeight &lw) {
	if ( index >= m_iNumOffsets )
		return (Acad::eInvalidIndex) ;
	lw =m_lweightArray [index] ;
	return (Acad::eOk) ;
}

Acad::ErrorStatus AsdkOPLineStyle::setStyleFrom (AcDbObjectId sourceId) {
	return (Acad::eNotImplementedYet) ;
}

Acad::ErrorStatus AsdkOPLineStyle::appendOffset (double off) {
	//----- These are the defaults
	AcDbObjectId stdId ;
	//----- Get the objectId of continuous linetype
	AcDbDatabase *pDb =database () ;
	if ( pDb == NULL )
		pDb =acdbHostApplicationServices ()->workingDatabase () ;
	Acad::ErrorStatus es =AcDbSymbolUtilities::getLinetypeId (stdId, _T(/*NOXLATE*/"BYLAYER"), pDb) ;
	assert( es == Acad::eOk ) ;
	return (appendOffset (off, stdId, 256, AcDb::kLnWt000)) ;
}

Acad::ErrorStatus AsdkOPLineStyle::appendOffset (double off, AcDbObjectId ltId, Adesk::UInt16 color, AcDb::LineWeight lwt) {
	m_iNumOffsets++ ;
	m_offsetArray.append (off) ;
	m_ltypeArray.append (ltId) ;
	m_colorArray.append (color) ;
	m_lweightArray.append (lwt) ;
	return (Acad::eOk) ;
}

Acad::ErrorStatus AsdkOPLineStyle::deleteOffset (Adesk::UInt32 index) {
	if ( index >= m_iNumOffsets )
		return (Acad::eInvalidIndex) ;
	m_offsetArray.removeAt (index) ;
	m_ltypeArray.removeAt (index) ;
	m_colorArray.removeAt (index) ;
	m_lweightArray.removeAt (index) ;
	m_iNumOffsets-- ;
	return (Acad::eOk) ;
}

Adesk::UInt32 AsdkOPLineStyle::numOffsets () {
	return (m_iNumOffsets) ;
}

Acad::ErrorStatus AsdkOPLineStyle::wblockClone(AcRxObject* pOwnerObject,
                                               AcDbObject*& pClonedObject,
                                               AcDbIdMapping& idMap,
                                               Adesk::Boolean isPrimary) const
{
	assertReadEnabled();
	// TODO: implement this function.
    Acad::ErrorStatus es ;
	AcDb::DeepCloneType dc =idMap.deepCloneContext () ;

	if (   dc != AcDb::kDcXrefBind
		&& dc != AcDb::kDcXrefInsert
		&& dc != AcDb::kDcWblkObjects
	)
		return (AcDbObject::wblockClone(pOwnerObject, pClonedObject, idMap, isPrimary)) ;

	pClonedObject =NULL ;

	//----- Make sure we have not already done this one.
	AcDbIdPair idPair (objectId (), AcDbObjectId::kNull, true) ;
	if ( idMap.compute (idPair) && (idPair.value () != AcDbObjectId::kNull) )
		return (Acad::eOk) ;

	//----- Xref Bind will always pass in an AcDbDatabase* for pOwner, but
	//----- kDcWblkObjects may also pass in the destination dictionary already
	//----- open. So, if pDestDb == NULL, we know that we did not open the
	//----- dictionary here.
	AcDbDictionary *pDestDict =NULL ;
	AcDbDatabase *pDestDb =AcDbDatabase::cast (pOwnerObject) ;
	if ( pDestDb ) {
		AcDbDictionary *pNOD ;
		pDestDb->getNamedObjectsDictionary (pNOD, AcDb::kForRead) ;
		es =pNOD->getAt (ASDK_OPLINESTYLE, (AcDbObject *&)pDestDict, AcDb::kForWrite) ;
		pNOD->close () ;
		if ( es == Acad::eKeyNotFound ) {
			//----- It is safe to clone as usual in 2000
			return (AcDbObject::wblockClone (pOwnerObject, pClonedObject, idMap, isPrimary)) ;
		}
		if ( es != Acad::eOk )
			return (es) ;
	} else {
		pDestDict =AcDbDictionary::cast (pOwnerObject) ;
	}
	ASSERT ( pDestDict != NULL ) ;
	ASSERT ( pDestDb != NULL || dc == AcDb::kDcWblkObjects ) ;

	//----- Now determine how we should handle duplicates. Normally
	//----- we would honor the DuplicateRecordCloning type, but OPLines
	//----- cannot handle swapping styles, so we never ignore or replace
	//----- the records.
	AcDb::DuplicateRecordCloning drc =idMap.duplicateRecordCloning();

	//----- We never replace OPLineStyles. Since we only ignore exact
	//----- duplicates, replace and ignore are identical anyway.
	if ( drc == AcDb::kDrcReplace )
		drc =AcDb::kDrcIgnore ;

	AcDbDictionary *pDct ;
	acdbOpenObject (pDct, ownerId (), AcDb::kForRead) ;
	char *name =NULL ;
	pDct->nameAt (objectId (), name) ;
	pDct->close () ;
	CString csName (name) ;
	delete [] name ;

	bool bNewName =false ;
	AcDbObjectId dupId ;
	int idx =0 ; //----- mangle index
	CString csRootName, csXrefName ;

	switch ( drc ) {
		case AcDb::kDrcUnmangleName:
			bNewName =wblockObjectsUnmangleName (csName) ;
			//----- fall through ...
		case AcDb::kDrcIgnore:
			if ( pDestDict->getAt (csName, dupId) == Acad::eOk ) {
				//----- Normally, we consider a duplicate name enough to
				//----- ignore - but if the style is really not a duplicate,
				//----- OPLines crash - so we have to further verify the 
				//----- duplicate against the entire OPLineStyle.
				if ( isEquivalentDuplicate (dupId, drc) ) {
					if ( pDestDb )
						pDestDict->close () ;
					idPair.setKey (objectId ()) ;
					idPair.setValue (dupId) ;
					idPair.setIsCloned (false) ;
					return (idMap.assign (idPair)) ;

				} else {
					//----- Use an alternate mangling 
					csRootName =csName ;
					do {
						csName.Format (_T("%s%d"), csRootName, ++idx) ;
						ASSERT (idx < 1000) ;
					} while ( pDestDict->has (csName) ) ;
					bNewName = true;
				} 
			}
			break ;
		case AcDb::kDrcMangleName:
			//----- Use $0$name mangling
			csRootName =csName ;
			do {
				csName.Format (_T("%c%d%c%s"), kMchr, idx++, kMchr, csRootName) ;
				ASSERT( idx < 1000 ) ;
			} while ( pDestDict->has (csName) ) ;
			bNewName =true ;
			break ;
		case AcDb::kDrcXrefMangleName:
			//----- Use xref$0$name mangling

			//----- Get the xref name from which this style comes from. We need
			//----- to look this up, because unlike STRs, OPLineStyles are not
			//----- attached, so we don't start out with a name of the form,
			//----- "<xref>|<name>".  The bindManager uses graph of the current
			//----- drawing's Xrefs to find the correct name. If the name comes
			//----- back as NULL, then this OPLineStyle is not from an Xref'ed
			//----- database - and should not have gotten this call in the 
			//----- first place.

			//----- BindManager is only an issue in AutoCAD, which has XREF-BIND
			//----- Command. In DWG U, we should only get here via an API, not an
			//----- XREF-BIND operation, which means we shouldn't ever enter this
			//----- case in DWG U.
	        if ( acdbHostApplicationServices ()->acadInternalServices () ) {
				csXrefName =xrefName (database ()) ;
				ASSERT ( !csXrefName.IsEmpty ()) ;
				csRootName =csName ;
				do {
					csName.Format (_T("%s%c%d%c%s"), csXrefName, kMchr, idx++, kMchr, csRootName) ;
					ASSERT(idx < 1000);
				} while ( pDestDict->has (csName) ) ;
				bNewName =true ;
			}
			break ;
		default:  
			ASSERT ( 0 ) ;
	}

	//----- We pass in pDict so that wblockClone knows that we are going
	//----- to call setAt(), and it will not set isOwnerXlated to false.
	//----- Since we know the right owner and will do the setAt() here, the
	//----- setting of isPrimary does not matter. Also, we can get away
	//----- with calling setAt() here because it is not possible to nest
	//----- these calls. In other words, there is no way an OPLineStyle
	//----- entry can reference anything which would in turn reference
	//----- another OPLineStyle. 
	es =AcDbObject::wblockClone (pDestDict, pClonedObject, idMap, isPrimary) ;
	if ( es != Acad::eOk || pClonedObject == NULL ) {
		if ( pDestDb )
			pDestDict->close () ;
		return (es) ;
	}

	AcDbObjectId id ;
	es =pDestDict->setAt (csName, pClonedObject, id) ;

	//----- if OPLineStyles have duplicate name placements, both in the
	//----- dictionary, and in their own records. Do this:
	//if ( bNewName && (es == Acad::eOk) )
	//	es =((AsdkOPLineStyle *)pClonedObject)->setName (csName) ;

	if ( pDestDb )
		pDestDict->close () ;

	//----- If no errors, leave the clone open, as we are still
	//----- inside wblockClone().
	if ( es != Acad::eOk ) {
		pClonedObject->close () ;
		pClonedObject =NULL ;
	}
	return (es) ;
}

bool AsdkOPLineStyle::isEquivalentDuplicate (AcDbObjectId otherId, AcDb::DuplicateRecordCloning drc) const {
	//----- It doesn't make sense to call this function unless using
	//----- one of the following duplicate handling types.  The function
	//----- probably would not work properly, either.
	ASSERT ( drc == AcDb::kDrcIgnore || drc == AcDb::kDrcReplace || drc == AcDb::kDrcUnmangleName ) ;

	//----- If you want the 'Standard' style being always equivalent
	//----- then activate the following code
	/*AcDbDictionary *pDct ;
	acdbOpenObject (pDct, ownerId (), AcDb::kForRead) ;
	char *name =NULL ;
	pDct->nameAt (objectId (), name) ;
	pDct->close () ;
	CString mName (name);
	delete [] name ;

	if ( mName != ASDK_OPLINESTYLE_STD )
		return (true) ;*/

	AsdkOPLineStyle *pStyle ;
	if ( acdbOpenObject (pStyle, otherId, AcDb::kForRead) != Acad::eOk )
		return (false) ;

	if ( m_iNumOffsets != pStyle->numOffsets () )
		return (false) ;

	bool bSame =true ;
	for ( Adesk::UInt32 i =0 ; i < m_iNumOffsets ; i++ ) {
		if (   m_colorArray.at (i) != pStyle->m_colorArray.at (i)
			|| m_lweightArray.at (i) != pStyle->m_lweightArray.at (i)
			|| m_offsetArray.at (i) != pStyle->m_offsetArray.at (i)
		) {
			bSame =false ;
			break ;
		}
		//----- Linetypes are more difficult. The Ids will not compare,
		//----- so we have to compare by name - including doing any
		//----- unmangling, if that is the Drc mode.
		char *name ;
		CString csMyLtName, csOtherLtName ;
		AcDbSymUtil::getSymbolName (name, m_ltypeArray.at (i)) ;
		csMyLtName =name ;
		delete [] name ;
		AcDbSymUtil::getSymbolName (name, pStyle->m_ltypeArray.at (i)) ;
		csOtherLtName =name ;
		delete [] name ;
		if ( drc == AcDb::kDrcUnmangleName )
			wblockObjectsUnmangleName (csMyLtName) ;
		if ( acdbSymUtil ()->compareSymbolName (csMyLtName, csOtherLtName) != 0 ) {
			bSame =false ;
			break ;
		}
	}

	pStyle->close () ;
	return (bSame) ;
}
