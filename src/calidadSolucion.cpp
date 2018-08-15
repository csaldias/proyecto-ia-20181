#include "calidadSolucion.h"

#include "preferencia.h"
#include "solucion.h"
#include "instancia.h"
#include "demanda.h"
#include "config.h"

#include <map>
#include <vector>
#include <iostream>

using namespace std;

int CalidadSolucion::calcular(Instancia instancia, map<int, vector<Preferencia> > preferencias, map<int, vector<Solucion> > solucion) {
    int costoRestriccBlandas = calcularRestriccBlandas(instancia, preferencias, solucion);
    int costoRestriccDuras = calcularRestriccDuras(instancia, solucion);
    return costoRestriccBlandas + costoRestriccDuras;
}

int CalidadSolucion::calcularRestriccBlandas(Instancia instancia, map<int, vector<Preferencia> > preferencias, map<int, vector<Solucion> > solucion) {
    Config config;
    int totalPenalidades = 0;

    //Penalizar preferencias
    for(int dia = 1; dia <= instancia.getCantDias(); dia++) {
        map<int, vector<Solucion> >::iterator it;
        for(it = solucion.begin(); it != solucion.end(); it++) {
            //it->first es int, trabajador
            int trabador = it->first;
            //it->second es vector<Solucion>, soluciones
            vector<Preferencia> listaPreferencias = preferencias[trabador]; //Preferencias de trabajador
            vector<Solucion> listaSoluciones = it->second; //Soluciones por trabador
            Solucion solucion = getSolucion(listaSoluciones, dia);
            Preferencia preferencia = getPreferencia(listaPreferencias, dia, it->first, solucion.getTurno());
            totalPenalidades += preferencia.getPeso();
        }
    }

    //Penalizar limites de dias trabajados
    map<int, vector<Solucion> >::iterator it;
    for(it = solucion.begin(); it != solucion.end(); it++) {
        //it->first es int, trabajador
        //it->second es vector<Solucion>, soluciones
        vector<Solucion> listaSoluciones = it->second; //Soluciones por trabajador
        //contar cantidad de turnos no libres
        int cantidadTurnosTrabajo = 0;
        for (int i = 0; i < listaSoluciones.size(); ++i) {
            if (listaSoluciones[i].getTurno() != 4) cantidadTurnosTrabajo++;
        }
        if (cantidadTurnosTrabajo < instancia.getMinAsignaciones() || cantidadTurnosTrabajo > instancia.getMaxAsignaciones())
            totalPenalidades += config.getValorPenalizacionBlanda();
    }

    return totalPenalidades;
}

int CalidadSolucion::calcularRestriccDuras(Instancia instancia, map<int, vector<Solucion> > solucion) {
    Config config;
    int totalPenalidades = 0;
    
    map<int, vector<Solucion> >::iterator it;
    vector<Solucion>::iterator it2;
    //Penalizar cobertura de demanda por turno por día
    for (int dia = 1; dia <= instancia.getCantDias(); dia++) {
        for (int turno = 1; turno <= instancia.getCantTurnos(); turno++) {
            //Demanda esperada para este dia/turno
            int demandaEsperada = instancia.getDemandaDiaTurno(dia, turno).getCantidadRequerida();

            //Oferta para este dia/turno
            int oferta = 0;
            for(it = solucion.begin(); it != solucion.end(); it++) {
                for(it2 = (it->second).begin(); it2 != (it->second).end(); it2++) {
                    if ((*it2).getDia() == dia && (*it2).getTurno() == turno) {
                        oferta++;
                        break;
                    }
                }
            }

            if (demandaEsperada > oferta) totalPenalidades += config.getValorPenalizacionDura() * (demandaEsperada - oferta);
        }
    }
    
    //Penalizar turnos consecutivos
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
                if (turnoActual == 1 && turnoAnterior == 2) totalPenalidades += config.getValorPenalizacionDura();
            }
            turnoAnterior = turnoActual;
        }
    }
    
    //Penalizar cantidad total max y min de turnos
    map<int, int> cantidadTiposTurnos;
    for(it = solucion.begin(); it != solucion.end(); it++) {
        //it->first es int, trabajador
        //it->second es vector<Solucion>, soluciones
        vector<Solucion> listaSoluciones = it->second; //Soluciones por trabajador
        
        //Contamos la cantidad de cada tipo de turno
        cantidadTiposTurnos[1] = 0;
        cantidadTiposTurnos[2] = 0;
        cantidadTiposTurnos[3] = 0;
        cantidadTiposTurnos[4] = 0;
        for (int i = 0; i < listaSoluciones.size(); ++i) {
            int turno = listaSoluciones[i].getTurno();
            cantidadTiposTurnos[turno]++;
        }
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

    //penalizar turnos (cantidad de dias consecutivos de trabajo)
    for(it = solucion.begin(); it != solucion.end(); it++) {
        //it->first es int, trabajador
        //it->second es vector<Solucion>, soluciones
        vector<Solucion> listaSoluciones = it->second; //Soluciones por trabajador

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
                if (asignaciones > 0) {
                    asignacionesConsecutivas = (asignaciones > asignacionesConsecutivas) ? (asignaciones) : (asignacionesConsecutivas);
                }
                asignaciones = 0;
            }

            //Contamos asignaciones consecutivas del turno mañana
            if (turno == 1) {
                turnoManiana++;
            } else {
                if (turnoManiana > 0) {
                    manianaConsecutiva = (turnoManiana > manianaConsecutiva) ? (turnoManiana) : (manianaConsecutiva);
                }
                turnoManiana = 0;
            }

            //Contamos asignaciones consecutivas del turno tarde
            if (turno == 2) {
                turnoTarde++;
            } else {
                if (turnoTarde > 0) {
                    tardeConsecutiva = (turnoTarde > tardeConsecutiva) ? (turnoTarde) : (tardeConsecutiva);
                }
                turnoTarde = 0;
            }

            //Contamos asignaciones consecutivas del turno noche
            if (turno == 3) {
                turnoNoche++;
            } else {
                if (turnoNoche > 0) {
                    nocheConsecutiva = (turnoNoche > nocheConsecutiva) ? (turnoNoche) : (nocheConsecutiva);
                }
                turnoNoche = 0;
            }

            //Contamos asignaciones consecutivas del turno libre
            if (turno == 4) {
                turnoLibre++;
            } else {
                if (turnoLibre > 0) {
                    libreConsecutiva = (turnoLibre > libreConsecutiva) ? (turnoLibre) : (libreConsecutiva);
                }
                turnoLibre = 0;
            }
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
    return totalPenalidades;
}

Preferencia CalidadSolucion::getPreferencia(vector<Preferencia> listaPreferencias, int dia, int trabajador, int turno) {
    for(int i = 0; i < listaPreferencias.size(); ++i) {
        if (listaPreferencias[i].getTrabajador() == trabajador && listaPreferencias[i].getDia() == dia && listaPreferencias[i].getTurno() == turno) return listaPreferencias[i];
    }
}

Solucion CalidadSolucion::getSolucion(vector<Solucion> listaSoluciones, int dia) {
    for(int i = 0; i < listaSoluciones.size(); ++i) {
        if (listaSoluciones[i].getDia() == dia) return listaSoluciones[i];
    }
}