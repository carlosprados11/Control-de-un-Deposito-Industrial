#Makefile para la compilacion de los prog Sensor y Control con colas de msg
#Limpia tambien los semaforos

todo: Control Sensor Valvula Deposito

Control: Control.o colamsg.o
	g++ Control.o colamsg.o -o Control -lrt -lpthread

Sensor: Sensor.o colamsg.o memocomp.o semaforo.o
	g++ Sensor.o colamsg.o memocomp.o semaforo.o -o Sensor -lrt -lpthread

Valvula: Valvula.o colamsg.o memocomp.o semaforo.o
	g++ Valvula.o colamsg.o memocomp.o semaforo.o -o Valvula -lrt -lpthread

Deposito: Deposito.o memocomp.o semaforo.o
	g++ Deposito.o memocomp.o semaforo.o -o Deposito -lrt -lpthread

Control.o: Control.cpp colamsg.hpp
	g++ -c Control.cpp

Sensor.o: Sensor.cpp colamsg.hpp memocomp.hpp semaforo.hpp
	g++ -c Sensor.cpp

Valvula.o: Valvula.cpp colamsg.hpp memocomp.hpp semaforo.hpp
	g++ -c Valvula.cpp

Deposito.o: Deposito.cpp memocomp.hpp semaforo.hpp
	g++ -c Deposito.cpp

colamsg.o: colamsg.cpp colamsg.hpp
	g++ -c colamsg.cpp

memocomp.o: memocomp.cpp memocomp.hpp
	g++ -c memocomp.cpp

semaforo.o: semaforo.cpp semaforo.hpp
	g++ -c semaforo.cpp

clean:
	rm /dev/mqueue/1000*
	rm /dev/shm/sem.1000*
	rm /dev/shm/1000*