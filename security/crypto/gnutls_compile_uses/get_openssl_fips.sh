#!/bin/sh

OPENSSL='openssl-1.0.1e'
FIPS='openssl-fips-2.0.5'
ROOT_DIR=$PWD

if [ ! -d "$OPENSSL" ]; then
	if [ ! -d "$OPENSSL.tar.gz" ]; then
		wget http://www.openssl.org/source/$OPENSSL.tar.gz
	fi
	tar zxvf $OPENSSL.tar.gz
fi

if [ ! -d "$FIPS" ]; then
	if [ ! -d "$FIPS.tar.gz" ]; then
		wget http://www.openssl.org/source/$FIPS.tar.gz
	fi
	tar zxvf $FIPS.tar.gz
fi

# Compiling FIPS at first
cd $ROOT_DIR/$FIPS
./config
make

cp fips/*.h $ROOT_DIR/$OPENSSL/include/openssl/
cp fips/rand/*.h $ROOT_DIR/$OPENSSL/include/openssl/

sudo mkdir -p /usr/local/ssl/fips-2.0/bin
sudo cp fips/fipsld /usr/local/ssl/fips-2.0/bin/
sudo ln -s `which openssl` /usr/local/ssl/fips-2.0/bin/openssl

# nOW, compiling the openssl....
cd $ROOT_DIR/$OPENSSL
./config fips no-shared --with-fipslibdir=$ROOT_DIR/$FIPS/fips/
make depend
make
