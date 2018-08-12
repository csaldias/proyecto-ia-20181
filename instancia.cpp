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
         cout << "-------------------- Linea " << numeroLinea << " --------------------" << endl;
         if (numeroLinea == 1) {
             //Leemos dias y turnos
             iss >> this->cantDias >> this->cantTurnos;
             cout << "Dias: " << this->cantDias << endl;
             cout << "Turnos: " << this->cantTurnos << endl;
         }
         if (numeroLinea == 2) {
             //Leemos minimo y maximo de asignaciones de turnos
             iss >> this->minAsignaciones >> this->maxAsignaciones;
             cout << "Asignaciones minimas: " << this->minAsignaciones << endl;
             cout << "Asignaciones maximas: " << this->maxAsignaciones << endl;
         }
         if (numeroLinea == 3) {
             //Leemos nimimo y maximo de asignaciones consecutivas de turnos
             iss >> this->minAsignacionesConsecutivas >> this->maxAsignacionesConsecutivas;
             cout << "Asignaciones consecutivas minimas: " << this->minAsignacionesConsecutivas << endl;
             cout << "Asignaciones consecutivas maximas: " << this->maxAsignacionesConsecutivas << endl;
         }
         if (numeroLinea >= 4) {
             //Leemos minimos y maximos por turno
             iss >> a >> b >> c >> d;
             this->asigPorTurno[numeroLinea - 3].push_back(c);
             this->asigPorTurno[numeroLinea - 3].push_back(d);
             this->asigConsecPorTurno[numeroLinea - 3].push_back(a);
             this->asigConsecPorTurno[numeroLinea - 3].push_back(b);
             
             cout << "Para turno " << numeroLinea - 3 << ":" << endl;
             cout << "Turnos consecutivos: min=" << this->asigConsecPorTurno[numeroLinea - 3][0] << ", max=" << this->asigConsecPorTurno[numeroLinea - 3][1] << endl;
             cout << "Cant de asignaciones: min=" << this->asigPorTurno[numeroLinea - 3][0] << ", max=" << this->asigPorTurno[numeroLinea - 3][1] << endl;
         }
         numeroLinea++;
         
     }
     genFile.close();
     
     //Ahora, leemos el archivo .nsp
     cout << "Archivo .nsp: " << pathNspFile << endl;
    
     return 0;
}

string Instancia::getFileName() {
    //Temporal
    return "Cases/1.gen";
}