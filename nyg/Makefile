# makefile for AVR compiler
# 14.04.2015


SOURCE_M = t3.c


SOURCE = $(SOURCE_M)

OUT   = t3.elf
HEX  := $(OUT:%.elf=%.hex)

MCU_AVR = atmega8
F_CPU = 1000000

CC       = avr-gcc
WARNING  = -Wall
C_FLAG   = -mmcu=$(MCU_AVR) -DF_CPU=$(F_CPU)UL -Os -std=c11 
LD_FLAG  = 

PROG          = avrdude
AVRDUDE_DEV   = USBasp #avrispmkII 	# stk500V2
AVRDUDE_PORT  = usb:003:06	# Jonex

P_WRITE_FLASH = -U flash:w:$(HEX)
P_FLAGS = -p $(MCU_AVR)  -c $(AVRDUDE_DEV) -P $(AVRDUDE_PORT)




comp: $(SOURCE)
	@echo "-------------------------"
	$(CC) $(C_FLAG) $(LD_FLAG) $(WARNING) $(SOURCE) -o $(OUT) 
	avr-objcopy -O ihex $(OUT) $(HEX)
	avr-size $(HEX)
	

conv: $(OUTPUT)
	avr-objcopy -O ihex $(OUT) $(HEX)	
	avr-size $(HEX)

upload:
	sudo $(PROG)  $(P_FLAGS) $(P_WRITE_FLASH) 		

test:
	sudo $(PROG) -c $(AVRDUDE_DEV) -P $(AVRDUDE_PORT) -p $(MCU_AVR) -F -v -u 

clean:
	/bin/rm -f $(OUT)	


all: $(SOURCE)
	@echo "================"
	$(CC) $(C_FLAG) $(LD_FLAG) $(WARNING) $(SOURCE) -o $(OUT)	
	avr-objcopy -O ihex $(OUT) $(HEX)
	avr-size $(HEX)	
	sudo $(PROG)  $(P_FLAGS) $(P_WRITE_FLASH)
