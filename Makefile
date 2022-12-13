# Задаем имя проекта, в результате получатся файлы test.hex test.bin
TARG=dali_uart
 
CC = arm-none-eabi-gcc 
OBJCOPY = arm-none-eabi-objcopy
 
# Задаем из каких файлов собирать проект, можно указать несколько файлов
SRCS= main.c
 
OBJS = $(SRCS:.c=.o)

# Задаем для какого микроконтроллера будем компилировать (atmega8)
TARGETS=stm32/f1
 
# Флаги компилятора, при помощи F_CPU определяем частоту на которой будет работать контроллер,
LDFLAGS= -L./libopencm3/lib/ -lopencm3_stm32f1 -Tstm32f1.ld --specs=nosys.specs -nostartfiles -mcpu=cortex-m3 -mthumb
CFLAGS=-I./libopencm3/include/ -mcpu=cortex-m3 -mthumb  -DSTM32F1
 
LIBOPENCM3_LIB=./libopencm3/lib/libopencm3_stm32f1.a


all: $(LIBOPENCM3_LIB) $(TARG)

$(LIBOPENCM3_LIB):
	TARGETS=${TARGETS} $(MAKE) -C libopencm3

libopencm3_clean:
	TARGETS=${TARGETS} $(MAKE) clean -C libopencm3

$(TARG): $(OBJS)
	$(CC) $(OBJS) -o $@.elf $(LDFLAGS)
	$(OBJCOPY) -O binary $@.elf $@.bin
	$(OBJCOPY) -O ihex $@.elf $@.hex
 
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^
 
clean:
	rm -f *.elf *.bin *.hex  $(OBJS) *.map

.PHONY: $(LIBOPENCM3_LIB)
