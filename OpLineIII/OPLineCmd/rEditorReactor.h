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
// AcEditorReactor reactors.

#if !defined(ARX__REDITORREACTOR_H__19991223_133534)
#define ARX__REDITORREACTOR_H__19991223_133534

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbidmap.h"
#include "aced.h"


class AsdkEdReact : public AcEditorReactor
{
public:

	// Constructor / Destructor
	AsdkEdReact(const bool autoInitAndRelease = true);
	virtual ~AsdkEdReact();

	//{{AFX_ARX_METHODS(AsdkEdReact)
	virtual void abortSave(AcDbDatabase* x0);
	virtual void saveComplete(AcDbDatabase* x0,
	                          const char* pActualName);
	virtual void beginSave(AcDbDatabase* x0,
	                       const char* pIntendedName);
	virtual void dwgFileOpened(AcDbDatabase* x0,
	                           char* fileName);
	//}}AFX_ARX_METHODS

private:
	// Auto initialization and release flag.
	bool m_autoInitAndRelease;
};



#endif // !defined(ARX__REDITORREACTOR_H__19991223_133534)