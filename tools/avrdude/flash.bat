@ echo off
REM Program to flash hex files with avrdude
title Avrdude flash batch files

REM set PATH=\;%PATH%

:loop
REM reset all variables
set com=
set mcu=
set mode=
REM set command=


REM comport selection
:com
set /p com=Please enter the number of the comport:
echo.
IF "%com%"=="" (
 echo Error: Please enter a valid number!
 echo.
 goto com
)


REM MCU selection
:mcu
echo Please enter the name of the MCU you want to use:
set /p mcu=(32u4, 8u2, 16u2, 32u2):
echo.

IF "%mcu%"=="" (
 echo Error: Please enter a valid MCU!
 echo.
 goto mcu
)
IF NOT %mcu%==32u4 IF NOT %mcu%==8u2 IF NOT %mcu%==16u2 IF NOT %mcu%==32u2 (
 echo Error: Please enter a valid MCU!
 echo.
 goto mcu
)


REM Flash/Erase
:flasherase
set /p mode=Do you want to (f)lash or (e)rase? 
echo.

IF %mode%==f (
 set /p hexfile=Please enter the name of the hexfile and put it into the same directory (e.g: firmware.hex)
 echo.
)

IF %mode%==f (
 echo Flashing with the following option:
 set command=avrdude.exe -C avrdude.conf -p Atmega%mcu% -c avr109 -b 57600 -P COM%com% -U flash:w:%hexfile%
) ELSE IF %mode%==e (
 echo Erasing with the following option:
 set command=avrdude.exe -C avrdude.conf -p Atmega%mcu% -c avr109 -b 57600 -P COM%com% -e
) ELSE (
 echo Error: Wrong option entered!
 goto flasherase
)
echo %command%
echo.


REM user verification
:verification
echo Start now? (y/n)
set /p start=


REM Staring operation
IF "%start%"=="" (
 goto verification
)
IF %start%==y (
 echo Now flashing...
 echo.
) ELSE (
 echo Flashing aborted.
 echo.
 goto loop
)

%command%
echo.

goto loop

:end
pause