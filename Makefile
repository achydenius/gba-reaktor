CC			:=	$(DEVKITARM)/bin/arm-none-eabi-gcc
OBJCOPY		:=	$(DEVKITARM)/bin/arm-none-eabi-objcopy
GBAFIX		:=	${DEVKITPRO}/tools/bin/gbafix

ARCH		:=	-mthumb -mthumb-interwork
SPECS		:=	-specs=gba.specs

CFLAGS		:=	-g -Wall -pedantic -O3\
				-mcpu=arm7tdmi -mtune=arm7tdmi\
				-fomit-frame-pointer\
				-ffast-math\
				$(ARCH)
LDFLAGS		:=	$(ARCH) $(SPECS)
INCLUDES	:=	$(DEVKITPRO)/libgba/include

all: main.gba

main.gba: main.elf
	$(OBJCOPY) -O binary main.elf main.gba
	$(GBAFIX) main.gba

main.elf: main.o
	$(CC) main.o $(LDFLAGS) -o main.elf

main.o: main.c
	$(CC) -c main.c $(CFLAGS) -o main.o -I$(INCLUDES)

clean:
	rm main.gba main.elf main.o
