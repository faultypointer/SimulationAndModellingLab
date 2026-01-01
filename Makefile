RAYLIB ?= ./external/raylib-5.5/src/
CFLAGS := -Wall -Wextra -Werror -O2
LFLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

.PHONY: clean

all: lab1

lab1: | target
	gcc $(CFLAGS) 01_discharging_of_capacitor/main.c -I $(RAYLIB) -L $(RAYLIB) $(LFLAGS) -o target/$@


target:
	mkdir -p target

clean:
	rm -f target/*
