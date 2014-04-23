# Microsoft Developer Studio Project File - Name="playuvApp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=playuvApp - Win32 ReleaseDLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "playuvApp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "playuvApp.mak" CFG="playuvApp - Win32 ReleaseDLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "playuvApp - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "playuvApp - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "playuvApp - Win32 DebugDLL" (based on "Win32 (x86) Console Application")
!MESSAGE "playuvApp - Win32 ReleaseDLL" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "W32Release"
# PROP Intermediate_Dir "W32Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G6 /MD /GX /O2 /I "$(WXWIN28)\lib\vc_lib\msw" /I "$(WXWIN28)\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D WINVER=0x400 /D "_MT" /D wxUSE_GUI=1 /FR /FD /c
# ADD BASE RSC /l 0x410 /d "NDEBUG"
# ADD RSC /l 0x410 /i "$(WXWIN28)\include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib wxzlib.lib wxregex.lib wxpng.lib wxjpeg.lib wxbase28.lib wxmsw28_core.lib wxmsw28_html.lib wxmsw28_adv.lib /nologo /subsystem:windows /machine:I386 /out:"W32Release/pyuv.exe" /libpath:"$(WXWIN28)\lib\vc_lib" /IGNORE:4089
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "playuvApp___Win32_Debug"
# PROP BASE Intermediate_Dir "playuvApp___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "W32Debug"
# PROP Intermediate_Dir "W32Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /Gm /GX /Zi /Od /I "$(WXWIN28)\INCLUDE" /I "$(WXWIN28)\lib\vc_lib\msw" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D WINVER=0x400 /D "_MT" /D wxUSE_GUI=1 /D "__WXDEBUG__" /D WXDEBUG=1 /FR /FD /GZ /c
# ADD BASE RSC /l 0x410 /d "_DEBUG"
# ADD RSC /l 0x410 /i "$(WXWIN28)\INCLUDE" /i "$(WXWIN28)\lib\vc_lib\msw" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib wxzlibd.lib wxregexd.lib wxpngd.lib wxjpegd.lib wxtiffd.lib wxbase28d.lib wxmsw28d_core.lib wxmsw28d_html.lib wxmsw28d_adv.lib /nologo /subsystem:windows /debug /machine:I386 /out:"W32Debug/pyuv.exe" /pdbtype:sept /libpath:"$(WXWIN28)\lib\vc_lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "playuvApp___Win32_DebugDLL"
# PROP BASE Intermediate_Dir "playuvApp___Win32_DebugDLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "playuvApp___Win32_DebugDLL"
# PROP Intermediate_Dir "playuvApp___Win32_DebugDLL"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /I "$(WXWIN28)\lib\mswd" /I "$(WXWIN28)\include" /I "$(WXWIN28)\contrib\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D WINVER=0x400 /D "_MT" /D wxUSE_GUI=1 /D "__WXDEBUG__" /D WXDEBUG=1 /D WXUSINGDLL=1 /Fp"DebugDLL/playuvApp.pch" /YX"wxprec.h" /Fo"DebugDLL/" /Fd"DebugDLL/" /FD /GZ /c
# ADD BASE RSC /l 0x410 /d "_DEBUG"
# ADD RSC /l 0x410 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib wxmsw24d.lib /nologo /subsystem:windows /pdb:"DebugDLL/playuvApp.pdb" /debug /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"libcid.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "playuvApp___Win32_ReleaseDLL"
# PROP BASE Intermediate_Dir "playuvApp___Win32_ReleaseDLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "playuvApp___Win32_ReleaseDLL"
# PROP Intermediate_Dir "playuvApp___Win32_ReleaseDLL"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "$(WXWIN28)\lib\mswdll" /I "$(WXWIN28)\include" /I "$(WXWIN28)\contrib\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "NDEBUG" /D "_WINDOWS" /D "__WINDOWS__" /D "__WXMSW__" /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /D WXUSINGDLL=1 /Fo"ReleaseDLL/" /Fd"ReleaseDLL/" /FD /c
# ADD BASE RSC /l 0x410 /d "_DEBUG"
# ADD RSC /l 0x410 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib wxmsw24.lib /nologo /subsystem:windows /incremental:no /pdb:"ReleaseDLL/playuvApp.pdb" /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libci.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "playuvApp - Win32 Release"
# Name "playuvApp - Win32 Debug"
# Name "playuvApp - Win32 DebugDLL"
# Name "playuvApp - Win32 ReleaseDLL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\source\converter.cpp
# End Source File
# Begin Source File

SOURCE=.\source\playuvApp.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\source\converter.h
# End Source File
# Begin Source File

