from socket import *
s=socket(AF_INET, SOCK_STREAM, 0);
s.connect(('', 32323))
bin = open('androidauto.h264').read()
p = 0
while p < len(bin):
	r = s.send(bin[p:])
	print 'sent', r
	p += r
raw_input();
