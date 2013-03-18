#!/bin/sh

certtool --generate-privkey > client-key.pem

certtool --generate-certificate --load-privkey client-key.pem \
	  --load-ca-certificate cacert.pem --load-ca-privkey cakey.pem \
	    --template client.info --outfile client.pem