SOURCE=.\source\playuvApp.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\resource\wx\msw\blank.cur
# End Source File
# Begin Source File

SOURCE=.\resource\wx\msw\bullseye.cur
# End Source File
# Begin Source File

SOURCE=.\resource\wx\msw\cdrom.ico
# End Source File
# Begin Source File

SOURCE=.\resource\wx\msw\computer.ico
# End Source File
# Begin Source File

SOURCE=.\resource\wx\msw\cross.cur
# End Source File
# Begin Source File

SOURCE=.\resource\wx\msw\drive.ico
# End Source File
# Begin Source File

SOURCE=.\resource\wx\msw\file1.ico
# End Source File
# Begin Source File

SOURCE=.\resource\wx\msw\floppy.ico
# End Source File
# Begin Source File

SOURCE=.\resource\wx\msw\folder1.ico
# End Source File
# Begin Source File

SOURCE=.\resource\wx\msw\folder2.ico
# End Source File
# Begin Source File

SOURCE=.\resource\wx\msw\hand.cur
# End Source File
# Begin Source File

SOURCE=.\resource\wx\msw\magnif1.cur
# End Source File
# Begin Source File

SOURCE=.\Makefile
# End Source File
# Begin Source File

SOURCE=.\resource\wx\msw\pbrush.cur
# End Source File
# Begin Source File

SOURCE=.\resource\wx\msw\pencil.cur
# End Source File
# Begin Source File

SOURCE=.\resource\playuv.ico
# End Source File
# Begin Source File

SOURCE=.\resource\playuv.rc
# End Source File
# Begin Source File

SOURCE=.\resource\wx\msw\pntleft.cur
# End Source File
# Begin Source File

SOURCE=.\resource\wx\msw\pntright.cur
# End Source File
# Begin Source File

SOURCE=.\resource\wx\msw\removble.ico
# End Source File
# Begin Source File

SOURCE=.\resource\wx\msw\rightarr.cur
# End Source File
# Begin Source File

SOURCE=.\resource\wx\msw\roller.cur
# End Source File
# End Group
# Begin Group "Help"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HTMLHelp\back.gif

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\HTMLHelp\back.gif

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\HTMLHelp.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\HTMLHelp\back.gif

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\HTMLHelp.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HTMLHelp\books.gif

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\HTMLHelp\books.gif

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\HTMLHelp.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\HTMLHelp\books.gif

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\HTMLHelp.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HTMLHelp\contents.gif

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\HTMLHelp\contents.gif

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\HTMLHelp.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\HTMLHelp\contents.gif

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\HTMLHelp.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HTMLHelp\doc.hhc

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\HTMLHelp\doc.hhc

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\HTMLHelp.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\HTMLHelp\doc.hhc

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\HTMLHelp.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doc\doc.hhk

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\HTMLHelp\doc.hhk

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\HTMLHelp.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\HTMLHelp\doc.hhk

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doc\doc.hhp

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\doc.hhp

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\doc.hhp

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doc\doc.htm

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\doc.htm

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\doc.htm

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doc\doc1.htm

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\doc1.htm

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\doc1.htm

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doc\doc2.htm

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\doc2.htm

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\doc2.htm

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doc\doc3.htm

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\doc3.htm

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\doc3.htm

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doc\doc4.htm

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\doc4.htm

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\doc4.htm

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doc\doc5.htm

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\doc5.htm

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\doc5.htm

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doc\forward.gif

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\forward.gif

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\forward.gif

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doc\l32pyuv.jpg

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\l32pyuv.jpg

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\l32pyuv.jpg

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doc\playuv128.gif

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\playuv128.gif

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\playuv128.gif

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doc\subsampling.png

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\subsampling.png

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\subsampling.png

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doc\up.gif

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\up.gif

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\up.gif

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doc\w32formatdialogextra.jpg

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\w32formatdialogextra.jpg

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\w32formatdialogextra.jpg

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doc\w32formatdialogmain.jpg

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\w32formatdialogmain.jpg

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\w32formatdialogmain.jpg

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doc\w32pyuv.jpg

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build
InputPath=.\doc\w32pyuv.jpg

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\w32pyuv.jpg

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\doc\w32syncdialog.jpg

!IF  "$(CFG)" == "playuvApp - Win32 Release"

# Begin Custom Build - Preparing zipped HTML help file
InputPath=.\doc\w32syncdialog.jpg

"$(InputPath).old" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo >$(InputPath).old 
	zip -j .\doc.htb $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "playuvApp - Win32 Debug"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 DebugDLL"

!ELSEIF  "$(CFG)" == "playuvApp - Win32 ReleaseDLL"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
