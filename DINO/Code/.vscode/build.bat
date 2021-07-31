@echo off

set UV=%1
set UV_PROJ_PATH=%2
echo Init building
echo Building ... > %UV_PROJ_PATH%\..\build_log.txt
%UV% -j0 -b %UV_PROJ_PATH% -o %UV_PROJ_PATH%\..\build_log.txt
type build_log.txt
echo Init downloading 
echo Downloading ... > %UV_PROJ_PATH%\..\download_log.txt
%UV% -j0 -f %UV_PROJ_PATH% -o %UV_PROJ_PATH%\..\download_log.txt
type download_log.txt
echo Done.
exit
