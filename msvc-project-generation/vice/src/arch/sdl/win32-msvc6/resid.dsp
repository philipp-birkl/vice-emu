# Microsoft Developer Studio Project File - Name="resid" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=resid - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "resid.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "resid.mak" CFG="resid - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "resid - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "resid - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "resid - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "libs\resid\Release"
# PROP Intermediate_Dir "libs\resid\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"resid\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\" /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"resid\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "WIN32" /d "IDE_COMPILE"
# ADD RSC /l 0x409 /i "..\msvc" /i "..\\" /i "..\..\..\\" /d "NDEBUG" /d "WIN32" /d "IDE_COMPILE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "resid - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "libs\resid\Debug"
# PROP Intermediate_Dir "libs\resid\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Z7 /Od  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"resid\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I ".\" /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"resid\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /YX /FD /c
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

# Name "resid - Win32 Release"
# Name "resid - Win32 Debug"
# Begin Source File

SOURCE="..\..\..\resid\dac.cc"

!IF  "$(CFG)" == "resid - Win32 Release"

# Begin Custom Build
InputPath="..\..\..\resid\dac.cc"
InputName=dac

"libs\resid\Release\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MD /W3 /GX /O2 /EHsc /I ".\\" /I "..\\"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\resid\Release\resid.pch" /Fo"libs\resid\Release\\" /Fd"libs\resid\Release\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "resid - Win32 Debug"

# Begin Custom Build
InputPath="..\..\..\resid\dac.cc"
InputName=dac

"libs\resid\Debug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MDd /W3 /GX /Z7 /Od /EHsc /I ".\\" /I "..\\"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\resid\Debug\resid.pch" /Fo"libs\resid\Debug\\" /Fd"libs\resid\Debug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ENDIF

# End Source File
# Begin Source File

SOURCE="..\..\..\resid\envelope.cc"

!IF  "$(CFG)" == "resid - Win32 Release"

# Begin Custom Build
InputPath="..\..\..\resid\envelope.cc"
InputName=envelope

"libs\resid\Release\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MD /W3 /GX /O2 /EHsc /I ".\\" /I "..\\"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\resid\Release\resid.pch" /Fo"libs\resid\Release\\" /Fd"libs\resid\Release\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "resid - Win32 Debug"

# Begin Custom Build
InputPath="..\..\..\resid\envelope.cc"
InputName=envelope

"libs\resid\Debug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MDd /W3 /GX /Z7 /Od /EHsc /I ".\\" /I "..\\"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\resid\Debug\resid.pch" /Fo"libs\resid\Debug\\" /Fd"libs\resid\Debug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ENDIF

# End Source File
# Begin Source File

SOURCE="..\..\..\resid\extfilt.cc"

!IF  "$(CFG)" == "resid - Win32 Release"

# Begin Custom Build
InputPath="..\..\..\resid\extfilt.cc"
InputName=extfilt

"libs\resid\Release\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MD /W3 /GX /O2 /EHsc /I ".\\" /I "..\\"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\resid\Release\resid.pch" /Fo"libs\resid\Release\\" /Fd"libs\resid\Release\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "resid - Win32 Debug"

# Begin Custom Build
InputPath="..\..\..\resid\extfilt.cc"
InputName=extfilt

"libs\resid\Debug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MDd /W3 /GX /Z7 /Od /EHsc /I ".\\" /I "..\\"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\resid\Debug\resid.pch" /Fo"libs\resid\Debug\\" /Fd"libs\resid\Debug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ENDIF

# End Source File
# Begin Source File

SOURCE="..\..\..\resid\filter.cc"

!IF  "$(CFG)" == "resid - Win32 Release"

# Begin Custom Build
InputPath="..\..\..\resid\filter.cc"
InputName=filter

"libs\resid\Release\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MD /W3 /GX /O2 /EHsc /I ".\\" /I "..\\"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\resid\Release\resid.pch" /Fo"libs\resid\Release\\" /Fd"libs\resid\Release\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "resid - Win32 Debug"

# Begin Custom Build
InputPath="..\..\..\resid\filter.cc"
InputName=filter

"libs\resid\Debug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MDd /W3 /GX /Z7 /Od /EHsc /I ".\\" /I "..\\"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\resid\Debug\resid.pch" /Fo"libs\resid\Debug\\" /Fd"libs\resid\Debug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ENDIF

# End Source File
# Begin Source File

SOURCE="..\..\..\resid\pot.cc"

!IF  "$(CFG)" == "resid - Win32 Release"

