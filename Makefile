export CC = gcc

PWD = $(shell pwd)

HACKS = @networking

all: $(HACKS)
	@echo $(PWD)

@networking:
	@echo "net"

