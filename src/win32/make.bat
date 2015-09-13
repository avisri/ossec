echo off
echo ###########################################
call :message Making windows agent
echo ###########################################
"C:\MinGW\bin\windres.exe" -i icofile.rc -o icon.o

call :message    ossec-agent [ for before vista os ]
"C:\MinGW\bin\gcc.exe" -o "ossec-agent" -Wall  		    -DARGV0=\"ossec-agent\" -DCLIENT -DWIN32 -DOSSECHIDS 			icon.o os_regex/*.c os_net/*.c os_xml/*.c zlib-1.2.8/*.c config/*.c shared/*.c os_execd/*.c os_crypto/blowfish/*.c os_crypto/md5/*.c os_crypto/sha1/*.c os_crypto/md5_sha1/*.c os_crypto/shared/*.c rootcheck/*.c *.c -I. -Iheaders/ -lwsock32

call :message    ossec-agent{-eventchannel} [ for After windows vista os ]
"C:\MinGW\bin\gcc.exe" -o "ossec-agent-eventchannel" -Wall  -DARGV0=\"ossec-agent\" -DCLIENT -DWIN32 -DOSSECHIDS -DEVENTCHANNEL_SUPPORT icon.o os_regex/*.c os_net/*.c os_xml/*.c zlib-1.2.8/*.c config/*.c shared/*.c os_execd/*.c os_crypto/blowfish/*.c os_crypto/md5/*.c os_crypto/sha1/*.c os_crypto/md5_sha1/*.c os_crypto/shared/*.c rootcheck/*.c *.c -I. -Iheaders/ -lwsock32 -lwevtapi
call :message    ossec-rootcheck
"C:\MinGW\bin\gcc.exe" -o "ossec-rootcheck" -Wall  -DARGV0=\"ossec-rootcheck\" -DCLIENT -DWIN32 icon.o os_regex/*.c os_net/*.c os_xml/*.c config/*.c shared/*.c win_service.c rootcheck/*.c -Iheaders/ -I. -lwsock32
call :message    manage-agents
"C:\MinGW\bin\gcc.exe" -o "manage_agents" -Wall  -DARGV0=\"manage-agents\" -DCLIENT -DWIN32 -DMA os_regex/*.c zlib-1.2.8/*.c os_zlib.c shared/*.c os_crypto/blowfish/*.c os_crypto/md5/*.c os_crypto/shared/*.c addagent/*.c -Iheaders/ -I. -lwsock32 -lshlwapi
call :message    setup-windows
"C:\MinGW\bin\gcc.exe" -o setup-windows -Wall os_regex/*.c -DARGV0=\"setup-windows\" -DCLIENT -DWIN32 win_service.c shared/file_op.c shared/debug_op.c setup/setup-win.c setup/setup-shared.c -Iheaders/ -I. -lwsock32
call :message    setup-syscheck
"C:\MinGW\bin\gcc.exe" -o setup-syscheck -Wall os_regex/*.c os_xml/*.c setup/setup-syscheck.c setup/setup-shared.c -I. -Iheaders/
call :message    setup-iis
"C:\MinGW\bin\gcc.exe" -o setup-iis -Wall os_regex/*.c setup/setup-iis.c -I.
call :message    add-localfile
"C:\MinGW\bin\gcc.exe" -o add-localfile -Wall os_regex/*.c setup/add-localfile.c -I.

call :message "Making External deps : Lua exes [ using  msys make and gcc 32 ]"

cd lua
pwd
./make.bat
cd ..

call :message Making Agent ui exe [ using gcc ] 
cd ui
pwd
./make.bat
cd ..

echo ###########################################
call :message   Validation
echo ###########################################
dir *.exe 

call :message Making Agent msis [ uses nsis ]
makensis ossec-installer.nsi


:: force execution to quit at the end of the "main" logic
EXIT /B %ERRORLEVEL%




:: a function to write to a log file and write to stdout
:message
echo *******************************************
echo ******************************************* 1>&2
ECHO [make.bat][stderr] %* 1>&2
ECHO [make.bat][stdout] %*
echo *******************************************
echo ******************************************* 1>&2
EXIT /B 0