# Begin Custom Build
InputPath="..\..\..\resid\pot.cc"
InputName=pot

"libs\resid\Release\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MD /W3 /GX /O2 /EHsc /I ".\\" /I "..\\"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\resid\Release\resid.pch" /Fo"libs\resid\Release\\" /Fd"libs\resid\Release\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "resid - Win32 Debug"

# Begin Custom Build
InputPath="..\..\..\resid\pot.cc"
InputName=pot

"libs\resid\Debug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MDd /W3 /GX /Z7 /Od /EHsc /I ".\\" /I "..\\"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\resid\Debug\resid.pch" /Fo"libs\resid\Debug\\" /Fd"libs\resid\Debug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ENDIF

# End Source File
# Begin Source File

SOURCE="..\..\..\resid\sid.cc"

!IF  "$(CFG)" == "resid - Win32 Release"

# Begin Custom Build
InputPath="..\..\..\resid\sid.cc"
InputName=sid

"libs\resid\Release\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MD /W3 /GX /O2 /EHsc /I ".\\" /I "..\\"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\resid\Release\resid.pch" /Fo"libs\resid\Release\\" /Fd"libs\resid\Release\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "resid - Win32 Debug"

# Begin Custom Build
InputPath="..\..\..\resid\sid.cc"
InputName=sid

"libs\resid\Debug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MDd /W3 /GX /Z7 /Od /EHsc /I ".\\" /I "..\\"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\resid\Debug\resid.pch" /Fo"libs\resid\Debug\\" /Fd"libs\resid\Debug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ENDIF

# End Source File
# Begin Source File

SOURCE="..\..\..\resid\version.cc"

!IF  "$(CFG)" == "resid - Win32 Release"

# Begin Custom Build
InputPath="..\..\..\resid\version.cc"
InputName=version

"libs\resid\Release\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MD /W3 /GX /O2 /EHsc /I ".\\" /I "..\\"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\resid\Release\resid.pch" /Fo"libs\resid\Release\\" /Fd"libs\resid\Release\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "resid - Win32 Debug"

# Begin Custom Build
InputPath="..\..\..\resid\version.cc"
InputName=version

"libs\resid\Debug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MDd /W3 /GX /Z7 /Od /EHsc /I ".\\" /I "..\\"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\resid\Debug\resid.pch" /Fo"libs\resid\Debug\\" /Fd"libs\resid\Debug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ENDIF

# End Source File
# Begin Source File

SOURCE="..\..\..\resid\voice.cc"

!IF  "$(CFG)" == "resid - Win32 Release"

# Begin Custom Build
InputPath="..\..\..\resid\voice.cc"
InputName=voice

"libs\resid\Release\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MD /W3 /GX /O2 /EHsc /I ".\\" /I "..\\"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\resid\Release\resid.pch" /Fo"libs\resid\Release\\" /Fd"libs\resid\Release\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "resid - Win32 Debug"

# Begin Custom Build
InputPath="..\..\..\resid\voice.cc"
InputName=voice

"libs\resid\Debug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MDd /W3 /GX /Z7 /Od /EHsc /I ".\\" /I "..\\"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\resid\Debug\resid.pch" /Fo"libs\resid\Debug\\" /Fd"libs\resid\Debug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ENDIF

# End Source File
# Begin Source File

SOURCE="..\..\..\resid\wave.cc"

!IF  "$(CFG)" == "resid - Win32 Release"

# Begin Custom Build
InputPath="..\..\..\resid\wave.cc"
InputName=wave

"libs\resid\Release\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MD /W3 /GX /O2 /EHsc /I ".\\" /I "..\\"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H"  /D "NDEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\resid\Release\resid.pch" /Fo"libs\resid\Release\\" /Fd"libs\resid\Release\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ELSEIF  "$(CFG)" == "resid - Win32 Debug"

# Begin Custom Build
InputPath="..\..\..\resid\wave.cc"
InputName=wave

"libs\resid\Debug\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /nologo /MDd /W3 /GX /Z7 /Od /EHsc /I ".\\" /I "..\\"  /D "WIN32" /D "_WINDOWS" /D "IDE_COMPILE" /D "DONT_USE_UNISTD_H" /D "_DEBUG" /D PACKAGE=\"%s\" /D VERSION=\"0.7\" /D SIZEOF_INT=4 /Fp"libs\resid\Debug\resid.pch" /Fo"libs\resid\Debug\\" /Fd"libs\resid\Debug\\" /FD /TP /c "$(InputPath)"

# End Custom Build

!ENDIF

# End Source File
# End Target
# End Project
