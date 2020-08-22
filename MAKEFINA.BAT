wmake
copy demo.exe test
copy hardcopy.dat test
copy *.xm test
copy pmodew.exe test
cd test
pmwlite demo.exe
copy /b demo.exe+hardcopy.dat demo.exe
del hardcopy.dat
del *.zip
del pmodew.exe
pkzip spd_okie.zip *.*


