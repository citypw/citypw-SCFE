require 'openssl'

c = OpenSSL::Cipher::Cipher.new('des-cbc')
c.encrypt
c.key = "\x01\x23\x45\x67\x89\xab\xcd\xef"
c.iv = "\xfe\xdc\xba\x98\x76\x54\x32\x10"
data = c.update("Yesterday's enemy becomes today's bitch!@#") + c.final
print "Ciphertext:"
print data.unpack("H*")
