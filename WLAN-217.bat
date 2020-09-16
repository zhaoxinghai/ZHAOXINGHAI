netsh interface ip set address name="WLAN" source=static addr=192.168.1.217 mask=255.255.255.0 gateway=192.168.1.1
netsh interface ip set dns name="WLAN" source=static addr=192.168.1.1