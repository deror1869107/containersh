all: check build_image

CC = gcc

build_image:
	docker build -t ubuntu_socat .

check: check.o
	$(CC) $+ -o $@

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f check check.o
