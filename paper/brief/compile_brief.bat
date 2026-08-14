@echo off
setlocal enabledelayedexpansion

echo ==========================================
echo        MANAR 2-PAGE BRIEF COMPILER
echo ==========================================

set "MIKTEX_BIN=C:\Users\omarb\AppData\Local\Programs\MiKTeX\miktex\bin\x64"
if exist "%MIKTEX_BIN%\pdflatex.exe" (
    set "PATH=%MIKTEX_BIN%;%PATH%"
)

echo Pass 1: Running pdflatex...
pdflatex -interaction=nonstopmode brief.tex
if errorlevel 1 goto error

echo Pass 2: Finalizing cross-references...
pdflatex -interaction=nonstopmode brief.tex
if errorlevel 1 goto error

echo.
echo ==========================================
echo       BRIEF BUILD SUCCESSFUL
echo       Generated: paper\brief\brief.pdf
echo ==========================================
goto end

:error
echo.
echo ==========================================
echo       BRIEF BUILD FAILED
echo       Check paper\brief\brief.log for details
echo ==========================================
exit /b 1

:end
endlocal
