#include "instancia.h"
#include "asignacion.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cctype>
#include <locale>
#include <functional> 

using namespace std;

 int Instancia::loadInstance(const char pathGenFile[], const char pathNspFile[]) {
     //Primero, leemos el archivo .gen
     cout << "Leyendo Archivo GEN: " << pathGenFile << endl;
     ifstream genFile;
     genFile.open(pathGenFile);
     if (!genFile) {
         cerr << "Error al abrir archivo GEN: " << pathGenFile << endl;
         exit(1);
     }
     int numeroLinea = 1;
     string linea = "";
     int a;
     int b;
     int c;
     int d;

     while (getline(genFile, linea)) {
         istringstream iss(linea);
         if (linea.length() < 2) continue; //Nos saltamos las lineas vacias
         //Dependiendo de la linea, lo que significa cada numero
         if (numeroLinea == 1) {
             //Leemos dias y turnos
             iss >> this->cantDias >> this->cantTurnos;
             if (this->cantTurnos != 4){
                 cerr << "ERROR: este programa no considera una cantidad de turnos distinta de 4." << endl;
                 exit(1);
             }
         }
         if (numeroLinea == 2) {
             //Leemos minimo y maximo de asignaciones de turnos
             iss >> this->minAsignaciones >> this->maxAsignaciones;
         }
         if (numeroLinea == 3) {
             //Leemos nimimo y maximo de asignaciones consecutivas de turnos
             iss >> this->minAsignacionesConsecutivas >> this->maxAsignacionesConsecutivas;
         }
         if (numeroLinea >= 4) {
             //Leemos minimos y maximos por turno
             iss >> a >> b >> c >> d;
             this->asigPorTurno[numeroLinea - 3].push_back(c);
             this->asigPorTurno[numeroLinea - 3].push_back(d);
             this->asigConsecPorTurno[numeroLinea - 3].push_back(a);
             this->asigConsecPorTurno[numeroLinea - 3].push_back(b);
         }
         numeroLinea++;
         
     }
     genFile.close();
     
     //Ahora, leemos el archivo .nsp
     cout << "Leyendo Archivo NSP: " << pathNspFile << endl;
     ifstream nspFile;
     nspFile.open(pathNspFile);
     if (!nspFile) {
         cerr << "Error al abrir archivo NSP: " << pathNspFile << endl;
         exit(1);
     }
     numeroLinea = 1;
     linea = "";

     while (getline(nspFile, linea)) {
         istringstream iss2(linea);
         if (linea.length() < 2) continue; //Nos saltamos las lineas vacias
         //Dependiendo de la linea, lo que significa cada numero
         if (numeroLinea == 1) {
             //Leemos trabajadores, dias y turnos
             int turnosNsp = 0;
             int diasNsp = 0;
             iss2 >> this->cantTrabajadores >> diasNsp >> turnosNsp;
             if (diasNsp != this->cantDias || turnosNsp != this->cantTurnos) {
                 cerr << "ERROR: La cantidad de dias y turnos no coincide entre los archivos GEN y NSP." << endl;
                 exit(1);
             }
         }
         if (numeroLinea >= 2 && numeroLinea <= (1 + this->cantDias)) {
             //Leemos demanda de cada turno por dia (matriz demandas)
             iss2 >> a >> b >> c >> d;
             //cout << "Dia " << numeroLinea - 1 << ": " << a << "-" << b << "-" << c << "-" << d << endl;
             this->matrizDemandas[numeroLinea - 1].push_back(Demanda(numeroLinea-1, 1, a));
             this->matrizDemandas[numeroLinea - 1].push_back(Demanda(numeroLinea-1, 2, b));
             this->matrizDemandas[numeroLinea - 1].push_back(Demanda(numeroLinea-1, 3, c));
             this->matrizDemandas[numeroLinea - 1].push_back(Demanda(numeroLinea-1, 4, d));
         }
         if (numeroLinea > (1 + this->cantDias)) {
             //Leemos preferencias de turnos de cada trabajador por dia (matriz preferencias)
             int idTrabajador = numeroLinea - (1 + this->cantDias);
             for (int dia = 1; dia <= this->cantDias; dia++) {
                 iss2 >> a >> b >> c >> d;
                 this->matrizPreferencias[idTrabajador].push_back(Preferencia(idTrabajador, 1, dia, a)); //trabajador, turno, dia, peso
                 this->matrizPreferencias[idTrabajador].push_back(Preferencia(idTrabajador, 2, dia, b));
                 this->matrizPreferencias[idTrabajador].push_back(Preferencia(idTrabajador, 3, dia, c));
                 this->matrizPreferencias[idTrabajador].push_back(Preferencia(idTrabajador, 4, dia, d));
             }
         }
         numeroLinea++; 
     }
     nspFile.close();
     cout << "Informacion cargada." << endl;
     return 0;
}

