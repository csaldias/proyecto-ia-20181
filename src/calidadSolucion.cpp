#include "calidadSolucion.h"

#include "preferencia.h"
#include "solucion.h"
#include "instancia.h"
#include "demanda.h"
#include "config.h"

#include <map>
#include <vector>

using namespace std;

int CalidadSolucion::calcular(Instancia instancia, map<int, vector<Preferencia>> preferencias, map<int, vector<Solucion>> solucion) {
    int costoRestriccBlandas = calcularRestriccBlandas(instancia, preferencias, solucion);
    int costoRestriccDuras = calcularRestriccDuras(instancia, solucion);
    return costoRestriccBlandas + costoRestriccDuras;
}

int CalidadSolucion::calcularRestriccBlandas(Instancia instancia, map<int, vector<Preferencia>> preferencias, map<int, vector<Solucion>> solucion) {
    Config config;
    //workedSequence ==> (min,max)_asig_periodo
    int totalPenalidades = 0;
    //Penalizar preferencias
    //Instancia podría ser parametros
    for(int dia = 1; dia <= instancia.getCantDias(); dia++) {
        map<int, vector<Solucion>>::iterator it;
        for(it = solucion.begin(); it != solucion.end(); it++) {
            //it->first es int, trabajador
            //it->second es vector<Solucion>, soluciones
            vector<Preferencia> listaPreferencias = preferencias[it->first]; //Preferencias de trabajador
            vector<Solucion> listaSoluciones = it->second; //Soluciones por trabador
            Preferencia preferencia = getPreferencia(listaPreferencias, dia, it->first);
            Solucion solucion = getSolucion(listaSoluciones, dia, it->first);
            if(solucion.getTurno() != preferencia.getTurno()) totalPenalidades += preferencia.getPeso();
        }
    }

    //Penalizar limite de dias trabajados
    //Instancia podría ser parametros
    map<int, vector<Solucion>>::iterator it;
    for(it = solucion.begin(); it != solucion.end(); it++) {
        //it->first es int, trabajador
        //it->second es vector<Solucion>, soluciones
        vector<Solucion> listaSoluciones = it->second; //Soluciones por trabajador
        //contar cantidad de turnos no libres
        int cantidadTurnosTrabajo = 0;
        for (int i = 0; i < listaSoluciones.size(); ++i) {
            if (listaSoluciones[i].getTurno() != 4) cantidadTurnosTrabajo++;
        }
        //workedSequence viene de instancia
        if (cantidadTurnosTrabajo < instancia.getMinAsignaciones() || cantidadTurnosTrabajo > instancia.getMaxAsignaciones())
            totalPenalidades += config.getValorPenalizacionBlanda();
    }

    return totalPenalidades;
}

