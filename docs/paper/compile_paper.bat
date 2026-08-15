@echo off
title MANAR Paper Build System
echo ==========================================
echo        MANAR LATEX PAPER COMPILER
echo ==========================================
echo.

cd /d "%~dp0"

set "LATEX_FLAGS=-interaction=nonstopmode -halt-on-error -file-line-error"
set "BUILD_LOG=main.build.log"
del /f /q "%BUILD_LOG%" >nul 2>&1

set "PDFLATEX_PATH=C:\Users\omarb\AppData\Local\Programs\MiKTeX\miktex\bin\x64\pdflatex.exe"
set "BIBTEX_PATH=C:\Users\omarb\AppData\Local\Programs\MiKTeX\miktex\bin\x64\bibtex.exe"

where pdflatex >nul 2>&1
if %errorlevel% equ 0 (
    set "PDFLATEX_CMD=pdflatex"
    set "BIBTEX_CMD=bibtex"
) else if exist "%PDFLATEX_PATH%" (
    set "PDFLATEX_CMD="%PDFLATEX_PATH%""
    set "BIBTEX_CMD="%BIBTEX_PATH%""
) else (
    echo [ERROR] pdflatex not found in PATH or MiKTeX default directory.
    echo Please install MiKTeX or TeX Live.
    pause
    exit /b 1
)

echo Pass 1: Running pdflatex...
%PDFLATEX_CMD% %LATEX_FLAGS% main.tex >"%BUILD_LOG%" 2>&1
if errorlevel 1 goto :build_failed

echo Pass 2: Running bibtex...
%BIBTEX_CMD% main >>"%BUILD_LOG%" 2>&1
if errorlevel 1 goto :build_failed

echo Pass 3: Resolving citations (pdflatex pass 2)...
%PDFLATEX_CMD% %LATEX_FLAGS% main.tex >>"%BUILD_LOG%" 2>&1
if errorlevel 1 goto :build_failed

echo Pass 4: Finalizing cross-references (pdflatex pass 3)...
%PDFLATEX_CMD% %LATEX_FLAGS% main.tex >>"%BUILD_LOG%" 2>&1
if errorlevel 1 goto :build_failed

if not exist main.pdf (
    echo.
    echo ==========================================
    echo       PAPER BUILD FAILED
    echo ==========================================
    echo.
    pause
    exit /b 1
)

echo Cleaning intermediate build artifacts...
del /f /q main.aux main.blg main.out >nul 2>&1

echo.
echo ==========================================
echo       PAPER BUILD SUCCESSFUL
echo       Generated: %~dp0main.pdf
echo ==========================================
echo.
exit /b 0

:build_failed
echo.
echo ==========================================
echo       PAPER BUILD FAILED
echo ==========================================
echo See: %BUILD_LOG%
echo.
type "%BUILD_LOG%"
exit /b 1
