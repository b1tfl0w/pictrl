Control raspberry Pi by using an ssd1306 oled display and an endless encoder. 

ssd1306 library:
https://github.com/stealthylabs/libssd1306

ky-040 instructions:

"CLK" -> GPIO23

"DT" -> GPIO24

"SW" -> GPIO22

"+" -> 3V3

"GND" -> GND

add these lines in /boot/config.txt and reboot.

dtoverlay=rotary-encoder,pin_a=23,pin_b=24,relative_axis=1

dtoverlay=gpio-key,gpio=22,keycode=28,label="ENTER"

ToDo:
Populate menu items from config file instead of hardcoding them.



https://github.com/user-attachments/assets/709a928d-059a-4047-957b-db5cd4790c3c

