
//Recibe una cadena al proceso Control
// Debe ejecutarse antes Control.
//pues es este quien crea la cola de mensajes

#include <iostream>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "colamsg.hpp"
#include "semaforo.hpp"
#include "memocomp.hpp"
#define TAMCADENA 15

using namespace std;

        char miCadena[TAMCADENA], miProceso[TAMCADENA], *p_aux;

        colamsg valvula("llave", OPEN , RDONLY);
        colamsg cola_vive("proceso",OPEN,RDWR,TAMCADENA*sizeof(char));

        semaforo s_deposito("sem1");

        memocomp estado_llave("abre", OPEN, RDWR, TAMCADENA*sizeof(char));

        void manejador (int signum);                                // declaración del manejador de señales

int main ()
{
    sprintf(miProceso,"%d",getpid());
    cola_vive.send(miProceso,TAMCADENA*sizeof(char));        //enviamos el pid de valvula

    p_aux = (char *)estado_llave.getPointer();

    sprintf(miProceso,"%d",1);

    signal (SIGINT,manejador);

    system("clear");                                          //Limpia la pantalla una vez se inice el programa
    cout<<"\n\n\n\n\n";

    cout<<"Estado valvula:\t   ";

    while(true)
    {
        valvula.receive(miCadena, TAMCADENA*sizeof(char));
        strcpy(p_aux,miCadena);

        printf("\b\b\b%s",p_aux);
        fflush(stdout);                                         //actualizamos el estado de la valvula sin tener que imprimir una linea cada vez

        cola_vive.send(miProceso,TAMCADENA*sizeof(char));       //envia 1 todo en orden

        s_deposito.up();
    }
}

void manejador(int signum)                                      //cuando recibimos la señal ^C cierra los semaforos la memoria compartida y la cola de mensajes
{
    cout<<endl<<"Fin de programa Valvula"<<endl;

    sprintf(miProceso,"%d",-1);
    cola_vive.send(miProceso,TAMCADENA*sizeof(char));

    valvula.close();
    cola_vive.close();

    s_deposito.close();

    estado_llave.cerrar();

    exit(0);
}
