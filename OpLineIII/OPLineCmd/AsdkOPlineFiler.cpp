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

/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "StdArx.h"

///////////////////////////////////////////////////////////////////////////////
// Custom DWG Filer class
ACRX_CONS_DEFINE_MEMBERS( AsdkOPLineFiler, AcDbDwgFiler, 0 );

AsdkOPLineFiler::AsdkOPLineFiler() : m_rb( NULL ), m_ppwalk( &m_rb ),
    m_status( Acad::eOk )
{
    *m_ppwalk = acutNewRb( AcDb::kDxfRegAppName );
    (*m_ppwalk)->resval.rstring = strdup( "ASDKOPLINE" );
    m_ppwalk = &(*m_ppwalk)->rbnext;
}

AsdkOPLineFiler::AsdkOPLineFiler( resbuf* pRb ) : m_rb( pRb ),
    m_ppwalk( &m_rb ), m_status( Acad::eOk )
{
    // skip kDxfRegAppName record
    m_ppwalk = &(*m_ppwalk)->rbnext;
}

AsdkOPLineFiler::~AsdkOPLineFiler()
{
    if (NULL != m_rb)
        acutRelRb( m_rb );
}

const resbuf*
AsdkOPLineFiler::getResbuf() const
{
    return m_rb;
}

Acad::ErrorStatus
AsdkOPLineFiler::filerStatus() const
{
    return m_status;
}

AcDb::FilerType
AsdkOPLineFiler::filerType() const
{
    return AcDb::kCopyFiler;
}

void
AsdkOPLineFiler::setFilerStatus( Acad::ErrorStatus es )
{
    m_status = es;
}

void
AsdkOPLineFiler::resetFilerStatus()
{
    m_status = Acad::eOk;
}

//  readXxx() and writeXxx() functions

Acad::ErrorStatus
AsdkOPLineFiler::readHardOwnershipId( AcDbHardOwnershipId* id )
{
    return readSoftPointerId( (AcDbSoftPointerId*)id );
}

Acad::ErrorStatus
AsdkOPLineFiler::writeHardOwnershipId( const AcDbHardOwnershipId& id )
{
    return writeSoftPointerId( id );
}

Acad::ErrorStatus
AsdkOPLineFiler::readSoftOwnershipId( AcDbSoftOwnershipId* id )
{
    return readSoftPointerId( (AcDbSoftPointerId*)id );
}

Acad::ErrorStatus
AsdkOPLineFiler::writeSoftOwnershipId( const AcDbSoftOwnershipId& id )
{
    return writeSoftPointerId( id );
}

Acad::ErrorStatus
AsdkOPLineFiler::readHardPointerId( AcDbHardPointerId* id )
{
    return readSoftPointerId( (AcDbSoftPointerId*)id );
}

Acad::ErrorStatus
AsdkOPLineFiler::writeHardPointerId( const AcDbHardPointerId& id )
{
    return writeSoftPointerId( id );
}

