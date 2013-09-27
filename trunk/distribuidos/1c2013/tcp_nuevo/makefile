#

MYLIB= enviar.o recibir.o
LIBS	=
CFLAGS	= -O -g

# Internet stream version (TCP protocol).
#

tcp:	 tcpservc tcpservi tcpcli tcpecos

tcpcli.o tcpservc.o tcpservi.o tcpecos.o: inet.h

tcpservc:	tcpservc.o tcpoppas.o tcpservertrans.o  $(MYLIB)
		c++ $(CFLAGS) -o $@ tcpservc.o tcpoppas.o tcpservertrans.o  $(MYLIB) $(LIBS)

tcpecos:	tcpecos.o tcpservertrans.o  $(MYLIB)
		c++ $(CFLAGS) -o $@ tcpecos.o tcpservertrans.o  $(MYLIB) $(LIBS)

tcpservi:	tcpservi.o tcpoppas.o tcpservertrans.o  $(MYLIB)
		c++ $(CFLAGS) -o $@ tcpservi.o tcpoppas.o tcpservertrans.o  $(MYLIB) $(LIBS)

tcpcli:	tcpcli.o tcpopact.o tcpclienttrans.o
		c++ $(CFLAGS) -o $@ tcpcli.o tcpopact.o tcpclienttrans.o $(MYLIB) $(LIBS)

clean:
	rm -f *.o tcpcli tcpservc tcpservi tcpecos
