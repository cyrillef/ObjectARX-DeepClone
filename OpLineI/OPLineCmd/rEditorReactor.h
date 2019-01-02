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

#if !defined(ARX__REDITORREACTOR_H__20000104_113948)
#define ARX__REDITORREACTOR_H__20000104_113948

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbidmap.h"
#include "aced.h"


class AsdkEdReactor : public AcEditorReactor
{
public:

	// Constructor / Destructor
	AsdkEdReactor(const bool autoInitAndRelease = true);
	virtual ~AsdkEdReactor();

	//{{AFX_ARX_METHODS(AsdkEdReactor)
	virtual void commandFailed(const char* cmdStr);
	virtual void commandCancelled(const char* cmdStr);
	virtual void commandEnded(const char* cmdStr);
	virtual void commandWillStart(const char* cmdStr);
	//}}AFX_ARX_METHODS

private:
	// Auto initialization and release flag.
	bool m_autoInitAndRelease;
};



#endif // !defined(ARX__REDITORREACTOR_H__20000104_113948)