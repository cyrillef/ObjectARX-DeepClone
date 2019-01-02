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

// ArxCloning.cpp : Initialization functions
#include "StdAfx.h"
#include "StdArx.h"
#include "resource.h"

#include "CustObj/AdcgMyDictObj.h"
#include "CustObj/AdcgMyLine.h"

HINSTANCE _hdllInstance =NULL ;

// This command registers an ARX command.
void AddCommand(const char* cmdGroup, const char* cmdInt, const char* cmdLoc,
				const int cmdFlags, const AcRxFunctionPtr cmdProc, const int idLocal = -1);

// NOTE: DO NOT edit the following lines.
//{{AFX_ARX_MSG
void InitApplication();
void UnloadApplication();
//}}AFX_ARX_MSG

// NOTE: DO NOT edit the following lines.
//{{AFX_ARX_ADDIN_FUNCS
//}}AFX_ARX_ADDIN_FUNCS


/////////////////////////////////////////////////////////////////////////////
// ObjectARX EntryPoint
extern "C" AcRx::AppRetCode 
acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt)
{
	switch (msg) {
	case AcRx::kInitAppMsg:
		// Comment out the following line if your
		// application should be locked into memory
		acrxDynamicLinker->unlockApplication(pkt);
		acrxDynamicLinker->registerAppMDIAware(pkt);
		_hdllInstance =::GetModuleHandle ("AdcgArxCloning.arx") ;
		InitApplication();
		break;
	case AcRx::kUnloadAppMsg:
		UnloadApplication();
		break;
	}
	return AcRx::kRetOK;
}


// Init this application. Register your
// commands, reactors...
void InitApplication()
{
	// NOTE: DO NOT edit the following lines.
	//{{AFX_ARX_INIT
	AddCommand("ADCG", "CLONE1", "CLONE1", ACRX_CMD_MODAL, adcgclone1);
	AddCommand("ADCG", "CLONE2", "CLONE2", ACRX_CMD_MODAL, adcgclone2);
	AddCommand("ADCG", "CLONE3", "CLONE3", ACRX_CMD_MODAL, adcgclone3);
	AddCommand("ADCG", "DCLONE1", "DCLONE1", ACRX_CMD_MODAL, adcgdclone1);
	AddCommand("ADCG", "DCLONE2", "DCLONE2", ACRX_CMD_MODAL, adcgdclone2);
	AddCommand("ADCG", "DCLONE3", "DCLONE3", ACRX_CMD_MODAL, adcgdclone3);
	AddCommand("ADCG", "DCLONE4", "DCLONE4", ACRX_CMD_MODAL, adcgdclone4);
	AddCommand("ADCG", "DCLONE5", "DCLONE5", ACRX_CMD_MODAL, adcgdclone5);
	AddCommand("ADCG", "CRSTRUCT", "CRSTRUCT", ACRX_CMD_MODAL, adcgcrstruct);
	AddCommand("ADCG", "QSTRUCT", "QSTRUCT", ACRX_CMD_MODAL, adcgqstruct);
	AddCommand("ADCG", "CSTRUCT", "CSTRUCT", ACRX_CMD_MODAL, adcgcstruct);
	//}}AFX_ARX_INIT

	// TODO: add your initialization functions
	new AdcgCMyEdReactor (true) ;


	if ( acrxServiceIsRegistered (ADCGCUSTOBJ_DBXSERVICE) == 0 ) {
		if ( !acrxLoadModule ("AdcgCustObj.dbx", false, true) ) {
			acutPrintf ("DBX module not found! Sorry...\n") ;
		}
	}

}

// Unload this application. Unregister all objects
// registered in InitApplication.
void UnloadApplication()
{
	// NOTE: DO NOT edit the following lines.
	//{{AFX_ARX_EXIT
	acedRegCmds->removeGroup("ADCG");
	//}}AFX_ARX_EXIT

	// TODO: clean up your application
}

// This functions registers an ARX command.
// It can be used to read the localized command name
// from a string table stored in the resources.
void AddCommand(const char* cmdGroup, const char* cmdInt, const char* cmdLoc,
				const int cmdFlags, const AcRxFunctionPtr cmdProc, const int idLocal)
{
	char cmdLocRes[65];

	// If idLocal is not -1, it's treated as an ID for
	// a string stored in the resources.
	if (idLocal != -1) {
		HMODULE hModule = GetModuleHandle("AdcgArxCloning.arx");

		// Load strings from the string table and register the command.
		::LoadString(hModule, idLocal, cmdLocRes, 64);
		acedRegCmds->addCommand(cmdGroup, cmdInt, cmdLocRes, cmdFlags, cmdProc);

	} else
		// idLocal is -1, so the 'hard coded'
		// localized function name is used.
		acedRegCmds->addCommand(cmdGroup, cmdInt, cmdLoc, cmdFlags, cmdProc);
}
