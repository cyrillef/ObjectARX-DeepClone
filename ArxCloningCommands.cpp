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
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"

#include "CustObj/AdcgMyDictObj.h"
#include "CustObj/AdcgMyLine.h"

//-----------------------------------------------------------------------------
Acad::ErrorStatus postToDatabase (AcDbEntity *pEnt, AcDbObjectId &idObj) {
	//----- Purpose:
	//----- Adds an entity to the MODEL_SPACE of the CURRENT database.
	//----- Note:
	//----- It could be generalized to add it to any block table record of
	//-----  any database, but why complicate it...
	Acad::ErrorStatus es ;
	AcDbBlockTable *pBlockTable ;
	AcDbBlockTableRecord *pSpaceRecord ;

	//----- Get a pointer to the current drawing
	//----- and get the drawing's block table. Open it for read.
	if ( (es =acdbHostApplicationServices()->workingDatabase()->getBlockTable (pBlockTable, AcDb::kForRead)) == Acad::eOk ) {
		//----- Get the Model Space record and open it for write. This will be the owner of the new line.
		if ( (es =pBlockTable->getAt (ACDB_MODEL_SPACE, pSpaceRecord, AcDb::kForWrite)) == Acad::eOk ) {
			//----- Append pEnt to Model Space, then close it and the Model Space record.
			es =pSpaceRecord->appendAcDbEntity (idObj, pEnt) ;
			pSpaceRecord->close () ;
		}
		pBlockTable->close () ;
	}
	//----- It is good programming practice to return an error status
	return (es) ;
}

AcDbObjectId SelectEnt (AcRxClass *pClass) {
	ads_name ename ;
	char buffer [256], *p =NULL ;
	if ( pClass ) {
		sprintf (buffer, "Select a %s: ", pClass->name ()) ;
		p =buffer ;
	}
	AcGePoint3d pt ;
	if ( acedEntSel (p, ename, asDblArray (pt)) != RTNORM )
		return (AcDbObjectId::kNull) ;
	AcDbObjectId id ;
	acdbGetObjectId (id, ename) ;
	if ( pClass ) {
		AcDbObject *pObj=NULL ;
		acdbOpenAcDbObject (pObj, id, AcDb::kForRead) ;
		if ( !pObj->isKindOf (pClass) ) {
			pObj->close () ;
			return (AcDbObjectId::kNull) ;
		}
		pObj->close () ;
	}
	return (id) ;
}

AcDbObjectId CreateLine () {
	AcDbLine *pLine =new AcDbLine (AcGePoint3d (0, 0, 0), AcGePoint3d (1, 1, 0)) ;
	pLine->setDatabaseDefaults (acdbHostApplicationServices ()->workingDatabase ()) ;
	AcDbObjectId idLine ;
	postToDatabase (pLine, idLine) ;
	pLine->close () ;
	return (idLine) ;
}

// This is command 'CLONE1'
void adcgclone1()
{
	//----- Create a non-database resident object with some settings
	AcDbLine *pOrgLine =new AcDbLine (AcGePoint3d (0, 0, 0), AcGePoint3d (1, 1, 0)) ;
	pOrgLine->setDatabaseDefaults (acdbHostApplicationServices ()->workingDatabase ()) ;
	//----- Perform the clone() operation
	AcDbLine *pClonedLine =(AcDbLine *)pOrgLine->clone () ;
	//----- Is color the same?
	if ( pOrgLine->colorIndex () == pClonedLine->colorIndex () )
		acutPrintf ("Colors are the same. Object cloned properly!\n") ;
	else
		acutPrintf ("Colors are not the same. Failed to clone Object properly!\n") ;
	//----- Cleanup (because non-resident objects!)
	delete pOrgLine ;
	delete pClonedLine ;
}

