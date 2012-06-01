CC=gcc
AS=as
LD=ld
CFLAGS= -O0 -fno-stack-protector -z norelro -z execstack
# -fno-stack-protector: disable stack protection
LIBS =

APP = getenvaddr fmt_vuln gen_shellcode

all: $(APP)
	@echo "done"


getenvaddr: getenvaddr.c
	$(CC) -o $@ $^

fmt_vuln: fmt_vuln.c
	$(CC) $(CFLAGS) -o $@ $^

gen_shellcode: gen_shellcode.c
	$(CC) -o $@ $^

clean:
	rm -f *.o $(APP) shellcode.bin
