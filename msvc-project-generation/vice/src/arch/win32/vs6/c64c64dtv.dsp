# Microsoft Developer Studio Project File - Name="c64c64dtv" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=c64c64dtv - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "c64c64dtv.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "c64c64dtv.mak" CFG="c64c64dtv - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "c64c64dtv - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "c64c64dtv - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "c64c64dtv - Win32 DX Release" (based on "Win32 (x86) Static Library")
!MESSAGE "c64c64dtv - Win32 DX Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "c64c64dtv - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "libs\c64c64dtv\Release"
# PROP Intermediate_Dir "libs\c64c64dtv\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "NDEBUG" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\msvc" /I "..\\" /I "..\..\..\\" /I "..\..\..\drive" /I "..\..\..\drive\iec\c64exp" /I "..\..\..\lib\p64" /I "..\..\..\monitor" /I "..\..\..\raster" /I "..\..\..\rs232drv" /I "..\..\..\sid" /I "..\..\..\tape" /I "..\..\..\vicii"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "NDEBUG" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "WIN32" /d "IDE_COMPILE" /d "NODIRECTX"
# ADD RSC /l 0x409 /i "..\msvc" /i "..\\" /i "..\..\..\\" /d "NDEBUG" /d "WIN32" /d "IDE_COMPILE" /d "NODIRECTX"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "c64c64dtv - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "libs\c64c64dtv\Debug"
# PROP Intermediate_Dir "libs\c64c64dtv\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "_DEBUG" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "..\msvc" /I "..\\" /I "..\..\..\\" /I "..\..\..\drive" /I "..\..\..\drive\iec\c64exp" /I "..\..\..\lib\p64" /I "..\..\..\monitor" /I "..\..\..\raster" /I "..\..\..\rs232drv" /I "..\..\..\sid" /I "..\..\..\tape" /I "..\..\..\vicii"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "NODIRECTX" /D "_DEBUG" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WIN32" /d "IDE_COMPILE" /d "NODIRECTX"
# ADD RSC /l 0x409 /i "..\msvc" /i "..\\" /i "..\..\..\\" /d "_DEBUG" /d "WIN32" /d "IDE_COMPILE" /d "NODIRECTX"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "c64c64dtv - Win32 DX Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DXRelease"
# PROP BASE Intermediate_Dir "DXRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "libs\c64c64dtv\DXRelease"
# PROP Intermediate_Dir "libs\c64c64dtv\DXRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\msvc" /I "..\\" /I "..\..\..\\" /I "..\..\..\drive" /I "..\..\..\drive\iec\c64exp" /I "..\..\..\lib\p64" /I "..\..\..\monitor" /I "..\..\..\raster" /I "..\..\..\rs232drv" /I "..\..\..\sid" /I "..\..\..\tape" /I "..\..\..\vicii"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "WIN32" /d "IDE_COMPILE"
# ADD RSC /l 0x409 /i "..\msvc" /i "..\\" /i "..\..\..\\" /d "NDEBUG" /d "WIN32" /d "IDE_COMPILE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "c64c64dtv - Win32 DX Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DXDebug"
# PROP BASE Intermediate_Dir "DXDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "libs\c64c64dtv\DXDebug"
# PROP Intermediate_Dir "libs\c64c64dtv\DXDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "..\msvc" /I "..\\" /I "..\..\..\\" /I "..\..\..\drive" /I "..\..\..\drive\iec\c64exp" /I "..\..\..\lib\p64" /I "..\..\..\monitor" /I "..\..\..\raster" /I "..\..\..\rs232drv" /I "..\..\..\sid" /I "..\..\..\tape" /I "..\..\..\vicii"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WIN32" /d "IDE_COMPILE"
# ADD RSC /l 0x409 /i "..\msvc" /i "..\\" /i "..\..\..\\" /d "_DEBUG" /d "WIN32" /d "IDE_COMPILE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF

# Begin Target

# Name "c64c64dtv - Win32 Release"
# Name "c64c64dtv - Win32 Debug"
# Name "c64c64dtv - Win32 DX Release"
# Name "c64c64dtv - Win32 DX Debug"
# Begin Source File

SOURCE="..\..\..\c64\c64bus.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\c64\c64drive.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\c64\c64fastiec.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\c64\c64keyboard.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\c64\c64parallel.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\c64\c64printer.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\c64\c64rom.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\c64\c64romset.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\c64\c64rsuser.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\c64\c64video.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\c64\patchrom.c"
# End Source File
# End Target
# End Project
