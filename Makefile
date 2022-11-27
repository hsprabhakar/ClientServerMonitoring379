OBJSS	= server.o tands.o helpers.o
OBJSC	= client.o tands.o helpers.o
SOURCES	= server.c tands.c helpers.c
SOURCEC = client.c tands.c helpers.c
HEADER	= tands.h includes.h
OUTS	= server
OUTC	= client
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -O
PDFCLIENT	= client.pdf
PDFSERVER	= server.pdf
CLIENTMAN	= ./client.1
SERVERMAN	= ./server.1
LOGGER	= logger



all: $(OBJSS) $(OBJSC)
	$(CC) -g $(OBJSS) -o $(OUTS) $(LFLAGS)
	$(CC) -g $(OBJSC) -o $(OUTC) $(LFLAGS)
	man -Tpdf $(CLIENTMAN) >$(PDFCLIENT)
	man -Tpdf $(SERVERMAN) >$(PDFSERVER)


server.o: server.c
	$(CC) $(FLAGS) server.c 

client.o: client.c
	$(CC) $(FLAGS) client.c 

tands.o: tands.c
	$(CC) $(FLAGS) tands.c 

helpers.o: helpers.c
	$(CC) $(FLAGS) helpers.c 


clean:
	rm -f $(OBJSC) $(OUTC)
	rm -f $(OBJSS) $(OUTS)
	rm -f $(PDFCLIENT)
	rm -f $(PDFSERVER)
	rm -f $(LOGGER)