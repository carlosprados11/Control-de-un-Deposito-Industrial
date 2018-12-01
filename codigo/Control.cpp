
//Recibe una cadena del proceso Sensor
//IMPORTANTE: Debe ejecutarse primero este proceso
//pues es este quien crea la cola de mensajes

#include <iostream>
#include "colamsg.hpp"
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#define TAMCADENA 15
#define Nivel_MIN 1000
#define Nivel_MAX 10000
using namespace std;

    char pid_sensor[TAMCADENA], pid_valvula[TAMCADENA], proceso_valvula[TAMCADENA],proceso_sensor[TAMCADENA];

    colamsg cola_sensor("nivel_agua", CREAT, RDONLY, TAMCADENA*sizeof(char));//Tienen qe ser declaradas comovariables globales

    colamsg cola_valv("llave", CREAT, WRONLY, TAMCADENA*sizeof(char));      //para que el manejador las pueda eliminar

    colamsg cola_vive("proceso",CREAT,RDWR,TAMCADENA*sizeof(char));

 void manejador(int signum);                            // declaración del manejador de señales
 void alarma_sensor(int signum);
 void alarma_valvula(int signum);

int main (int agrc, char *argv[])
{
    int nivel, nivelMax,nivelMin;

    if(agrc==3)
    {
        if(atoi(argv[1])>atoi(argv[2]))
        {
            nivelMax=atoi(argv[1]);
            nivelMin=atoi(argv[2]);
        }
        else if (atoi(argv[1])<atoi(argv[2]))
        {
            nivelMax=atoi(argv[2]);
            nivelMin=atoi(argv[1]);                  //Introduce los límites del deposito independientemente del orden y solo si son lógicos
        }
        else
        {
                nivelMax=Nivel_MAX;
                nivelMin=Nivel_MIN;
        }
    }

    else
    {
        nivelMax=Nivel_MAX;
        nivelMin=Nivel_MIN;                           //Toma valores por defecto
    }

    system("clear");                                  //Limpia la pantalla una vez se inice el programa
    cout<<"\n\n\n\n\n";
    cout<<"Los limites del deposito son el mayor: "<<nivelMax<<" y el menor: "<<nivelMin<<endl<<endl;

    char cadenaRecibida[TAMCADENA], mandaValvula[TAMCADENA];
    strcpy(mandaValvula,"OFF");                                //Valor por defecto de la valvula OFF

    cola_vive.receive(pid_valvula,TAMCADENA*sizeof(char));

    cola_sensor.receive(pid_sensor,TAMCADENA*sizeof(char));

    signal(SIGINT, manejador);

    cout<<"Nivel del tanque, Estado de la valvula:\t            ";

    while(true)
    {

        signal(SIGALRM,alarma_sensor);
        alarm(2);

        cola_sensor.receive(cadenaRecibida, TAMCADENA*sizeof(char));

        if(atoi(cadenaRecibida)!=-1)
        {
            alarm(0);

            nivel=atoi(cadenaRecibida);
            if(nivel<nivelMin)
                    strcpy(mandaValvula,"ON ");
            if(nivel>nivelMax)
                    strcpy(mandaValvula,"OFF");

            printf("\b\b\b\b\b\b\b\b\b\b\b\b%05d       ", nivel);

            fflush(stdout);
            printf("\b\b\b%s",mandaValvula);
            fflush(stdout);

        }

        cola_valv.send(mandaValvula,TAMCADENA*sizeof(char));

        signal(SIGALRM,alarma_valvula);
        alarm(2);
        cola_vive.receive(proceso_valvula,TAMCADENA*sizeof(char));

        if(atoi(proceso_valvula)!=-1)
            alarm(0);
    }
}

void alarma_valvula(int signum)                                         //detecta fallo en Valvula
{
    cout<<endl<<"Fallo en Valvula"<<endl;
    cola_vive.receive(pid_valvula,TAMCADENA*sizeof(char));

}

void alarma_sensor(int signum)                                         //detecta fallo en Sensor
{
    cout<<endl<<"Fallo en Sensor"<<endl;
    cola_sensor.receive(pid_sensor,TAMCADENA*sizeof(char));

}

void manejador(int signum)
{
    cout<<endl<<"Fin de programa"<<endl;
    kill(atoi(pid_sensor),SIGINT);
    kill(atoi(pid_valvula),SIGINT);                  //Mandamos señal de terminacion a Valvula y a Sensor

    cola_sensor.close();
    cola_valv.close();
    cola_vive.close();

    cola_sensor.unlink();
    cola_valv.unlink();
    cola_vive.unlink();

    exit(0);
}



