# Microsoft Developer Studio Project File - Name="x128" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=x128 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "x128.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "x128.mak" CFG="x128 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "x128 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "x128 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "x128 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\data"
# PROP Intermediate_Dir "libs\x128\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I ".\msvc" /I ".\\" /I "..\..\\" /I "..\..\drive" /I "..\..\vdrive" /I "..\..\monitor" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i ".\MSVC" /i ".\\" /i "..\..\\" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib comctl32.lib winmm.lib ddraw.lib dsound.lib ole32.lib dxguid.lib wsock32.lib version.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "x128 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\data"
# PROP Intermediate_Dir "libs\x128\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I ".\msvc" /I ".\\" /I "..\..\\" /I "..\..\drive" /I "..\..\vdrive" /I "..\..\monitor" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i ".\MSVC" /i ".\\" /i "..\..\\" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dxguid.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib comctl32.lib winmm.lib ddraw.lib dsound.lib ole32.lib wsock32.lib version.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "x128 - Win32 Release"
# Name "x128 - Win32 Debug"
# Begin Source File

SOURCE=..\..\main.c
# End Source File
# Begin Source File

SOURCE=.\resc128.rc

!IF  "$(CFG)" == "x128 - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__RESC1="..\..\debug.h"	"resc128.rc"	"res.rc"	"resacia.rc"	"reside64.rc"	"resreu.rc"	"resgeoram.rc"	"resramcart.rc"	"resmmc64.rc"	"resrs232user.rc"	"ressid.rc"	"resvicii.rc"	"resdrivec128.rc"	"resdigimax.rc"	"resmouse.rc"	"resmidi.rc"	"restfe.rc"	
# Begin Custom Build
InputPath=.\resc128.rc

"resc128cat.rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy /b resc128.rc + res.rc + resacia.rc + reside64.rc + resreu.rc + resgeoram.rc + resramcart.rc + resmmc64.rc + resrs232user.rc + ressid.rc + resvicii.rc + resdrivec128.rc + resdigimax.rc + resmouse.rc + resmidi.rc + restfe.rc resc128cat.rc /b

# End Custom Build

!ELSEIF  "$(CFG)" == "x128 - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RESC1="..\..\debug.h"	"resc128.rc"	"res.rc"	"resacia.rc"	"reside64.rc"	"resreu.rc"	"resgeoram.rc"	"resramcart.rc"	"resmmc64.rc"	"resrs232user.rc"	"ressid.rc"	"resvicii.rc"	"resdrivec128.rc"	"resdigimax.rc"	"resmouse.rc"	"resmidi.rc"	"restfe.rc"	
# Begin Custom Build
InputPath=.\resc128.rc

"resc128cat.rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy /b resc128.rc + res.rc + resacia.rc + reside64.rc + resreu.rc + resgeoram.rc + resramcart.rc + resmmc64.rc + resrs232user.rc + ressid.rc + resvicii.rc + resdrivec128.rc + resdigimax.rc + resmouse.rc + resmidi.rc + restfe.rc resc128cat.rc /b

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resc128cat.rc
# End Source File
# End Target
# End Project
