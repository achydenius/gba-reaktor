include $(DEVKITARM)/gba_rules

CC			:=	$(DEVKITARM)/bin/arm-none-eabi-gcc
OBJCOPY		:=	$(DEVKITARM)/bin/arm-none-eabi-objcopy
GBAFIX		:=	${DEVKITPRO}/tools/bin/gbafix
MMUTIL		:=	${DEVKITPRO}/tools/bin/mmutil

ARCH		:=	-mthumb -mthumb-interwork
SPECS		:=	-specs=gba.specs

CFLAGS		:=	-g -Wall -pedantic -O3\
				-mcpu=arm7tdmi -mtune=arm7tdmi\
				-fomit-frame-pointer\
				-ffast-math\
				$(ARCH)
LDFLAGS		:=	$(ARCH) $(SPECS)
INCLUDES	:=	-I$(DEVKITPRO)/libgba/include
AUDIO		:=	audio
LIBS		:=	-L$(DEVKITPRO)/libgba/lib -lmm -lgba

export AUDIOFILES := $(foreach dir,$(notdir $(wildcard $(AUDIO)/*.*)),$(CURDIR)/$(AUDIO)/$(dir))

all: main.gba

soundbank.bin.o: soundbank.bin
	@$(bin2o)

soundbank.bin: $(AUDIOFILES)
	$(MMUTIL) $^ -osoundbank.bin -hsoundbank.h

main.gba: main.elf
	$(OBJCOPY) -O binary main.elf main.gba
	$(GBAFIX) main.gba

main.elf: main.o
	$(CC) main.o soundbank.bin.o $(LDFLAGS) $(LIBS) -o main.elf

main.o: soundbank.bin.o main.c tables.h objects.h *.h
	$(CC) -c main.c $(CFLAGS) -o main.o $(INCLUDES)

tables.h: generate-tables.py
	./generate-tables.py > tables.h

objects.h: parse-obj.py monogram.obj
	./parse-obj.py > objects.h

clean:
	rm main.gba main.elf main.o tables.h objects.h soundbank.*
