//
// StdArx.h : include file for ObjectARX/DBX include files
// this file is only included once by your stdafx.h

#if !defined(AFX_STDARX_H__F5AF4417_A0B0_11D3_8E00_0060B0CDE437__INCLUDED_)
#define AFX_STDARX_H__F5AF4417_A0B0_11D3_8E00_0060B0CDE437__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//{{AFX_ARX_INC_SELECTED
#include "dbdict.h"           // dictionaries
#include "dbsymtb.h"          // symboltables
#include "dbidmap.h"          // ID mapping
#include "dbmain.h"           // Acad base class
#include "dbpl.h"             // polyline
#include "xgraph.h"           // Xref node classes
//}}AFX_ARX_INC_SELECTED

//{{AFX_ARX_INC_OTHERS
#include "AsdkOPLineReactor.h"
#include "AsdkOPLineStyle.h"
#include "rRxEventReactor.h"
//}}AFX_ARX_INC_OTHERS
//{{AFX_ARX_FUNC
//}}AFX_ARX_FUNC

// TODO: Here you can add your own includes / declarations
#include "dbsymutl.h"

#define ARXOK(a) {\
	Acad::ErrorStatus es ;\
	if ( (es =a) != Acad::eOk )\
		return (es) ;\
	}
#define ARXOK2(a) {\
	Acad::ErrorStatus es ;\
	if ( (es =a) != Acad::eOk && es != Acad::eObjectToBeDeleted )\
		return (es) ;\
	}


#endif 
