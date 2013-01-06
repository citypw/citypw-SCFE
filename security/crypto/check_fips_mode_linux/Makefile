CC = gcc
CFLAGS = -g
LIBS= -lcrypto

APP = check_fips_mode_linux get_set_fips_mode

all:$(APP)
	@echo "done"

check_fips_mode_linuxt: check_fips_mode_linux.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

get_set_fips_mode: get_set_fips_mode.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
clean:
	rm -f $(APP)
