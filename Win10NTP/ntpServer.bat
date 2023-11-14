@echo off

net start w32time

reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\W32Time\TimeProviders\NtpServer" /v "Enabled" /t REG_DWORD /d 1 /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\W32Time\Config" /v "AnnounceFlags" /t REG_DWORD /d 5 /f

netsh advfirewall firewall add rule name="MyNTPServer" protocol=UDP dir=in localport=123 action=allow enable=yes

net stop w32time
net start w32time
