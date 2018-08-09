#ifndef __parametros_h__
#define __parametros_h__

#include <vector>
#include <map>

class Parametros {
    private:

    public:
        int cant_trab;                              //Cantidad de trabajadores
        int cant_dias;                              //Cantidad de dias
        int cant_turnos;                            //Cantidad de turnos por dia
        vector< vector<int> > matriz_dist;          //Matriz distribucion
        vector< vector<int> > matriz_pref;          //Matriz preferencia
        int min_asig_periodo;                       //Minimo de asignaciones por periodo
        int max_asig_periodo;                       //Maximo de asignaciones por periodo
        map<int, vector<int>> min_max_asig_turno;   //Min y Max de asignaciones por cada turno
        int min_turnos_consec;                      //Minimo de turnos de trabajo consecutivos
        int max_turnos_consec;                      //Maximo de turnos de trabajo consecutivos
        map<int, vector<int>> min_max_turno_consec; //Min y Max de turnos consecutivos del mismo tipo

        Parametros() {
            /* Inicializar parametros a cero */
        }
        int load_instance();

};

#endif