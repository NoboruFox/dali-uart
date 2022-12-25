
openocd -f dali-uart.cfg -c "init; reset halt; stm32f1x unlock 0; reset halt; flash write_image erase /home/fox/projects/dali-uart/dali_uart.bin 0x08000000; reset; exit"
