# Microsoft Developer Studio Project File - Name="OPLine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=OPLine - Win32 AcadDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OPLine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OPLine.mak" CFG="OPLine - Win32 AcadDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OPLine - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OPLine - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OPLine - Win32 AcadDebug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OPLine - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Oy- /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_OPLINE_" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Release/AsdkOPLine.dbx"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 rxapi.lib acrx15.lib acdb15.lib acutil15.lib acge15.lib acgiapi.lib achapi15.lib acISMobj15.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../AsdkOPLine.dbx"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "OPLine - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_OPLINE_" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug/AsdkOPLine.dbx" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 rxapi.lib acrx15.lib acdb15.lib acutil15.lib acge15.lib acgiapi.lib achapi15.lib acISMobj15.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../AsdkOPLine.dbx" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "OPLine - Win32 AcadDebug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "OPLine___Win32_AcadDebug"
# PROP BASE Intermediate_Dir "OPLine___Win32_AcadDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "OPLine___Win32_AcadDebug"
# PROP Intermediate_Dir "OPLine___Win32_AcadDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_OPLINE_" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_OPLINE_" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "ACADDEBUG" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 rxapi.lib acrx15.lib acdb15.lib acutil15.lib acge15.lib acgiapi.lib achapi15.lib acISMobj15.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../AsdkOPLine.dbx" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 rxapi.lib acrx15.lib acdb15.lib acutil15.lib acge15d.lib acgiapi.lib achapi15.lib acISMobj15.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../AsdkOPLine.dbx" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "OPLine - Win32 Release"
# Name "OPLine - Win32 Debug"
# Name "OPLine - Win32 AcadDebug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AsdkOPLineReactor.cpp
# End Source File
# Begin Source File

SOURCE=.\AsdkOPLineStyle.cpp
# End Source File
# Begin Source File

SOURCE=.\OPLine.cpp
# End Source File
# Begin Source File

SOURCE=.\OPLine.def
# End Source File
# Begin Source File

SOURCE=.\OPLine.rc
# End Source File
# Begin Source File

SOURCE=.\rRxEventReactor.cpp
# End Source File
# Begin Source File

SOURCE=.\rxdebug.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AsdkOPLineReactor.h
# End Source File
# Begin Source File

SOURCE=.\AsdkOPLineStyle.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\rRxEventReactor.h
# End Source File
# Begin Source File

SOURCE=.\rxdebug.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StdArx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\OPLine.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ObjectARX.prj
# End Source File
# End Target
# End Project
