#
# This file defines the source and target file names
#
# TARGETS is a list of available targets
# TARGET is the default target.
#
# C_OBJECTS are the rest of the C files
# S_OBJECTS are the assembler files

VM_DIR := javavm
VM_PREFIX := jvm_

TARGETS := lejos_nxt_rom.bin # lejos_nxt_ram.bin lejos_nxt_samba.bin
TARGET := lejos_nxt_rom.bin

C_PLATFORM_OBJECTS := \
	aic.o \
	systick.o \
	udp.o \
	twi.o \
	nxt_spi.o \
	nxt_motors.o \
	data_abort.o \
	display.o \
	i2c.o \
	sound.o \
	bt.o \
	hs.o \
	usart.o \
	flashprog.o \
	flashwrite.o \
	printf.o

C_HOOK_OBJECTS := \
	main.o \
	maininit.o \
	nxt_avr.o \
	sensors.o \
	nxt_lcd.o

C_OBJECTS := \
	$(C_PLATFORM_OBJECTS) \
	$(C_HOOK_OBJECTS) \
	$(C_RAM_OBJECTS)

S_OBJECTS := \
	interrupts.o \
	vectors.o \
	init.o \
	irq.o
