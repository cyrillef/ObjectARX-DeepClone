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

#include "StdAfx.h"
#include "StdArx.h"


AsdkEdReactor::AsdkEdReactor(const bool autoInitAndRelease)
{
	m_autoInitAndRelease = autoInitAndRelease;
	if (m_autoInitAndRelease)
		if (NULL != acedEditor)
			acedEditor->addReactor(this);
		else
			m_autoInitAndRelease = false;
}

AsdkEdReactor::~AsdkEdReactor()
{
	if (m_autoInitAndRelease)
		if (NULL != acedEditor)
			acedEditor->removeReactor(this);
}

void AsdkEdReactor::commandWillStart(const char* cmdStr)
{
	// TODO: implement this function.
	if ( stricmp (cmdStr, "PEDIT") == 0 )
		set_bPEdit (true) ;
}

void AsdkEdReactor::commandEnded(const char* cmdStr)
{
	// TODO: implement this function.
	if ( stricmp (cmdStr, "PEDIT") == 0 )
		set_bPEdit (false) ;
}

void AsdkEdReactor::commandCancelled(const char* cmdStr)
{
	// TODO: implement this function.
	if ( stricmp (cmdStr, "PEDIT") == 0 )
		set_bPEdit (false) ;
}

void AsdkEdReactor::commandFailed(const char* cmdStr)
{
	// TODO: implement this function.
	if ( stricmp (cmdStr, "PEDIT") == 0 )
		set_bPEdit (false) ;
}

