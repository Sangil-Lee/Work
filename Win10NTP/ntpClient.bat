@echo off

net start w32time

w32tm /config /syncfromflags:manual /manualpeerlist:"%*" /reliable:yes /update

reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\W32Time\TimeProviders\NtpClient" /v "SpecialPollInterval" /t REG_DWORD /d 600 /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\W32Time\Config" /v "MaxPosPhaseCorrection" /t REG_DWORD /d 600 /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\W32Time\Config" /v "MaxNegPhaseCorrection" /t REG_DWORD /d 600 /f

net stop w32time
net start w32time
