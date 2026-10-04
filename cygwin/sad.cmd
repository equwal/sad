@echo off
set PATH=C:\cygwin64\bin;%PATH%
C:\cygwin64\bin\sh.exe -c "exec /usr/local/bin/sad -f ${SAD_SOCK:-/tmp/sad-sock}"
