
import sys
import socket
import time

HOST = sys.argv[1]
PORT = 9556

sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sockfd.connect((HOST, PORT))
buf = sockfd.recv(128)
print "Server: %s" % buf
time.sleep(1)
buf = "Are you a Jew?"
sockfd.send(buf)
print "Client: %s" % buf

buf = sockfd.recv(128)
print "Server: %s" % buf

if buf == 'Yes' or buf == 'yes':
     buf = "Actually, the singularitarian has always believed in ....."
     sockfd.send(buf)
     print "Client: %s" % buf

if buf == 'No' or buf == 'no':
    buf = "Yo, how you doing? buddy"
    sockfd.send(buf)
    print "Client: %s" % buf

sockfd.close()
