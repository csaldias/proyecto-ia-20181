#ifndef __instancia_h__
#define __instancia_h__

#include "preferencia.h"
#include "solucion.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

class Instancia {
    private:

    public:
        int cantTrabajadores;                      //Cantidad de trabajadores
        int cantDias;                              //Cantidad de dias
        int cantTurnos;                            //Cantidad de turnos por dia
        int minAsignaciones;                       //Minimo de asignaciones por periodo
        int maxAsignaciones;                       //Maximo de asignaciones por periodo
        int minAsignacionesConsecutivas;           //Minimo de turnos de trabajo consecutivos
        int maxAsignacionesConsecutivas;           //Maximo de turnos de trabajo consecutivos
        map<int, vector<int> > asigPorTurno;       //Min y Max de asignaciones por cada turno
        map<int, vector<int> > asigConsecPorTurno; //Min y Max de turnos consecutivos del mismo tipo
        vector< vector<int> > matriz_dist;          //Matriz distribucion
        vector< vector<int> > matriz_pref;          //Matriz preferencia
        

        Instancia() {
            /* Inicializar parametros a cero */
        }
        int loadInstance(const char pathGenFile[], const char pathNspFile[]);
        string getFileName();
        map<int, vector<Preferencia> > getPreferencias();
        map<int, vector<Solucion> > generarSolucion();
        int getNumeroDias();

};

#endif