CC = g++ -std=c++11
LFLAGS = -Wall -pedantic -pthread
CFLAGS = -Wall -pedantic -pthread -c
OBJS = main.o timer.o parser.o simulator.o application.o log.o config.h Escape_Sequences_Colors.h

sim02:	$(OBJS)
		$(CC) $(LFLAGS) $(OBJS) -o sim02

main.o:	main.cpp timer.h parser.h log.h
		$(CC) $(CFLAGS) main.cpp

timer.o:	timer.cpp timer.h
			$(CC) $(CFLAGS) timer.cpp

parser.o:	parser.cpp parser.h
			$(CC) $(CFLAGS) parser.cpp

simulator.o:	simulator.cpp simulator.h parser.h timer.h application.h log.h
				$(CC) $(CFLAGS) simulator.cpp		

application.o:	application.cpp application.h parser.h timer.h log.h
				$(CC) $(CFLAGS) application.cpp

log.o:	log.cpp log.h
		$(CC) $(CFLAGS) log.cpp

clean:
	rm sim02 *.o