// This is command 'CLONE2'
void adcgclone2()
{
	//----- Select a line in the DWG
	AcDbObjectId idOfLine ;
	if ( (idOfLine =SelectEnt (AcDbLine::desc ())) == AcDbObjectId::kNull )
		return ;
	//----- Open the original object for read
	AcDbLine *pOrgLine =NULL ;
	acdbOpenAcDbObject ((AcDbObject *&)pOrgLine, idOfLine, AcDb::kForRead) ;
	//----- Perform the clone() operation
	AcDbLine *pClonedLine =(AcDbLine *)pOrgLine->clone () ;
	//----- Append the new line to the same block definition
	AcDbBlockTableRecord *pBlock =NULL ;
	acdbOpenAcDbObject (
		(AcDbObject *&)pBlock,
		pOrgLine->blockId (),
		AcDb::kForWrite
	) ;
	pBlock->appendAcDbEntity (pClonedLine) ;
	//----- Move the entity around
	AcGeMatrix3d mat ;
	mat.setToTranslation (AcGeVector3d (1, 0, 0)) ;
	pClonedLine->transformBy (mat) ;
	pClonedLine->close () ;
	pBlock->close () ;
	//----- Close the original object
	pOrgLine->close () ;
}

// This is command 'CLONE3'
void adcgclone3()
{
	//----- Select a line in the DWG
	AcDbObjectId idOfLine ;
	if ( (idOfLine =SelectEnt (AcDbLine::desc ())) == AcDbObjectId::kNull )
		return ;
	//----- Open the original object for read
	AcDbLine *pOrgLine =NULL ;
	acdbOpenAcDbObject ((AcDbObject *&)pOrgLine, idOfLine, AcDb::kForRead) ;
	//----- Perform the clone() operation
	AcDbLine *pClonedLine =(AcDbLine *)pOrgLine->clone () ;
	//----- Create a new database
	AcDbDatabase *pDwg =new AcDbDatabase (true) ;
	//----- Append the new line into the target database's ModelSpace
	AcDbBlockTable *pBT =NULL ;
	AcDbBlockTableRecord *pBlock =NULL ;
	pDwg->getBlockTable (pBT, AcDb::kForRead) ;
	//----- Get the Model Space record and open it for write. This will be the
	//----- owner of the new line.
	pBT->getAt (ACDB_MODEL_SPACE, pBlock, AcDb::kForWrite) ;
	//----- At that point, if you forget to reset AcDb(Hard/Soft)PointerId you may
	//----- crash AutoCAD immediately or later.
	//----- In our sample (using Drawing1.dwg) lets reset the LayerRecord to an
	//----- appropriate value (ie: Layer "0")
	AcDbLayerTable *pLT =NULL ;
	AcDbObjectId idLayer ;
	pDwg->getLayerTable (pLT, AcDb::kForRead) ;
	//----- Get the Layer "0"
	pLT->getAt ("0", idLayer) ;
	pClonedLine->setLayer (idLayer) ;
	pLT->close () ;
	//----- Append the entity
	pBlock->appendAcDbEntity (pClonedLine) ;
	pClonedLine->close () ;
	pBlock->close () ;
	pBT->close () ;
	//----- Close the original object
	pOrgLine->close () ;
	//----- Save DWG
	pDwg->saveAs ("Test.dwg") ;
	delete pDwg ;
}

// This is command 'DCLONE1'
void adcgdclone1()
{
	//----- Select an object in the DWG
	AcDbObjectId id ;
	if ( (id =SelectEnt (NULL)) == AcDbObjectId::kNull )
		return ;
	//----- Open the original object for read
	AcDbEntity *pEnt =NULL ;
	acdbOpenAcDbObject ((AcDbObject *&)pEnt, id, AcDb::kForRead) ;
	AcDbObjectId idOwner =pEnt->blockId () ;
	pEnt->close () ;
	//----- Perform the deepClone() operation
	AcDbObjectIdArray array ;
	AcDbIdMapping idMap ;
	array.append (id) ;
	acdbHostApplicationServices ()->workingDatabase ()->deepCloneObjects (array, idOwner, idMap, false) ;
}

