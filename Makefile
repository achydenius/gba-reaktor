CC		:=	$(DEVKITARM)/bin/arm-none-eabi-gcc
OBJCOPY	:=	$(DEVKITARM)/bin/arm-none-eabi-objcopy
GBAFIX	:=	${DEVKITPRO}/tools/bin/gbafix

ARCH	:=	-mthumb -mthumb-interwork
SPECS	:=	-specs=gba.specs

CFLAGS	:=	-g -Wall -O3\
			-mcpu=arm7tdmi -mtune=arm7tdmi\
			-fomit-frame-pointer\
			-ffast-math\
			$(ARCH)
LDFLAGS	:=	$(ARCH) $(SPECS)

all: main.gba

main.gba: main.elf
	$(OBJCOPY) -O binary main.elf main.gba
	$(GBAFIX) main.gba

main.elf: main.o
	$(CC) main.o $(LDFLAGS) -o main.elf

main.o: main.c
	$(CC) -c main.c $(CFLAGS) -o main.o

clean:
	rm main.gba main.elf main.o
