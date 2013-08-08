#!/usr/bin/env python

# GPL'ed
# function hmac (key, message)
#    if (length(key) > blocksize) then
#        key = hash(key) // keys longer than blocksize are shortened
#    end if
#    if (length(key) < blocksize) then
#        key = key ∥ [0x00 * (blocksize - length(key))] // keys shorter than blocksize are zero-padded ('∥' is concatenation) 
#    end if
#   
#    o_key_pad = [0x5c * blocksize] ⊕ key // Where blocksize is that of the underlying hash function
#    i_key_pad = [0x36 * blocksize] ⊕ key // Where ⊕ is exclusive or (XOR)
#   
#    return hash(o_key_pad ∥ hash(i_key_pad ∥ message)) // Where '∥' is concatenation
# end function

#!/usr/bin/env python

from hashlib import md5

opad = "".join(chr(x ^ 0x5c) for x in xrange(256))
ipad = "".join(chr(x ^ 0x36) for x in xrange(256))
blocksize = md5().block_size

def hmac_md5(key, msg):
	if len(key) > blocksize:
	# shorten the key, if it's larger than the block
		print "key is larget than blocksize..."
		key = md5(key).digest()
	if len(key) < blocksize:
	# keys shorter than blocksize are 0-padded
		key += chr(0) * (blocksize - len(key))

	o_key_pad = key.translate(opad)
	i_key_pad = key.translate(ipad)
	print "o_key_pad: size:" + o_key_pad + " size: " + repr(len(o_key_pad))
	print "i_key_pad: size:" + i_key_pad + " size: " + repr(len(o_key_pad))

	return md5(o_key_pad + md5(i_key_pad + msg).digest())

if __name__ == "__main__":
	h = hmac_md5("key-key-key-key-key-key-key-key-key-key-key-key-key-key-key-key-key-", "The quick brown fox jumps over the lazy dog")
	print h.hexdigest()