Acad::ErrorStatus
AsdkOPLineFiler::readSoftPointerId( AcDbSoftPointerId* id )
{
    AcDbHandle handle;
    Acad::ErrorStatus es = readAcDbHandle( &handle );

    if (Acad::eOk != es)
        return es;

    if (Acad::eOk !=
        acdbHostApplicationServices ()->workingDatabase ()->getAcDbObjectId( *id, Adesk::kFalse, handle ))
    {
        *id = 0;
    }
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::writeSoftPointerId( const AcDbSoftPointerId& id )
{
    AcDbObject* pObj;
    AcDbHandle handle;
    
/*    if (Acad::eOk == acdbOpenObject( pObj, id, AcDb::kForRead ))
    {
        pObj->getAcDbHandle( handle );
        pObj->close();
    }
    else
        handle = (Adesk::UInt32)0;*/
    return writeAcDbHandle( /*handle */id.handle ());
}

Acad::ErrorStatus
AsdkOPLineFiler::readChar( char* c )
{
    char *pTmp;
    Acad::ErrorStatus es = readString( &pTmp );
    if (Acad::eOk != es)
        return es;

    *c = *pTmp;
    free( pTmp );
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::writeChar( char c )
{
    // Convert it to a string in order to
    // allow code page translations.
    //
    char tmp[2];
    tmp[0] = c;
    tmp[1] = '\0';
    return writeString( tmp );
}

Acad::ErrorStatus
AsdkOPLineFiler::readString( char** ppStr )
{
    if (AcDb::kDxfXdAsciiString != (*m_ppwalk)->restype)
    {
        return Acad::eInvalidResBuf;
    }

    *ppStr = strdup( (*m_ppwalk)->resval.rstring );
    m_ppwalk = &(*m_ppwalk)->rbnext;
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::writeString( const char* pStr )
{
    *m_ppwalk = acutNewRb( AcDb::kDxfXdAsciiString );
    (*m_ppwalk)->resval.rstring = strdup( pStr );
    m_ppwalk = &(*m_ppwalk)->rbnext;
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::readBChunk( ads_binary *buf )
{
    if (AcDb::kDxfXdBinaryChunk != (*m_ppwalk)->restype)
    {
        return Acad::eInvalidResBuf;
    }

    buf->clen = (*m_ppwalk)->resval.rbinary.clen;
    buf->buf = (char*)malloc( buf->clen );
    memcpy( buf->buf, (*m_ppwalk)->resval.rbinary.buf, buf->clen );
    m_ppwalk = &(*m_ppwalk)->rbnext;
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::writeBChunk( const ads_binary& buf )
{
    *m_ppwalk = acutNewRb( AcDb::kDxfXdBinaryChunk );
    (*m_ppwalk)->resval.rbinary.clen = buf.clen;
    (*m_ppwalk)->resval.rbinary.buf = (char*)malloc( buf.clen );
    memcpy( (*m_ppwalk)->resval.rbinary.buf, buf.buf, buf.clen );
    m_ppwalk = &(*m_ppwalk)->rbnext;
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::readAcDbHandle( AcDbHandle* handle )
{
    if (AcDb::kDxfXdHandle != (*m_ppwalk)->restype)
    {
        return Acad::eInvalidResBuf;
    }

    *handle = (*m_ppwalk)->resval.rstring;
    m_ppwalk = &(*m_ppwalk)->rbnext;
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::writeAcDbHandle( const AcDbHandle& handle )
{

    *m_ppwalk = acutNewRb( AcDb::kDxfXdHandle );
    (*m_ppwalk)->resval.rstring = (char*)malloc( 17 );
    handle.getIntoAsciiBuffer( (*m_ppwalk)->resval.rstring );
    m_ppwalk = &(*m_ppwalk)->rbnext;
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::readInt32( Adesk::Int32* i )
{
    return readUInt32( (Adesk::UInt32*)i );
}

Acad::ErrorStatus
AsdkOPLineFiler::writeInt32( Adesk::Int32 i )
{
    return writeUInt32( i );
}

Acad::ErrorStatus
AsdkOPLineFiler::readInt16( Adesk::Int16* i )
{
    return readUInt16( (Adesk::UInt16*)i );
}

Acad::ErrorStatus
AsdkOPLineFiler::writeInt16( Adesk::Int16 i )
{
    return writeUInt16( i );
}

Acad::ErrorStatus
AsdkOPLineFiler::readUInt32( Adesk::UInt32* i )
{
    AcDbObjectId id;
    AcDbHandle handle;
    Acad::ErrorStatus es;

    switch ((*m_ppwalk)->restype) {
    
    case AcDb::kDxfXdInteger32:
        *i = (*m_ppwalk)->resval.rlong;
        m_ppwalk = &(*m_ppwalk)->rbnext;
        es = Acad::eOk;
        break;

    case AcDb::kDxfXdHandle:
        // Surprised ?
        // ACAD sometimes writes NULL-ObjectId's as longs.
        //
        if (Acad::eOk != (es = readAcDbHandle( &handle )))
            return es;
        
        if (Acad::eOk ==
            acdbHostApplicationServices ()->workingDatabase ()->getAcDbObjectId( id, Adesk::kFalse, handle ))
        {
            *i = id.asOldId();
        }
        else
            *i = 0;
        es = Acad::eOk;
        break;

    default:
        es = Acad::eInvalidResBuf;
        break;
    }

    return es;
}

Acad::ErrorStatus
AsdkOPLineFiler::writeUInt32( Adesk::UInt32 i )
{
    *m_ppwalk = acutNewRb( AcDb::kDxfXdInteger32 );
    (*m_ppwalk)->resval.rlong = i;
    m_ppwalk = &(*m_ppwalk)->rbnext;
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::readUInt16( Adesk::UInt16* i )
{
    if (AcDb::kDxfXdInteger16 != (*m_ppwalk)->restype)
    {
        return Acad::eInvalidResBuf;
    }

    *i = (*m_ppwalk)->resval.rint;
    m_ppwalk = &(*m_ppwalk)->rbnext;
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::writeUInt16( Adesk::UInt16 i )
{
    *m_ppwalk = acutNewRb( AcDb::kDxfXdInteger16 );
    (*m_ppwalk)->resval.rint = i;
    m_ppwalk = &(*m_ppwalk)->rbnext;
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::readUInt8( Adesk::UInt8* i )
{
    Adesk::UInt16 i16;
    Acad::ErrorStatus es = readUInt16( &i16 );
    if (Acad::eOk != es)
        return es;

    *i = (Adesk::UInt8)i16;
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::writeUInt8( Adesk::UInt8 i )
{
    Adesk::UInt16 i16 = i;
    return writeUInt16( i16 );
}

Acad::ErrorStatus
AsdkOPLineFiler::readBoolean( Adesk::Boolean* b )
{
    Adesk::UInt16 i;
    Acad::ErrorStatus es = readUInt16( &i );
    if (Acad::eOk != es)
        return es;

    *b = i;
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::writeBoolean( Adesk::Boolean b )
{
    Adesk::UInt16 i = b;
    return writeUInt16( i );
}

Acad::ErrorStatus
AsdkOPLineFiler::readBool( bool* b )
{
    Adesk::UInt16 i;
    Acad::ErrorStatus es = readUInt16( &i );
    if (Acad::eOk != es)
        return es;

    *b = i ? true : false ;
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::writeBool( bool b )
{
    Adesk::UInt16 i = b;
    return writeUInt16( i );
}

Acad::ErrorStatus
AsdkOPLineFiler::readDouble( double* d )
{
    if (AcDb::kDxfXdReal != (*m_ppwalk)->restype)
    {
        return Acad::eInvalidResBuf;
    }

    *d = (*m_ppwalk)->resval.rreal;
    m_ppwalk = &(*m_ppwalk)->rbnext;
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::writeDouble( double d )
{
    *m_ppwalk = acutNewRb( AcDb::kDxfXdReal );
    (*m_ppwalk)->resval.rreal = d;
    m_ppwalk = &(*m_ppwalk)->rbnext;
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::readPoint2d( AcGePoint2d* p )
{
    AcGePoint3d p3d;
    Acad::ErrorStatus es = readPoint3d( &p3d );
    if (Acad::eOk != es)
        return es;
    (*p)[0] = p3d[0];
    (*p)[1] = p3d[1];
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::writePoint2d( const AcGePoint2d& p )
{
    AcGePoint3d p3d( p[0], p[1], 0.0 );
    return writePoint3d( p3d );
}

Acad::ErrorStatus
AsdkOPLineFiler::readPoint3d( AcGePoint3d* p )
{
    if (AcDb::kDxfXdXCoord != (*m_ppwalk)->restype)
    {
        return Acad::eInvalidResBuf;
    }

    (*p)[0] = (*m_ppwalk)->resval.rpoint[0];
    (*p)[1] = (*m_ppwalk)->resval.rpoint[1];
    (*p)[2] = (*m_ppwalk)->resval.rpoint[2];
    m_ppwalk = &(*m_ppwalk)->rbnext;
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::writePoint3d( const AcGePoint3d& p )
{
    *m_ppwalk = acutNewRb( AcDb::kDxfXdXCoord );
    (*m_ppwalk)->resval.rpoint[0] = p[0];
    (*m_ppwalk)->resval.rpoint[1] = p[1];
    (*m_ppwalk)->resval.rpoint[2] = p[2];
    m_ppwalk = &(*m_ppwalk)->rbnext;
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::readVector2d( AcGeVector2d* vec )
{
    return readPoint2d( (AcGePoint2d*)vec );
}

Acad::ErrorStatus
AsdkOPLineFiler::writeVector2d( const AcGeVector2d& vec )
{
    return writePoint2d( *(AcGePoint2d*)&vec );
}

Acad::ErrorStatus
AsdkOPLineFiler::readVector3d( AcGeVector3d* vec )
{
    return readPoint3d( (AcGePoint3d*)vec );
}

Acad::ErrorStatus
AsdkOPLineFiler::writeVector3d( const AcGeVector3d& vec )
{
    return writePoint3d( *(AcGePoint3d*)&vec );
}

Acad::ErrorStatus
AsdkOPLineFiler::readScale3d( AcGeScale3d* s )
{
    return readPoint3d( (AcGePoint3d*)s );
}

Acad::ErrorStatus
AsdkOPLineFiler::writeScale3d( const AcGeScale3d& s )
{
    return writePoint3d( *(AcGePoint3d*)&s );
}

Acad::ErrorStatus
AsdkOPLineFiler::readBytes( void *buf, Adesk::UInt32 len )
{
    ads_binary bin;
    Acad::ErrorStatus es = readBChunk( &bin );
    if (Acad::eOk != es)
        return es;

    memcpy( buf, bin.buf, bin.clen );
    free( bin.buf );
    return Acad::eOk;
}

Acad::ErrorStatus
AsdkOPLineFiler::writeBytes( const void *buf, Adesk::UInt32 len )
{
    // TODO!
    // ads_binary is limited to max. 127 bytes,
    // while writeBytes is not.
    //
    if (len > 127) {
        return Acad::eNotImplementedYet;
    }

    ads_binary bin = { (short)len, (char*)buf };
    return writeBChunk( bin );
}

Acad::ErrorStatus
AsdkOPLineFiler::seek( long offset, int method )
{
    return Acad::eNotImplementedYet;
}

long
AsdkOPLineFiler::tell() const
{
    return 0;
}
