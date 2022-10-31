@echo off
FOR /F "usebackq tokens=1,* delims==" %%i IN ("%CD%\activate.bat.env") DO (
    CALL SET "%%i=%%j"
)
