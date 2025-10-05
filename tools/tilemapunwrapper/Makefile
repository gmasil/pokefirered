CC ?= gcc

CFLAGS = -Wall -Wextra -Werror -std=c99 -O3 -flto -DPNG_SKIP_SETJMP_CHECK
CFLAGS += $(shell pkg-config --cflags libpng)

LIBS = -lpng -lz
LDFLAGS += $(shell pkg-config --libs-only-L libpng)

SRCS = main.c image_utils.c palette_utils.c tilemap_utils.c file_utils.c tile_utils.c

ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif

.PHONY: all clean

all: tilemapunwrapper$(EXE)
	@:

tilemapunwrapper$(EXE): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS) $(LIBS)

clean:
	$(RM) tilemapunwrapper tilemapunwrapper.exe
