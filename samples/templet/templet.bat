if "%1" == "" goto demo

if %1 == dbg ..\..\..\preprocessor\bin\templet.exe templet.cpp -r TEMPLET_DBG -i dbg/tet.h
if %1 == win ..\..\..\preprocessor\bin\templet.exe templet.cpp -r TEMPLET_WIN -i win/tet.h
if %1 == nix ..\..\..\preprocessor\bin\templet.exe templet.cpp -r TEMPLET_NIX -i nix/tet.h
exit

:demo
call ..\..\..\preprocessor\bin\templet.exe templet.cpp -d -r TEMPLET_DBG -i dbg/tet.h