//
// StdArx.h : include file for ObjectARX/DBX include files
// this file is only included once by your stdafx.h

#if !defined(AFX_STDARX_H__3EE69F11_9E7B_11D3_8DF7_0060B0CDE437__INCLUDED_)
#define AFX_STDARX_H__3EE69F11_9E7B_11D3_8DF7_0060B0CDE437__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//{{AFX_ARX_INC_SELECTED
//}}AFX_ARX_INC_SELECTED

//{{AFX_ARX_INC_OTHERS
#include "rEditorReactor.h"
//}}AFX_ARX_INC_OTHERS
#include "AdskDMgr.h"         // Utility class for document data
#include "docdata.h"          // Your document specific data

// Declare it as an extern here so that it
// becomes available in all modules
extern AsdkDataManager<CDocData> DocVars;
//{{AFX_ARX_FUNC
void Asdkopl();
void Asdkopls();
//}}AFX_ARX_FUNC

// TODO: Here you can add your own includes / declarations
#include "dbsymutl.h"
#include <dbents.h>
#include <dbfiler.h>
#include <rxboiler.h>

#include "../OPLine/AsdkOPLineStyle.h"
#include "../OPLine/AsdkOPLine.h"
#include "AsdkOPLineFiler.h"

#endif 
