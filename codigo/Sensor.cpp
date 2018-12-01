
//Manda una cadena al proceso Control
// Debe ejecutarse antes Control.
//pues es este quien crea la cola de mensajes


#include <iostream>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "colamsg.hpp"
#include "semaforo.hpp"
#include "memocomp.hpp"
#define TAMCADENA 15
using namespace std;

void manejador(int signum);

char altura_agua[TAMCADENA], *p_aux, miProceso[TAMCADENA];
colamsg sensor("nivel_agua", OPEN, WRONLY);

semaforo s_sensor("sem2");

memocomp estado_deposito("nivel", OPEN, RDWR, TAMCADENA*sizeof(char));


int main ()
{
    p_aux = (char *)estado_deposito.getPointer();

    sprintf(miProceso, "%d",getpid());
    sensor.send(miProceso, TAMCADENA*sizeof(char));//enviamos el pid de sensor


    system("clear");                                          //Limpia la pantalla una vez se inice el programa
    cout<<"\n\n\n\n\n";

    cout<<"Nivel del deposito:          ";

    signal(SIGINT, manejador);

    while(true)
    {
        s_sensor.down();

        strcpy(altura_agua,p_aux);
        sensor.send(altura_agua, TAMCADENA*sizeof(char));

        printf("\b\b\b\b\b%05d",atoi(p_aux));
        fflush(stdout);                             //actualizamos el nivel del deposito sin tener que imprimir una linea cada vez

    }
}

void manejador(int signum)                      //cuando recibimos la señal ^C cierra los semaforos la memoria compartida y la cola de mensajes
{
    cout<<endl<<"Fin de programa Sensor"<<endl;

    sprintf(miProceso,"%d",-1);

    sensor.send(miProceso,TAMCADENA*sizeof(char));          //envia -1 como señal de fin de proceso

    sensor.close();

    s_sensor.close();

    estado_deposito.cerrar();
    exit(0);

}
