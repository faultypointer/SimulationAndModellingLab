RAYLIB ?= ./external/raylib-5.5/src/
CFLAGS := -Wall -Wextra -Werror -O2
LFLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

.PHONY: clean

all: lab1

lab1: | target
	gcc $(CFLAGS) src/01_discharging_of_capacitor/main.c src/common/rk2.c  -L $(RAYLIB) $(LFLAGS) -o target/$@


target:
	mkdir -p target

clean:
	rm -f target/*
