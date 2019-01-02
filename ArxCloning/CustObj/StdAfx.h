// StdAfx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__AF6300E2_01F0_11D3_8D57_0060B0CDE437__INCLUDED_)
#define AFX_STDAFX_H__AF6300E2_01F0_11D3_8D57_0060B0CDE437__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

extern HINSTANCE _hdllInstance ;

// RX Includes
#include "rxdebug.h"
#include "acdb.h"               // acdb definitions
#include "rxregsvc.h"           // ARX linker
#include "dbapserv.h"           // Host application services
#include "wtypes.h"             // Window definitions.

#include "dbidmap.h"
#include "dbobjptr.h"

#ifdef _DEBUG
#undef _DEBUG
#include <map>
#include <list>
#define _DEBUG
#else
#include <map>
#include <list>
#endif

#endif // !defined(AFX_STDAFX_H__AF6300E2_01F0_11D3_8D57_0060B0CDE437__INCLUDED)
