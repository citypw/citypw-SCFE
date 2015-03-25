%.so: %.c
	gcc -fpic -shared -Wl,-soname,$@ -o $@ $^

all: libgood.so libevil.so makepoc target libpoc.so all_is_well

libpoc.so: target.handchecked makepoc
	./makepoc

clean:
	rm -f *.so *.o target target.hand* makepoc all_is_well

target: target.c libgood.so libevil.so
	echo "#define INTERP \" 'objcopy -O binary -j .interp \
		/bin/ls /dev/stdout'\"" >> interp.inc && gcc -o target \
		-Os -Wl,-rpath,. -Wl,-efoo  -L . -shared -fPIC -lgood target.c \
	       && strip -K foo $@ && echo 'copy target to target.handchecked by hand!'

target.handchecked: target
	cp $< $@;
	echo "Beware, you compiled target yourself. YMMv with your compiler, this is ust a friendly poc"

all_is_well: all_is_well.c libpoc.so
	gcc -o $@ -Wl,-rpath ,. -lpoc -L. $<

makepoc: makepoc.c
	gcc -ggdb -o $@ $<
