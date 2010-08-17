@echo off

if "%1"=="" goto help
if "%2"=="" goto help
if "%3"=="" goto help

if "%1"=="s60" goto build_mmp
if "%1"=="uiq" goto build_mmp

:help
echo Usage: make.bat ui platform build
echo ui:
echo   s60 uiq
echo platform:
echo   wins armi
echo build:
echo   udeb urel
goto end

:build_mmp
rem TEST.MMP
rem ---------------------------------------------------------------------------
echo // Auto generated MMP file. Do not edit > test.mmp
echo TARGET		test.app >> test.mmp
echo TARGETTYPE		app >> test.mmp
echo UID		0x100039ce 0x10202DA0 >> test.mmp
if "%1"=="s60" echo MACRO TARGET_S60 >> test.mmp
echo TARGETPATH		\system\apps\test >> test.mmp
echo SOURCEPATH		..\..\test\epoc >> test.mmp
echo SOURCE		test.cpp >> test.mmp
echo SOURCE		app.cpp >> test.mmp
echo SOURCE		doc.cpp >> test.mmp 
echo SOURCE		appview.cpp >> test.mmp 
echo SOURCE		appui.cpp >> test.mmp
echo RESOURCE		test.rss >> test.mmp
echo SOURCE		paint.cpp >> test.mmp
echo SOURCE             stdafx.cpp >> test.mmp
echo USERINCLUDE	. >> test.mmp
echo SYSTEMINCLUDE	..\..\include >> test.mmp
echo SYSTEMINCLUDE	..\..\src >> test.mmp
echo SYSTEMINCLUDE	\epoc32\include\libc >> test.mmp
echo SYSTEMINCLUDE	\epoc32\include >> test.mmp
echo LIBRARY		apparc.lib cone.lib edll.lib euser.lib eikcore.lib fbscli.lib >> test.mmp
if "%1"=="s60" echo LIBRARY	avkon.lib >> test.mmp
if "%1"=="uiq" echo LIBRARY	eikcoctl.lib >> test.mmp
echo LIBRARY		gles_cl.lib >> test.mmp
echo START BITMAP dodge.mbm >> test.mmp
echo     SOURCEPATH ..\..\test\epoc >> test.mmp
echo     SOURCE c24 dodge.bmp >> test.mmp
echo END >> test.mmp

if "%1"=="s60" copy /y ..\..\test\epoc\tests60.rss ..\..\test\epoc\test.rss
if "%1"=="uiq" copy /y ..\..\test\epoc\testuiq.rss ..\..\test\epoc\test.rss

call bldmake bldfiles
call abld export
call abld build %2 %3
call abld test build %2 %3

if "%2"=="wins" goto end

rem create pkg files
rem --------------------------------------------------------------
rem GLES_CL.PKG
echo ;Auto generated PKG files. Do not edit > gles_cl.pkg
echo #{"gles_cl"},(0x10202D9E),1,0,0,TYPE=SISSYSTEM >> gles_cl.pkg
if "%1"=="s60" echo (0x101F6F88), 0, 0, 0, {"Series60ProductID"} >> gles_cl.pkg
if "%1"=="uiq" echo (0x101F617B), 2, 0, 0, {"UIQ20ProductID"} >> gles_cl.pkg
echo "EPOC32\RELEASE\ARMI\UREL\GLES_CL.DLL"-"!:\system\libs\gles_cl.dll" >> gles_cl.pkg

rem TEST.PKG
echo ;Auto generated PKG files. Do not edit > test.pkg
echo #{"test"},(0x10202DA0),1,0,0 >> test.pkg
if "%1"=="s60" echo (0x101F6F88), 0, 0, 0, {"Series60ProductID"} >> test.pkg
if "%1"=="uiq" echo (0x101F617B), 2, 0, 0, {"UIQ20ProductID"} >> test.pkg

echo "EPOC32\RELEASE\ARMI\UREL\TEST.APP"-"!:\system\apps\test\test.app" >> test.pkg
echo "EPOC32\DATA\Z\SYSTEM\APPS\TEST\TEST.RSC"-"!:\system\apps\test\test.rsc" >> test.pkg
echo "EPOC32\DATA\Z\SYSTEM\APPS\TEST\DODGE.MBM"-"!:\system\apps\test\dodge.mbm" >> test.pkg
echo @"gles_cl.sis",(0x10202D9E) >> test.pkg

rem build SIS files
rem --------------------------------------------------------------
call makesis -d%EPOCROOT% gles_cl.pkg
call makesis -d%EPOCROOT% test.pkg

:end