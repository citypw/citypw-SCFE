#!/bin/sh

PWD=`pwd`
COMPILE=no
GNUTLS=gnutls-3.1.9
NETTLE=nettle-2.6
LIBTASN1=libtasn1-3.2
P11=p11-kit-0.16.3
GMP=gmp-5.1.1
PREFIX=/home/john/john_test
ROOT_DIR=$PWD

if [ $# -ne 1 ]; then
	echo "Usage: [ Compile packages automatically: yes/no]"
	exit 1
fi

COMPILE="$1"
echo "Compile all pkgs, automatically: $COMPILE"

if [ ! -d "$GNUTLS" ]; then
	if [ ! -f "$GNUTLS.1.tar" ]; then
		wget ftp://ftp.gnutls.org/gcrypt/gnutls/v3.1/$GNUTLS.1.tar.xz
	fi
	xz -d $GNUTLS.1.tar.xz && tar xvf $GNUTLS.1.tar
fi

if [ ! -d "$NETTLE" ]; then
	if [ ! -f "$NETTLE.tar.gz" ]; then
		wget ftp://ftp.lysator.liu.se/pub/security/lsh/$NETTLE.tar.gz
	fi
	tar zxvf $NETTLE.tar.gz
fi

if [ ! -d "$LIBTASN1" ]; then
	if [ ! -f "$LIBTASN1.tar.gz" ]; then
		wget http://ftp.gnu.org/gnu/libtasn1/$LIBTASN1.tar.gz
	fi
	tar zxvf $LIBTASN1.tar.gz
fi

if [ ! -d "$P11" ]; then
	if [ ! -f "$P11.tar.gz" ]; then
		wget http://p11-glue.freedesktop.org/releases/$P11.tar.gz
	fi
	tar zxvf $P11.tar.gz
fi

if [ ! -d "$GMP" ]; then
	if [ ! -f "$GMP.tar.bz2" ]; then
		wget ftp://ftp.gnu.org/gnu/gmp/$GMP.tar.bz2
	fi
	tar jxvf $GMP.tar.bz2
fi

if [ "$COMPILE" == "yes" ]; then
#cat << EOF >> ~/.bashrc
#ROOT_SHAWN=$PREFIX
#
#export PATH=$PATH:$ROOT_SHAWN/bin:$ROOT_SHAWN/sbin:$JAVA_HOME/bin
#
#C_INCLUDE_PATH=$ROOT_SHAWN/include:/opt/AMDAPP/include
#CPLUS_INCLUDE_PATH=$ROOT_SHAWN/include
#LD_LIBRARY_PATH=$ROOT_SHAWN/lib
#LIBRARY_PATH=$ROOT_SHAWN/lib
#
#export C_INCLUDE_PATH LD_LIBRARY_PATH
#EOF
#source ~/.bashrc
	cd $ROOT_DIR/$LIBTASN1
	./configure --prefix=$PREFIX
	make && make install

	cd $ROOT_DIR/$P11
	LIBTASN1_CFLAGS="-I$PREFIX/include -L$PREFIX/lib" LIBTASN1_LIBS="-ltasn1" ./configure --prefix=$PREFIX
	make && make install

	cd $ROOT_DIR/$GMP
	./configure --prefix=$PREFIX
	make && make install

	cd $ROOT_DIR/$NETTLE
	./configure --prefix=$PREFIX
	make && make install

	mv $PREFIX/include/p11-kit-1/p11-kit $PREFIX/include/

	cd $ROOT_DIR/$GNUTLS
	P11_KIT_CFLAGS="-I$PREFIX/include -L$PREFIX/lib" P11_KIT_LIBS="-lp11-kit" ./configure --prefix=$PREFIX --with-libnettle-prefix=$PREFIX
	make && make install
fi
