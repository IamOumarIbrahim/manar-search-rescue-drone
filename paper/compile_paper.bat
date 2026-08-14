@echo off
title MANAR Paper Build System
echo ==========================================
echo        MANAR LATEX PAPER COMPILER
echo ==========================================
echo.

cd /d "%~dp0"

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
%PDFLATEX_CMD% -interaction=nonstopmode main.tex >nul 2>&1

echo Pass 2: Running bibtex...
%BIBTEX_CMD% main >nul 2>&1

echo Pass 3: Resolving citations (pdflatex pass 2)...
%PDFLATEX_CMD% -interaction=nonstopmode main.tex >nul 2>&1

echo Pass 4: Finalizing cross-references (pdflatex pass 3)...
%PDFLATEX_CMD% -interaction=nonstopmode main.tex >nul 2>&1

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
del /f /q main.aux main.bbl main.blg main.log main.out >nul 2>&1

echo.
echo ==========================================
echo       PAPER BUILD SUCCESSFUL
echo       Generated: paper\main.pdf
echo ==========================================
echo.
exit /b 0
