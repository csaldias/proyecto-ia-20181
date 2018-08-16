#ifndef __instancia_h__
#define __instancia_h__

#include "preferencia.h"
#include "demanda.h"
#include "solucion.h"
#include "puntaje.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

class Instancia {
    private:

    public:
        int cantTrabajadores;                              //Cantidad de trabajadores
        int cantDias;                                      //Cantidad de dias
        int cantTurnos;                                    //Cantidad de turnos por dia
        int minAsignaciones;                               //Minimo de asignaciones por periodo
        int maxAsignaciones;                               //Maximo de asignaciones por periodo
        int minAsignacionesConsecutivas;                   //Minimo de turnos de trabajo consecutivos
        int maxAsignacionesConsecutivas;                   //Maximo de turnos de trabajo consecutivos
        map<int, vector<int> > asigPorTurno;               //Min y Max de asignaciones por cada turno
        map<int, vector<int> > asigConsecPorTurno;         //Min y Max de turnos consecutivos del mismo tipo
        map<int, vector<Preferencia> > matrizPreferencias; //Matriz de preferencias {trabajador: [preferencias]}
        map<int, vector<Demanda> > matrizDemandas;         //Matriz de distribucion (o demanda) {dia: [demandas]}
        

        Instancia() {
            /* Inicializar parametros a cero */
        }
        int loadInstance(const char pathGenFile[], const char pathNspFile[]);
        
        int getCantTrabajadores();
        int getCantDias();
        int getCantTurnos();
        int getMinAsignaciones();
        int getMaxAsignaciones();
        int getMinAsignacionesConsecutivas();
        int getMaxAsignacionesConsecutivas();
        map<int, vector<int> > getAsigPorTurno();
        map<int, vector<int> > getAsigConsecPorTurno();
        map<int, vector<Preferencia> > getPreferencias();
        map<int, vector<Demanda> > getDemandas();
        vector<Demanda> getDemandaDia(int dia);
        Demanda getDemandaDiaTurno(int dia, int turno);

        map<int, vector<Solucion> > generarSolucion();
        map<int, vector<Solucion> > variarSolucion(map<int, vector<Solucion> >);
        void outputSolucion(map<int, vector<Solucion> > solucion, map<int, Puntaje> puntajes, string nombre);
};

#endif