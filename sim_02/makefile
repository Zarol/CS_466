CC = g++ -std=c++11
LFLAGS = -Wall -pedantic -pthread
CFLAGS = -Wall -pedantic -pthread -c
OBJS = main.o timer.o parser.o simulator.o application.o config.h Escape_Sequences_Colors.h

sim02:	$(OBJS)
		$(CC) $(LFLAGS) $(OBJS) -o sim02

main.o:	main.cpp timer.h parser.h config.h
		$(CC) $(CFLAGS) main.cpp

timer.o:	timer.cpp timer.h
			$(CC) $(CFLAGS) timer.cpp

parser.o:	parser.cpp parser.h config.h
			$(CC) $(CFLAGS) parser.cpp

simulator.o:	simulator.cpp simulator.h config.h parser.h timer.h application.h Escape_Sequences_Colors.h
				$(CC) $(CFLAGS) simulator.cpp		

application.o:	application.cpp application.h config.h parser.h timer.h Escape_Sequences_Colors.h
				$(CC) $(CFLAGS) application.cpp

clean:
	rm sim02 *.o