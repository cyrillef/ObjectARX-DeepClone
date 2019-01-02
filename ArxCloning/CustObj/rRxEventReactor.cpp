//-----------------------------------------------------------------------------
// AcRxEventReactor reactors, created by  [Nov/06/2002], , 
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "StdArx.h"

//-----------------------------------------------------------------------------
AdcgMyDictObjSetAt::AdcgMyDictObjSetAt (const bool autoInitAndRelease) {
	m_autoInitAndRelease =autoInitAndRelease ;
	if ( m_autoInitAndRelease ) {
		if ( NULL != acrxEvent )
			acrxEvent->addReactor (this) ;
		else
			m_autoInitAndRelease =false ;
	}
}

//-----------------------------------------------------------------------------
AdcgMyDictObjSetAt::~AdcgMyDictObjSetAt () {
	if ( m_autoInitAndRelease ) {
		if ( NULL != acrxEvent )
			acrxEvent->removeReactor (this) ;
	}
}

//-----------------------------------------------------------------------------
void AdcgMyDictObjSetAt::beginDeepClone (AcDbDatabase *pTo, AcDbIdMapping &x0) {
	// TODO: implement this function.
}

//-----------------------------------------------------------------------------
void AdcgMyDictObjSetAt::beginDeepCloneXlation (AcDbIdMapping &x0, Acad::ErrorStatus *x1) {
	// TODO: implement this function.
}

//-----------------------------------------------------------------------------
void AdcgMyDictObjSetAt::endDeepClone (AcDbIdMapping &x0) {
	// TODO: implement this function.
	AcDbIdMappingIter iter (x0) ;
	while ( !iter.done () ) {
		AcDbIdPair pair ;
		iter.getMap (pair) ;
		iter.next () ;

		AcDbObject *pObj =NULL ;
		AcDbObjectId idObj =pair.value () ;
		if ( idObj == AcDbObjectId::kNull )
			continue ;
		Acad::ErrorStatus es =acdbOpenAcDbObject (pObj, idObj, AcDb::kForWrite) ;
		if ( es != Acad::eOk )
			continue ;
		if ( pObj->isKindOf (AdcgMyDictObj::desc ()) ) {
			AcDbDictionaryPointer nod (pObj->database ()->namedObjectsDictionaryId (), AcDb::kForRead) ;
			AcDbObjectId idDict ;
			nod->getAt ("Mydict", idDict) ;
			AcDbDictionaryPointer dict (idDict, AcDb::kForWrite) ;
			dict->remove (idObj) ;
			dict->setAt ("*", pObj, idObj) ;
		}
		pObj->close () ;
	}
}

