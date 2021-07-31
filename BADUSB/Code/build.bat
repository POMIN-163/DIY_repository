@echo off

set UV=%1
set UV_PROJ_PATH=%2
echo Init building
echo Building ... > %cd%\build_log.txt
%UV% -j0 -b %UV_PROJ_PATH% -o %cd%\build_log.txt
type build_log.txt
copy Out\badusb.hex badusb.hex
echo Done.
exit
