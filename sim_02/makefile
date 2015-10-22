CC = g++ -std=c++11
LFLAGS = -Wall -pedantic -pthread
CFLAGS = -Wall -pedantic -pthread -c
OBJS = main.o timer.o parser.o application.o config.h

sim02:	$(OBJS)
		$(CC) $(LFLAGS) $(OBJS) -o sim02

main.o:	main.cpp timer.h parser.h config.h
		$(CC) $(CFLAGS) main.cpp

timer.o:	timer.cpp timer.h
			$(CC) $(CFLAGS) timer.cpp

parser.o:	parser.cpp parser.h config.h
			$(CC) $(CFLAGS) parser.cpp

application.o:	application.cpp application.h config.h parser.h timer.h
				$(CC) $(CFLAGS) application.cpp

clean:
	rm sim02 *.o