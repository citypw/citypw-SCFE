#!/bin/sh

certtool --generate-privkey > cakey.pem

certtool --generate-self-signed --load-privkey cakey.pem --template ca.info --outfile cacert.pem