// This is command 'DCLONE2'
void adcgdclone2()
{
	//----- Select an object in the DWG
	AcDbObjectId id ;
	if ( (id =SelectEnt (NULL)) == AcDbObjectId::kNull )
		return ;
	//----- Create a new database
	AcDbDatabase *pDwg =new AcDbDatabase (true) ;
	//----- Get the Model Space ObjectID. It will be the new owner
	AcDbBlockTable *pBT =NULL ;
	AcDbObjectId idOwner ;
	pDwg->getBlockTable (pBT, AcDb::kForRead) ;
	pBT->getAt (ACDB_MODEL_SPACE, idOwner) ;
	pBT->close () ;
	//----- Perform the deepClone() operation
	AcDbObjectIdArray array ;
	AcDbIdMapping idMap ;
	array.append (id) ;
	Acad::ErrorStatus e =acdbHostApplicationServices ()->workingDatabase ()->deepCloneObjects (array, idOwner, idMap, false) ;
	//----- Return eWrongDatabase
	acutPrintf ("Returned 'eWrongDatabase'\n") ;
	//----- Save DWG
	pDwg->saveAs ("Test.dwg") ;
	delete pDwg ;
}

// This is command 'DCLONE3'
void adcgdclone3()
{
	//----- Select an object in the DWG
	AcDbObjectId id ;
	if ( (id =SelectEnt (NULL)) == AcDbObjectId::kNull )
		return ;
	//----- Create a new database
	AcDbDatabase *pDwg =new AcDbDatabase (true) ;
	//----- Perform the deepClone() operation
	AcDbObjectIdArray array ;
	array.append (id) ;
	acdbHostApplicationServices ()->workingDatabase ()->wblock (pDwg, array, AcGePoint3d ()) ;
	//----- Save DWG
	pDwg->saveAs ("Test.dwg") ;
	delete pDwg ;
}

// This is command 'DCLONE4'
void adcgdclone4()
{
	//----- Check if Test.dwg is there
	AcDbDatabase *pOrgDwg =new AcDbDatabase (false) ;
	if ( pOrgDwg->readDwgFile ("Test.dwg") != Acad::eOk ) {
		acutPrintf ("File 'Test.dwg' not found!\n") ;
		delete pOrgDwg ;
		return ;
	}
	//----- Create a new database
	AcDbDatabase *pDwg =new AcDbDatabase (true) ;
	pDwg->insert (AcGeMatrix3d (), pOrgDwg, true) ; // false
	delete pOrgDwg ;
	//----- Save DWG
	pDwg->saveAs ("Test1.dwg") ;
	delete pDwg ;
}
// This is command 'DCLONE5'
void adcgdclone5()
{
	//----- Select an object in the DWG
	AcDbObjectId id ;
	if ( (id =SelectEnt (NULL)) == AcDbObjectId::kNull )
		return ;
	//----- Open the original object for read
	AcDbEntity *pEnt =NULL ;
	acdbOpenAcDbObject ((AcDbObject *&)pEnt, id, AcDb::kForRead) ;
	AcDbObjectId idOwner =pEnt->blockId () ;
	AcDbBlockTableRecord *pBlock =NULL ;
	acdbOpenAcDbObject ((AcDbObject *&)pBlock, idOwner, AcDb::kForWrite) ;
	//----- Perform the deepClone() operation
	AcDbIdMapping idMap ;
	AcDbObject *pObj =NULL ;
	Acad::ErrorStatus es =pEnt->deepClone (pBlock, pObj, idMap, true) ;
	pBlock->close () ;
	pEnt->close () ;
	pObj->close () ;
	//----- End
	acutPrintf ("deepClone() succesfully returned cloned objects.\n") ;
	acutPrintf ("But none of there object can be used, nor have been appended in the database.\n") ;
}

