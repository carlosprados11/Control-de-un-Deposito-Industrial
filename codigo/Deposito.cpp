
//Este programa representa el comportamiento del deposito
//lee el estado de la valvula y escribe el nivel del deposito

#define TAMCADENA 15
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <iostream>
#include "semaforo.hpp"
#include "memocomp.hpp"
#define caudalEntrada 300
#define caudalSalida 100

using namespace std;

    int pid_valvula, pid_sensor;

    semaforo s_deposito("sem1", 0);
    semaforo s_sensor("sem2", 1);

    memocomp estado_llave("abre", CREAT, RDWR, TAMCADENA*sizeof(char));
    memocomp estado_deposito("nivel", CREAT, RDWR, TAMCADENA*sizeof(char));         //creamos dos zonas de memoria compartida de lect escrit y tamaño 1 byte (sizeof char)

    void manejador (int signum);

int main(int agrc, char *argv[])
{
    char *cadena_valvula, * p_aux;                       // puntero a la cadena en memo compartida
    int nivel=0;
    int Qentrada, Qsalida;

    if(agrc==3)                                          // Asignamos el valor del caudal de entrada
    {                                                   // y de salida correctamente mediante los
        if(atoi(argv[1]) > atoi(argv[2]))               // argumentos introducidos
        {
            Qentrada=atoi(argv[1]);
            Qsalida=atoi(argv[2]);
        }
        else if(atoi(argv[1]) < atoi(argv[2]))
        {
            Qentrada=atoi(argv[2]);
            Qsalida=atoi(argv[1]);
        }
        else
        {
            Qentrada=caudalEntrada;
            Qsalida=caudalSalida;
        }
    }

    else
    {
        Qentrada=caudalEntrada;
        Qsalida=caudalSalida;
    }

    cadena_valvula = (char *)estado_llave.getPointer();
    p_aux=(char *)estado_deposito.getPointer();          //vinculamos esta memo a un puntero

    signal(SIGINT, manejador);

    system("clear");                                     //Limpia la pantalla una vez se inice el programa
    cout<<"\n\n\n\n\n";
    cout<<"El caudal de entrada al deposito es "<<Qentrada<<" y el menor "<< Qsalida <<endl<<endl;

    cout<<"Nivel del tanque, Estado de la valvula:\t             ";

    while(true)
    {
        s_deposito.down();

        if(strcmp(cadena_valvula,"ON ")==0)
            nivel=nivel+Qentrada;
        else if(strcmp(cadena_valvula,"OFF")==0)
            nivel=nivel-Qsalida;

        sleep(1);

        sprintf(p_aux,"%d",nivel);

        printf("\b\b\b\b\b\b\b\b\b\b\b\b%05d       ",atoi(p_aux));
        fflush(stdout);
        printf("\b\b\b%s",cadena_valvula);
        fflush(stdout);                                                 //Con esto conseguimos que se actualice el valor de deposito y no escribir un monton de lineas

        s_sensor.up();

    }
}

void manejador(int signum)               //manejador de la señal Ctrl+C (^C)
                                         //Cerramos semaforos y memoria compartida y los eliminamos
 {
    cout<<endl<<"Fin de programa"<<endl;

    s_deposito.close();
    s_sensor.close();

    estado_llave.cerrar();
    estado_deposito.cerrar();

    estado_llave.unlink();
    estado_deposito.unlink();

    s_deposito.unlink();
    s_sensor.unlink();

    exit(0);

}
