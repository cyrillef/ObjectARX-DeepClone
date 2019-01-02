//
// StdArx.h : include file for ObjectARX/DBX include files
// this file is only included once by your stdafx.h

#if !defined(AFX_STDARX_H__0377EC93_FE15_11D2_8D57_0060B0CDE437__INCLUDED_)
#define AFX_STDARX_H__0377EC93_FE15_11D2_8D57_0060B0CDE437__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//{{AFX_ARX_INC_SELECTED
#include "dbsymtb.h"          // symboltables
#include "geassign.h"         // AcGe classes
#include "dbents.h"           // line
//}}AFX_ARX_INC_SELECTED
#include "dbidmap.h"

//{{AFX_ARX_INC_OTHERS
#include "rEditorReactor.h"
//}}AFX_ARX_INC_OTHERS

#include "AdskDMgr.h"         // Utility class for document data
#include "docdata.h"          // Your document specific data

// Declare it as an extern here so that it
// becomes available in all modules
extern AsdkDataManager<CDocData> DocVars;

//{{AFX_ARX_FUNC
void adcgclone1();
void adcgclone2();
void adcgclone3();
void adcgdclone1();
void adcgdclone2();
void adcgdclone3();
void adcgdclone4();
void adcgdclone5();
void adcgcrstruct();
void adcgqstruct();
void adcgcstruct();
//}}AFX_ARX_FUNC

// TODO: Here you can add your own includes / declarations
#endif 
