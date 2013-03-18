#!/bin/sh

certtool --generate-privkey > server-key.pem

certtool --generate-certificate --load-privkey server-key.pem \
	   --load-ca-certificate cacert.pem --load-ca-privkey cakey.pem \
	     --template server.info --outfile server.pem