map<int, vector<Solucion> > Instancia::generarSolucion() {
    //Primero, asignamos los días y turnos de trabajo de forma aleatoria
    //  dia     turno   enfermero
    map<int, map<int, vector<int> > > asignaciones;
    //cout << "generarSolucion: Obteniendo asignaciones..." << endl;
    // trabajador  {trabajador, dia, turno, asignacion}
    map<int, vector<Solucion> > soluciones;
    srand(unsigned(time(0)));

    for (int dia = 1; dia <= this->cantDias; dia++) {
        //Designar turnos por dia

        vector<int> trabajadores;
        for (int i = 1; i <= this->cantTrabajadores; i++) {
            trabajadores.push_back(i);
        }
        //Obtenemos la demanda total del dia
        int demandaTotal = 0;
        for (int turno = 1; turno <= this->cantTurnos; turno++) {
            demandaTotal += this->matrizDemandas[dia][turno].getCantidadRequerida();
        }

        map<int, vector<int> > trabajadoresSeleccionados;
        for (int turno = 1; turno <= this->cantTurnos; turno++) {
            //Seleccionar enfermeros de forma aleatoria para cubrir la demanda
            int demandaTurno = this->matrizDemandas[dia][turno].getCantidadRequerida();
            random_shuffle(trabajadores.begin(), trabajadores.end());

            for (int i = 0; i < demandaTurno; i++) {
                trabajadoresSeleccionados[turno].push_back(trabajadores.back());
                trabajadores.pop_back();
            }
        }
        asignaciones[dia] = trabajadoresSeleccionados;
    }

    //cout << "generarSolucion: Generando matriz soluciones..." << endl;

    //Ahora, creamos la matriz de soluciones en base a esta asignacion
    for (int dia = 1; dia <= this->cantDias; dia++) {
        for (int turno = 1; turno <= this->cantTurnos; turno++) {
            vector<int> asigDiaTurno = asignaciones[dia][turno];
            vector<int>::iterator it;
            for(it = asigDiaTurno.begin(); it != asigDiaTurno.end(); it++) {
                soluciones[*it].push_back(Solucion(*it, dia, turno, 0));
            }
        }
    }
    return soluciones;
}

int Instancia::getCantTrabajadores(void) {
    return this->cantTrabajadores;
}
int Instancia::getCantDias(void) {
    return this->cantDias;
}
int Instancia::getCantTurnos(void) {
    return this->cantTurnos;
}
int Instancia::getMinAsignaciones(void) {
    return this->minAsignaciones;
}
int Instancia::getMaxAsignaciones(void) {
    return this->maxAsignaciones;
}
int Instancia::getMinAsignacionesConsecutivas(void) {
    return this->minAsignacionesConsecutivas;
}
int Instancia::getMaxAsignacionesConsecutivas(void) {
    return this->maxAsignacionesConsecutivas;
}
map<int, vector<int> > Instancia::getAsigPorTurno(void) {
    return (this->asigPorTurno);
}
map<int, vector<int> > Instancia::getAsigConsecPorTurno(void) {
    return this->asigConsecPorTurno;
}
map<int, vector<Preferencia> > Instancia::getPreferencias(void) {
    return this->matrizPreferencias;
}
map<int, vector<Demanda> > Instancia::getDemandas(void) {
    return this->matrizDemandas;
}
vector<Demanda> Instancia::getDemandaDia(int dia) {
    return this->matrizDemandas[dia];
}
Demanda Instancia::getDemandaDiaTurno(int dia, int turno) {
    return this->matrizDemandas[dia][turno-1];
}