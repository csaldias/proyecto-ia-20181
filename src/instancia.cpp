#include "instancia.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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
         istringstream iss(linea);
         if (linea.length() < 2) continue; //Nos saltamos las lineas vacias
         //Dependiendo de la linea, lo que significa cada numero
         if (numeroLinea == 1) {
             //Leemos trabajadores, dias y turnos
             int turnosNsp = 0;
             int diasNsp = 0;
             iss >> this->cantTrabajadores >> diasNsp >> turnosNsp;
             if (diasNsp != this->cantDias || turnosNsp != this->cantTurnos) {
                 cerr << "ERROR: La cantidad de dias y turnos no coincide entre los archivos GEN y NSP." << endl;
                 exit(1);
             }
         }
         if (numeroLinea >= 2 && numeroLinea <= (1 + this->cantDias)) {
             //Leemos demanda de cada turno por dia (matriz demandas)
             iss >> a >> b >> c >> d;
             this->matrizDemandas[numeroLinea - 1].push_back(Demanda(numeroLinea-1, 1, a));
             this->matrizDemandas[numeroLinea - 1].push_back(Demanda(numeroLinea-1, 2, b));
             this->matrizDemandas[numeroLinea - 1].push_back(Demanda(numeroLinea-1, 3, c));
             this->matrizDemandas[numeroLinea - 1].push_back(Demanda(numeroLinea-1, 4, d));
         }
         if (numeroLinea > (1 + this->cantDias)) {
             //Leemos preferencias de turnos de cada trabajador por dia (matriz preferencias)
             int idTrabajador = numeroLinea - (1 + this->cantDias);
             for (int dia = 1; dia <= this->cantDias; dia++) {
                 iss >> a >> b >> c >> d;
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

map<int, vector<Preferencia> > Instancia::getPreferencias(void) {
    return this->matrizPreferencias;
}

int Instancia::getNumeroDias(void) {
    return this->cantDias;
}
map<int, vector<Solucion> > Instancia::generarSolucion() {
    //codigo
}
