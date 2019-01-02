//////////////////////////////////////////////////////////////////////////////
// AcRxEventReactor reactors, created by  [Nov/06/2002], , 

#if !defined(ARX__RRXEVENTREACTOR_H__20021106_133132)
#define ARX__RRXEVENTREACTOR_H__20021106_133132

//-----------------------------------------------------------------------------
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------------
#include "rxevent.h"

//-----------------------------------------------------------------------------
class AdcgMyDictObjSetAt : public AcRxEventReactor {

public:
	// Constructor / Destructor
	AdcgMyDictObjSetAt (const bool autoInitAndRelease =true) ;
	virtual ~AdcgMyDictObjSetAt () ;

	//{{AFX_ARX_METHODS(AdcgMyDictObjSetAt)
	virtual void endDeepClone(AcDbIdMapping& x0);
	virtual void beginDeepCloneXlation(AcDbIdMapping& x0, Acad::ErrorStatus* x1);
	virtual void beginDeepClone(AcDbDatabase* pTo, AcDbIdMapping& x0);
	//}}AFX_ARX_METHODS

private:
	// Auto initialization and release flag.
	bool m_autoInitAndRelease ;
} ;

//-----------------------------------------------------------------------------
#endif // !defined(ARX__RRXEVENTREACTOR_H__20021106_133132)