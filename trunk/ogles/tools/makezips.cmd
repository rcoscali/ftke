@echo off

REM ==========================================================================
REM
REM makezips		Create Distribution package for a specific platform
REM
REM --------------------------------------------------------------------------
REM
REM 12-16-2003		Hans-Martin Will	initial version
REM
REM --------------------------------------------------------------------------
REM
REM Usage:
REM
REM makezips.cmd	<root-folder> <platform> <version number>
REM
REM
REM --------------------------------------------------------------------------
REM
REM Copyright (c) 2004, Hans-Martin Will. All rights reserved.
REM 
REM Redistribution and use in source and binary forms, with or without 
REM modification, are permitted provided that the following conditions are 
REM met:
REM 
REM	 *  Redistributions of source code must retain the above copyright
REM 	notice, this list of conditions and the following disclaimer. 
REM  *	Redistributions in binary form must reproduce the above copyright
REM 	notice, this list of conditions and the following disclaimer in the 
REM 	documentation and/or other materials provided with the distribution. 
REM 
REM THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
REM AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
REM IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
REM ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
REM LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, 
REM OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
REM SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
REM INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
REM CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
REM ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
REM THE POSSIBILITY OF SUCH DAMAGE.
REM
REM ==========================================================================


set ROOT=%1
set PLATFORM=%2
set VERSION=%3

call makedist %ROOT% %PLATFORM% Debug
call makedist %ROOT% %PLATFORM% Release

set GLES_DIST=%ROOT%\dist
set GLES_DIST_PACKAGE=%ROOT%\dist\%PLATFORM%
set ZIP_FILE=%GLES_DIST%\ogl-es-%PLATFORM%-%VERSION%.zip

echo "GLES_DIST=" %GLES_DIST%
echo "ZIP_FILE=" %ZIP_FILE%

del /s /q %ZIP_FILE%
pkzipc -add -recurse -directories=relative %ZIP_FILE% %GLES_DIST_PACKAGE%\*
