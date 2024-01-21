CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
SRC = $(wildcard src/*.c src/*/*.c)
OBJ = $(SRC:.c=.o)
RUN_ARGS ="-s"

all:
	gcc $(CFLAGS) $(SRC) -o src/icmp.o

debug:
	gcc $(CFLAGS) -g $(SRC) -o src/icmp.o
	sudo gdb ./src/icmp.o

container:
	-sudo docker rm icmp-server
	-sudo docker rm icmp-client
	sudo docker build --build-arg ARGS=$(RUN_ARGS) --no-cache --tag icmp-server .
	@echo "\n\n[--> To switch to daemon mode, press ctrl+p then ctrl+q <--]\n"
	@sudo docker run -it --privileged --init --rm --name icmp-server icmp-server

noargs:
	gcc -std=c99 -pedantic $(SRC) -g -o src/icmp-test.o

check:
	sudo valgrind --leak-check=yes src/*.o

clean:
	rm -f $(OBJ)

.PHONY: all debug container noargs check clean
