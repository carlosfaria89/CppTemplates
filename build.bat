@echo off

set VCPKG_ROOT=%VCPKG_ROOT%
set PROJECT_ROOT=%CD%

:: Check if the environment variable is set
IF NOT DEFINED VCPKG_ROOT (
    echo ERROR: The environment variable VCPKG_ROOT is not set.
    echo Download and install VCPKG from https://github.com/microsoft/vcpkg#quick-start-windows.
    echo Create an environment variable VCPKG_ROOT that points to the installation directory.
    exit /b 1
)

:: Install GTest using Vcpkg
"%VCPKG_ROOT%\vcpkg.exe" install gtest:x64-windows

:: Using MSBuild may require elevation
"%VCPKG_ROOT%\vcpkg.exe" integrate install

:: Run CMake
cmake -S "%PROJECT_ROOT%" -B "%PROJECT_ROOT%\build" -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake"

:: Optionally, you can add more CMake configuration here if needed

:: Build your project using the generated build files (replace 'YourProject.sln' with your actual solution file)
cmake --build "%PROJECT_ROOT%\build" --config Release

:: Optionally, you can run your executable here if applicable