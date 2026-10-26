@echo off
setlocal
if "%~1"=="" goto usage
set "MAFIAMP_ARCH=%~2"
if not "%MAFIAMP_ARCH%"=="32" if not "%MAFIAMP_ARCH%"=="64" goto usage
set "MAFIAMP_TARGET=x64"
set "MAFIAMP_VCVARS=amd64"
set "MAFIAMP_SUFFIX=-64"
if "%MAFIAMP_ARCH%"=="32" (
    set "MAFIAMP_TARGET=x86"
    set "MAFIAMP_VCVARS=amd64_x86"
    set "MAFIAMP_SUFFIX="
)
set "MAFIAMP_CONFIG=%~3"
if not defined MAFIAMP_CONFIG set "MAFIAMP_CONFIG=Debug"
set "MAFIAMP_BUILD=build%MAFIAMP_SUFFIX%"
if "%MAFIAMP_CONFIG%"=="Release" set "MAFIAMP_BUILD=build-prod%MAFIAMP_SUFFIX%"
if "%MAFIAMP_CONFIG%"=="RelWithDebInfo" set "MAFIAMP_BUILD=build-release%MAFIAMP_SUFFIX%"
if not "%MAFIAMP_CONFIG%"=="Debug" if not "%MAFIAMP_CONFIG%"=="Release" if not "%MAFIAMP_CONFIG%"=="RelWithDebInfo" goto usage
if not defined VSCMD_ARG_TGT_ARCH (
    call :setup_msvc
    if errorlevel 1 exit /b 1
)
if not "%VSCMD_ARG_TGT_ARCH%"=="%MAFIAMP_TARGET%" (
    echo An %MAFIAMP_TARGET% compiler environment is required; detected "%VSCMD_ARG_TGT_ARCH%".
    exit /b 1
)
pushd "%~dp0.."
chcp 65001 >nul
rem Where the Framework comes from (cmake/FrameworkBootstrap.cmake): the pinned SDK release by
rem default, FW_SDK_ROOT for a locally installed SDK, FW_SOURCE_DIR to build it from a checkout.
rem Both are cache entries, so switching modes clears the other one instead of letting a stale
rem value win in an existing build tree. The pinned archive is shared by the 32- and 64-bit trees
rem (it carries both architectures) unless FW_SDK_CACHE_DIR points elsewhere.
if not defined FW_SDK_CACHE_DIR set "FW_SDK_CACHE_DIR=%CD%/builds/framework-sdk"
set "MAFIAMP_FRAMEWORK_ARGS=-UFW_SDK_ROOT -UFW_SOURCE_DIR"
if defined FW_SDK_ROOT set "MAFIAMP_FRAMEWORK_ARGS=-DFW_SDK_ROOT=%FW_SDK_ROOT% -UFW_SOURCE_DIR"
if defined FW_SOURCE_DIR set "MAFIAMP_FRAMEWORK_ARGS=-DFW_SOURCE_DIR=%FW_SOURCE_DIR% -UFW_SDK_ROOT"
cmake -S . -B "builds\%MAFIAMP_BUILD%" -G Ninja -DCMAKE_BUILD_TYPE=%MAFIAMP_CONFIG% -DBUILD_SHARED_LIBS=OFF -DCMAKE_MAKE_PROGRAM="%CD%\scripts\ninja_utf8.cmd" -DCMAKE_PROJECT_INCLUDE_BEFORE="%CD%\cmake\MSVCUtf8.cmake" %MAFIAMP_FRAMEWORK_ARGS% %MAFIAMP_CMAKE_ARGS%
if errorlevel 1 goto failed
cmake --build "builds\%MAFIAMP_BUILD%" --target "%~1" --parallel %CMAKE_BUILD_PARALLEL_LEVEL%
if errorlevel 1 goto failed
popd
exit /b 0
:failed
popd
exit /b 1
:usage
echo Usage: builds\build.bat ^<target^> ^<32^|64^> [Debug^|Release^|RelWithDebInfo]
echo   FW_SDK_ROOT=^<dir^>    build against a locally installed Framework SDK
echo   FW_SOURCE_DIR=^<dir^>  build the Framework from a checkout as a subdirectory
exit /b 2
:setup_msvc
set "MAFIAMP_VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%MAFIAMP_VSWHERE%" (
    echo Visual Studio Build Tools not found. Use an x64 developer prompt.
    exit /b 1
)
for /f "usebackq tokens=*" %%i in (`"%MAFIAMP_VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "MAFIAMP_VS_ROOT=%%i"
if not defined MAFIAMP_VS_ROOT exit /b 1
call "%MAFIAMP_VS_ROOT%\VC\Auxiliary\Build\vcvarsall.bat" %MAFIAMP_VCVARS%
exit /b %errorlevel%
