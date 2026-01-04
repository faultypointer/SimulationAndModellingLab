RAYLIB ?= ./external/raylib-5.5/src/
CFLAGS := -Wall -Wextra -Werror -O2 -g3
LFLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

.PHONY: clean

all: lab1 lab2 lab3

lab1: rk2.o lab1.o | target
	gcc $(CFLAGS) target/rk2.o target/lab1.o  -L $(RAYLIB) $(LFLAGS) -o target/$@

lab1.o: | target
	gcc $(CFLAGS) -c src/01_discharging_of_capacitor/main.c  -o target/$@


lab2: rk2.o lab2.o | target
	gcc $(CFLAGS) target/rk2.o target/lab2.o  -L $(RAYLIB) $(LFLAGS) -o target/$@

lab2.o: | target
	gcc $(CFLAGS) -c src/02_spring_mass_system/main.c  -o target/$@

lab3: lab3.o | target
	gcc $(CFLAGS) target/lab3.o  -L $(RAYLIB) $(LFLAGS) -o target/$@

lab3.o: | target
	gcc $(CFLAGS) -c src/03_approximate_pi/main.c  -o target/$@


rk2.o: | target
	gcc $(CLFAGS) -c src/common/rk2.c -o target/$@

target:
	mkdir -p target

clean:
	rm -f target/*