int CalidadSolucion::calcularRestriccDuras(Instancia instancia, map<int, vector<Solucion>> solucion) {
    Config config;
    //workedSequence ==> (min,max)_asig_periodo
    int totalPenalidades = 0;
    
    //Penalizar turnos consecutivos
    map<int, vector<Solucion>>::iterator it;
    for(it = solucion.begin(); it != solucion.end(); it++) {
        //it->first es int, trabajador
        //it->second es vector<Solucion>, soluciones
        vector<Solucion> listaSoluciones = it->second; //Soluciones por trabajador
        int turnoAnterior = 0;
        for (int i = 0; i < listaSoluciones.size(); ++i) {
            int turnoActual = listaSoluciones[i].getTurno();
            if ((turnoAnterior != 0) && (turnoAnterior != turnoActual)) {
                //Penalizamos el día
                if (turnoActual == 2 && turnoAnterior == 3) totalPenalidades += config.getValorPenalizacionDura();
                if (turnoActual == 1 && turnoAnterior == 3) totalPenalidades += config.getValorPenalizacionDura();
            }
            turnoAnterior = turnoActual;
        }
    }

    //Penalizar turnos (minimos y maximos, cantidad de turnos consecutivos)
    map<int, vector<Solucion>>::iterator it;
    for(it = solucion.begin(); it != solucion.end(); it++) {
        //it->first es int, trabajador
        //it->second es vector<Solucion>, soluciones
        vector<Solucion> listaSoluciones = it->second; //Soluciones por trabajador
        
        //Contamos la cantidad de cada tipo de turno
        map<int, int> cantidadTiposTurnos;
        cantidadTiposTurnos[1] = 0;
        cantidadTiposTurnos[2] = 0;
        cantidadTiposTurnos[3] = 0;
        cantidadTiposTurnos[4] = 0;
        for (int i = 0; i < listaSoluciones.size(); ++i) {
            int turno = listaSoluciones[i].getTurno();
            cantidadTiposTurnos[turno]++;
        }
        //Penalizamos los limites minimos y maximos de cada tipo de turno
        //Turno 1: mañana
        if (cantidadTiposTurnos[1] < instancia.getAsigPorTurno()[1][0] || cantidadTiposTurnos[1] > instancia.getAsigPorTurno()[1][1]) totalPenalidades += config.getValorPenalizacionDura();
        //Turno 2: tarde
        if (cantidadTiposTurnos[2] < instancia.getAsigPorTurno()[2][0] || cantidadTiposTurnos[2] > instancia.getAsigPorTurno()[2][1]) totalPenalidades += config.getValorPenalizacionDura();
        //Turno 3: noche
        if (cantidadTiposTurnos[3] < instancia.getAsigPorTurno()[3][0] || cantidadTiposTurnos[3] > instancia.getAsigPorTurno()[3][1]) totalPenalidades += config.getValorPenalizacionDura();
        //Turno 4: descanso
        if (cantidadTiposTurnos[4] < instancia.getAsigPorTurno()[4][0] || cantidadTiposTurnos[4] > instancia.getAsigPorTurno()[4][1]) totalPenalidades += config.getValorPenalizacionDura();

        int turnoManiana = 0;
        int turnoTarde = 0;
        int turnoNoche = 0;
        int turnoLibre = 0;
        int asignaciones = 0;
        int manianaConsecutiva = 0;
        int tardeConsecutiva = 0;
        int nocheConsecutiva = 0;
        int libreConsecutiva = 0;
        int asignacionesConsecutivas = 0;

        for (int i = 0; i < listaSoluciones.size(); ++i) {
            int turno = listaSoluciones[i].getTurno();

            //Contamos asignaciones consecutivas (en general)
            if (turno != 4) {
                asignaciones++;
            } else {
                if (asignaciones > 1) {
                    asignacionesConsecutivas = (asignaciones > asignacionesConsecutivas) ? (asignaciones) : (asignacionesConsecutivas);
                }
                asignaciones = 0;
            }

            //Contamos asignaciones consecutivas del turno mañana
            if (turno == 1) {
                turnoManiana++;
            } else {
                if (turnoManiana > 1) {
                    manianaConsecutiva = (turnoManiana > manianaConsecutiva) ? (turnoManiana) : (manianaConsecutiva);
                }
                turnoManiana = 0;
            }

            //Contamos asignaciones consecutivas del turno tarde
            if (turno == 2) {
                turnoTarde++;
            } else {
                if (turnoTarde > 1) {
                    tardeConsecutiva = (turnoTarde > tardeConsecutiva) ? (turnoTarde) : (tardeConsecutiva);
                }
                turnoTarde = 0;
            }

            //Contamos asignaciones consecutivas del turno noche
            if (turno == 3) {
                turnoNoche++;
            } else {
                if (turnoNoche > 1) {
                    nocheConsecutiva = (turnoNoche > nocheConsecutiva) ? (turnoNoche) : (nocheConsecutiva);
                }
                turnoNoche = 0;
            }

            //Contamos asignaciones consecutivas del turno libre
            if (turno == 4) {
                turnoLibre++;
            } else {
                if (turnoLibre > 1) {
                    libreConsecutiva = (turnoLibre > libreConsecutiva) ? (turnoLibre) : (libreConsecutiva);
                }
                turnoLibre = 0;
            }

            //Ahora, calculamos las penalidades
            if (asignacionesConsecutivas < instancia.getMinAsignacionesConsecutivas() || asignacionesConsecutivas > instancia.getMaxAsignacionesConsecutivas())
                totalPenalidades += config.getValorPenalizacionDura();
            
            if (manianaConsecutiva < instancia.getAsigConsecPorTurno()[1][0] || manianaConsecutiva > instancia.getAsigConsecPorTurno()[1][1])
                totalPenalidades += config.getValorPenalizacionDura();
            
            if (tardeConsecutiva < instancia.getAsigConsecPorTurno()[2][0] || tardeConsecutiva > instancia.getAsigConsecPorTurno()[2][1])
                totalPenalidades += config.getValorPenalizacionDura();
            
            if (nocheConsecutiva < instancia.getAsigConsecPorTurno()[3][0] || nocheConsecutiva > instancia.getAsigConsecPorTurno()[3][1])
                totalPenalidades += config.getValorPenalizacionDura();
            
            if (libreConsecutiva < instancia.getAsigConsecPorTurno()[4][0] || libreConsecutiva > instancia.getAsigConsecPorTurno()[4][1])
                totalPenalidades += config.getValorPenalizacionDura();
            
        }
    }
    return totalPenalidades;
}

Preferencia CalidadSolucion::getPreferencia(vector<Preferencia> listaPreferencias, int dia, int trabajador) {
    for(int i = 0; i < listaPreferencias.size(); ++i) {
        if (listaPreferencias[i].getTrabajador() == trabajador && listaPreferencias[i].getDia() == dia) return listaPreferencias[i];
    }
}

Solucion CalidadSolucion::getSolucion(vector<Solucion> listaSoluciones, int dia, int trabajador) {
    for(int i = 0; i < listaSoluciones.size(); ++i) {
        if (listaSoluciones[i].getTrabajador() == trabajador && listaSoluciones[i].getDia() == dia) return listaSoluciones[i];
    }
}