
CC=avr-gcc
#CFLAGS=--std=gnu99 -O3 -DF_CPU=16000000UL  -funsigned-char -funsigned-bitfields  -fshort-enums -Wall -Wstrict-prototypes -mmcu=atmega328p $(INCLUDE_DIRS)  -D__AVR_3_BYTE_PC__
#CFLAGS=-O3 -DF_CPU=16000000UL -Wall -mmcu=atmega328p

CFLAGS+=-Os -g -DF_CPU=16000000UL -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega328p
CFLAGS+=-DUID=0xFFF2
#CFLAGS+=-w -Os -g -DF_CPU=16000000UL -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega328p


DEFINE_FLAGS=-D__arduino__

AVRDUDE=avrdude

AVRDUDE_PROGRAMMER=arduino
#AVRDUDE_PROGRAMMER=wiring

#avrdude -Pusb -v -p atmega328p -C /home/gio/.platformio/packages/tool-avrdude/avrdude.conf -c usbasp -D -U f
#lash:w:.pio/build/uno/firmware.hex:i

#AVRDUDE_PORT = /dev/ttyACM0    # programmer connected to serial device
AVRDUDE_PORT = /dev/ttyUSB0

#AVRDUDE_FLAGS = -p m328p -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) -b 115200
AVRDUDE_FLAGS = -p m328p -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) -b 57600

#AVRDUDE_FLAGS =-Pusb -v -p m328p -c usbasp -u -b 115200

AVRDUDE_FLAGS += -D -V -v -v -v  -C avrdude.conf


#AVRDUDE_FLAGS += -D -V -C /usr/share/arduino/hardware/tools/avr/../avrdude.conf

#AVRDUDE_FLAGS += -D -q -V -C /usr/share/arduino/hardware/tools/avr/../avrdude.conf
FIRMWARE = main

$(FIRMWARE).elf: Adafruit_NeoPixel.o HNSSocketPower.o EmonLib.o  ../src/main.cpp libHNSCore.a  ../lib/HNSArduino/build/core.a
	$(CC) $(CFLAGS) $(LIBS) $(DEFINE_FLAGS) -o $(FIRMWARE).elf  ../src/main.cpp libHNSCore.a Adafruit_NeoPixel.o HNSSocketPower.o EmonLib.o ../lib/HNSArduino/build/core.a
	avr-size -C --mcu=atmega328p main.elf

HNSSocketPower.o: ../lib/HNSDeviceLib/HNSSocketPower/HNSSocketPower.h ../lib/HNSDeviceLib/HNSSocketPower/HNSSocketPower.cpp ../lib/HNSCommand/HNSSocketV2.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/HNSDeviceLib/HNSSocketPower/HNSSocketPower.cpp

EmonLib.o:  ../lib/HNSDeviceLib/HNSSocketPower/EmonLib/EmonLib.cpp ../lib/HNSDeviceLib/HNSSocketPower/EmonLib/EmonLib.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c   ../lib/HNSDeviceLib/HNSSocketPower/EmonLib/EmonLib.cpp

Adafruit_NeoPixel.o:  ../lib/HNSDevice/Adafruit_NeoPixel-master/Adafruit_NeoPixel.cpp ../lib/HNSDevice/Adafruit_NeoPixel-master/Adafruit_NeoPixel.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c ../lib/HNSDevice/Adafruit_NeoPixel-master/Adafruit_NeoPixel.cpp

#////////////////////////////libHNSCore////////////////////////////////////////

libHNSCore.a: HNSSync.o HNSDevice.o HNSTest.o HNSRouting.o CRC.o HNSError.o USART_Atmega328.o Time_Atmega328.o ADC_Atmega328P.o
	ar rvs libHNSCore.a HNSSync.o HNSDevice.o HNSTest.o HNSRouting.o CRC.o HNSError.o USART_Atmega328.o Time_Atmega328.o ADC_Atmega328P.o

HNSDevice.o: libHNSCore.a ../lib/HNSDevice/HNSDevice.h ../lib/HNSDevice/HNSDevice.c ../lib/util/Atmega328P/avrMacro.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/HNSDevice/HNSDevice.c ../lib/util/Atmega328P/avrMacro.h

HNSSync.o: ../lib/HNSDevice/HNSSync.h ../lib/HNSDevice/HNSSync.c
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/HNSDevice/HNSSync.c

HNSTest.o : HNSRouting.o  ../lib/HNSCore/HNSTest.h  ../lib/HNSCore/HNSTest.c
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/HNSCore/HNSTest.c

HNSRouting.o: USART_Atmega328.o HNSError.o CRC.o ../lib/HNSCore/HNSRouting.h ../lib/HNSCore/HNSRouting.c
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/HNSCore/HNSRouting.c

CRC.o: ../lib/util/CRC.c ../lib/util/CRC.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c ../lib/util/CRC.c

USART_Atmega328.o: Time_Atmega328.o ../lib/util/Atmega328P/USART_Atmega328.h ../lib/util/Atmega328P/USART_Atmega328.c ../lib/util/Atmega328P/MAX485.h ../lib/util/Atmega328P/avrMacro.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/util/Atmega328P/USART_Atmega328.c ../lib/util/Atmega328P/MAX485.h ../lib/util/Atmega328P/avrMacro.h

HNSError.o: ../lib/HNSCore/HNSError.h ../lib/HNSCore/HNSError.c
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/HNSCore/HNSError.c

ADC_Atmega328P.o: ../lib/util/Atmega328P/ADC_Atmega328P.h ../lib/util/Atmega328P/ADC_Atmega328P.c ../lib/util/Atmega328P/avrMacro.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/util/Atmega328P/ADC_Atmega328P.c ../lib/util/Atmega328P/avrMacro.h

Time_Atmega328.o: ../lib/util/Atmega328P/Time_Atmega328.h ../lib/util/Atmega328P/Time_Atmega328.c ../lib/util/Atmega328P/avrMacro.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/util/Atmega328P/Time_Atmega328.c ../lib/util/Atmega328P/avrMacro.h


.phony: clean

clean:
	rm -f $(FIRMWARE).elf $(FIRMWARE).hex *.o *.a

.phony: upload

upload: $(FIRMWARE).elf
	avr-objcopy -O ihex -R .eeprom $(FIRMWARE).elf $(FIRMWARE).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$(FIRMWARE).hex:i #$(AVRDUDE_WRITE_EEPROM)
