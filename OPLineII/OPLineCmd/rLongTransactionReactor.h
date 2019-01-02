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
// AcApLongTransactionReactor reactors.

#if !defined(ARX__RLONGTRANSACTIONREACTOR_H__19991126_151331)
#define ARX__RLONGTRANSACTIONREACTOR_H__19991126_151331

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbltrans.h"
#include "lngtrans.h"





class AsdkLgTransReactor : public AcApLongTransactionReactor
{
public:

	// Constructor / Destructor
	AsdkLgTransReactor(const bool autoInitAndRelease = true);
	virtual ~AsdkLgTransReactor();

	//{{AFX_ARX_METHODS(AsdkLgTransReactor)
	virtual void beginCheckOut(AcDbLongTransaction& x0,
	                           AcDbObjectIdArray& originList);
	virtual void endCheckIn(AcDbLongTransaction& x0) ;
	//}}AFX_ARX_METHODS

private:
	// Auto initialization and release flag.
	bool m_autoInitAndRelease;
};

#endif // !defined(ARX__RLONGTRANSACTIONREACTOR_H__19991126_151331)