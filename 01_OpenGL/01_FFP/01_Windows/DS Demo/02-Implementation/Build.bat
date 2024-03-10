@echo off
cls

 

if not exist "bin" (
	mkdir "bin"
	echo bin folder created successfully!
) else (
	echo bin folder already exist!
)

echo ========================================= cleaning up

del OGLMain.exe
del ".\\bin\\OGLMain.res"
del ".\\bin\\*.obj"

echo =========================================== Compilation started

@REM Compiling all available cpp files in the directory

for /r %%i in (*.cpp) do (
	echo "%%i"
	cl.exe /Fo".\\bin\\" /c /EHsc /I "C:\Program Files (x86)\OpenAL 1.1 SDK\include" "%%i" || goto :error
)
echo ============================================ Compilation completed

rc.exe /Fo".\\bin\\OGLMain.res" OGLMain.rc || exit /b %errorlevel% 

echo =============================================Linking starts

link.exe /out:OGLmain.exe ./bin/*.obj ./bin/*.res /LIBPATH:"C:\Program Files (x86)\OpenAL 1.1 SDK\libs\Win64" user32.lib gdi32.lib /SUBSYSTEM:WINDOWS || exit /b %errorlevel%

echo ============================================Linking ends

OGLMain.exe
goto :eof

:error
echo failed with error #%errorlevel%
exit /b %errorlevel%

:eof