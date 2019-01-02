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
#ifndef _AsdkOPLineFiler_H_
#define _AsdkOPLineFiler_H_

///////////////////////////////////////////////////////////////////////////////
// Custom DWG Filer class
class AsdkOPLineFiler: public AcDbDwgFiler
{
public:
    ACRX_DECLARE_MEMBERS( AsdkOPLineFiler );

                        AsdkOPLineFiler ();
                        AsdkOPLineFiler (resbuf*);
    virtual             ~AsdkOPLineFiler ();

    const resbuf*       getResbuf () const;

    Acad::ErrorStatus   filerStatus () const;
    AcDb::FilerType     filerType () const;
    void                setFilerStatus (Acad::ErrorStatus) ;
    void                resetFilerStatus ();

    //  readXxx() and writeXxx() functions

    Acad::ErrorStatus readHardOwnershipId (AcDbHardOwnershipId*);
    Acad::ErrorStatus writeHardOwnershipId (const AcDbHardOwnershipId&);

    Acad::ErrorStatus readSoftOwnershipId (AcDbSoftOwnershipId*);
    Acad::ErrorStatus writeSoftOwnershipId (const AcDbSoftOwnershipId&);

    Acad::ErrorStatus readHardPointerId (AcDbHardPointerId*);
    Acad::ErrorStatus writeHardPointerId (const AcDbHardPointerId&);

    Acad::ErrorStatus readSoftPointerId (AcDbSoftPointerId*);
    Acad::ErrorStatus writeSoftPointerId (const AcDbSoftPointerId&);

    Acad::ErrorStatus readChar (char*);
    Acad::ErrorStatus writeChar (char);

    Acad::ErrorStatus readString (char**);
    Acad::ErrorStatus writeString (const char*);

    Acad::ErrorStatus readBChunk (ads_binary *);
    Acad::ErrorStatus writeBChunk (const ads_binary&);

    Acad::ErrorStatus readAcDbHandle (AcDbHandle*);
    Acad::ErrorStatus writeAcDbHandle (const AcDbHandle&);

    Acad::ErrorStatus readInt32 (Adesk::Int32*);
    Acad::ErrorStatus writeInt32 (Adesk::Int32);

    Acad::ErrorStatus readInt16 (Adesk::Int16*);
    Acad::ErrorStatus writeInt16 (Adesk::Int16);

    Acad::ErrorStatus readUInt32 (Adesk::UInt32*);
    Acad::ErrorStatus writeUInt32 (Adesk::UInt32);

    Acad::ErrorStatus readUInt16 (Adesk::UInt16*);
    Acad::ErrorStatus writeUInt16 (Adesk::UInt16);

    Acad::ErrorStatus readUInt8 (Adesk::UInt8*);
    Acad::ErrorStatus writeUInt8 (Adesk::UInt8);

    Acad::ErrorStatus readBoolean (Adesk::Boolean*);
    Acad::ErrorStatus writeBoolean (Adesk::Boolean);

    Acad::ErrorStatus readBool (bool *);
    Acad::ErrorStatus writeBool (bool);

    Acad::ErrorStatus readDouble (double*);
    Acad::ErrorStatus writeDouble (double);

    Acad::ErrorStatus readPoint2d (AcGePoint2d*);
    Acad::ErrorStatus writePoint2d (const AcGePoint2d&);

    Acad::ErrorStatus readPoint3d (AcGePoint3d*);
    Acad::ErrorStatus writePoint3d (const AcGePoint3d&);

    Acad::ErrorStatus readVector2d (AcGeVector2d*);
    Acad::ErrorStatus writeVector2d (const AcGeVector2d&);

    Acad::ErrorStatus readVector3d (AcGeVector3d*);
    Acad::ErrorStatus writeVector3d (const AcGeVector3d&);

    Acad::ErrorStatus readScale3d (AcGeScale3d*);
    Acad::ErrorStatus writeScale3d (const AcGeScale3d&);

    Acad::ErrorStatus readBytes (void *, Adesk::UInt32);
    Acad::ErrorStatus writeBytes (const void *, Adesk::UInt32);

    Acad::ErrorStatus seek (long offset, int method);
    long              tell () const;

private:
    resbuf             *m_rb,
                      **m_ppwalk;
    Acad::ErrorStatus   m_status;
} ;

#endif // _AsdkOPLineFiler_H_
