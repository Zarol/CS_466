CC = g++ -std=c++11
INC = -I.
LFLAGS = -Wall -pedantic -pthread
CFLAGS = $(INC) -Wall -pedantic -pthread -c
OBJS = main.o timer.o parser.o simulator.o application.o operation.o log.o Tools/config.h Tools/Escape_Sequences_Colors.h

sim03:	$(OBJS)
		$(CC) $(LFLAGS) $(OBJS) -o sim03

main.o:	main.cpp Tools/timer.h Tools/parser.h Tools/log.h
		$(CC) $(CFLAGS) main.cpp

timer.o:	Tools/timer.cpp Tools/timer.h
			$(CC) $(CFLAGS) Tools/timer.cpp

parser.o:	Tools/parser.cpp Tools/parser.h
			$(CC) $(CFLAGS) Tools/parser.cpp

simulator.o:	Simulation/simulator.cpp Simulation/simulator.h Tools/parser.h Tools/timer.h Tools/log.h Simulation/application.h Simulation/operation.h
				$(CC) $(CFLAGS) Simulation/simulator.cpp		

application.o:	Simulation/application.cpp Simulation/application.h Tools/parser.h Tools/timer.h Tools/log.h Simulation/operation.h
				$(CC) $(CFLAGS) Simulation/application.cpp

operation.o:	Simulation/operation.cpp Simulation/operation.h Tools/timer.h Tools/log.h
				$(CC) $(CFLAGS) Simulation/operation.cpp

log.o:	Tools/log.cpp Tools/log.h
		$(CC) $(CFLAGS) Tools/log.cpp

clean:
	rm -f sim03 *.o