// This is command 'CRSTRUCT'
// service is ADCGCUSTOBJ_DBXSERVICE
void adcgcrstruct()
{
	AdcgMyLine *pml =new AdcgMyLine ;
	pml->setDatabaseDefaults () ;
	pml->setStartPoint (AcGePoint3d ()) ;
	pml->setEndPoint (AcGePoint3d (1, 1, 0)) ;
	AcDbObjectId idml ;
	postToDatabase (pml, idml) ;

	AcDbLine *pl1, *pl2 ;
	AcDbObjectId idl1, idl2 ;

	pl1 =new AcDbLine ;
	pl1->setDatabaseDefaults () ;
	pl1->setStartPoint (AcGePoint3d (-1, 0, 0)) ;
	pl1->setEndPoint (AcGePoint3d (0, 1, 0)) ;
	postToDatabase (pl1, idl1) ;

	pl2 =new AcDbLine ;
	pl2->setDatabaseDefaults () ;
	pl2->setStartPoint (AcGePoint3d (1, 0, 0)) ;
	pl2->setEndPoint (AcGePoint3d (2, 1, 0)) ;
	postToDatabase (pl2, idl2) ;

	AdcgMyDictObj *pObj =new AdcgMyDictObj ;
	pObj->setMyLine (idml) ;
	pObj->setToLine1 (idl1) ;
	pObj->setToLine2 (idl2) ;

	AcDbDictionary *pNOD, *pDict ;
	AcDbObjectId iddict ;
	acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary (pNOD, AcDb::kForRead) ;
	if ( !pNOD->has ("MyDict") ) {
		pNOD->upgradeOpen () ;
		pDict =new AcDbDictionary ;
		pNOD->setAt ("MyDict", pDict, iddict) ;
		pNOD->downgradeOpen () ;
	} else {
		pNOD->getAt ("MyDict", (AcDbObject *&)pDict, AcDb::kForWrite) ;
	}

	pDict->setAt ("MyObj", pObj, iddict) ;
	pml->setMyObj (pObj->objectId ()) ;

	pDict->close () ;
	pNOD->close () ;

	pObj->close () ;

	pml->close () ;
	pl1->close () ;
	pl2->close () ;
}
// This is command 'QSTRUCT'
void adcgqstruct()
{
	//----- Select an object in the DWG
	AcDbObjectId id ;
	if ( (id =SelectEnt (AdcgMyLine::desc())) == AcDbObjectId::kNull )
		return ;

	AdcgMyLine *pl ;
	acdbOpenAcDbObject ((AcDbObject *&)pl, id, AcDb::kForRead) ;

	AdcgMyDictObj *po ;
	AcDbHardPointerId ido ;
	pl->myObj (ido) ;
	acdbOpenAcDbObject ((AcDbObject *&)po, ido, AcDb::kForRead) ;

	AcDbSoftPointerId id1, id2 ;
	po->toLine1 (id1) ;
	po->toLine2 (id2) ;

	acutPrintf ("MyLine: %ld -> MyObj: %ld =>> %ld, %ld\n",
		id.asOldId (), ido.asOldId (), id1.asOldId (), id2.asOldId ()
	) ;

	pl->close () ;
	po->close () ;
}

// This is command 'CSTRUCT'
void adcgcstruct()
{
	//----- Select an object in the DWG
	acutPrintf ("We will clone only from the dictionary object. However, please\n") ;
	AcDbObjectId id ;
	if ( (id =SelectEnt (AdcgMyLine::desc())) == AcDbObjectId::kNull )
		return ;

	AdcgMyLine *pl ;
	acdbOpenAcDbObject ((AcDbObject *&)pl, id, AcDb::kForRead) ;

	AdcgMyDictObj *po ;
	AcDbHardPointerId ido ;
	pl->myObj (ido) ;
	pl->close () ;

	acdbOpenAcDbObject ((AcDbObject *&)po, ido, AcDb::kForRead) ;
	AcDbObjectId idOwner =po->ownerId () ;
	po->close () ;

	//----- Perform the deepClone() operation
	AcDbObjectIdArray array ;
	AcDbIdMapping idMap ;
	array.append (ido) ;
	acdbHostApplicationServices ()->workingDatabase ()->deepCloneObjects (array, idOwner, idMap, false) ;

}

