#ifndef __calidadSolucion_h__
#define __calidadSolucion_h__

#include "preferencia.h"
#include "solucion.h"
#include "instancia.h"
#include "demanda.h"

#include <map>
#include <vector>

using namespace std;

class CalidadSolucion {
    public:
        int calcular(Instancia instancia, map<int, vector<Preferencia>> preferencias, map<int, vector<Solucion>> solucion);
        int calcularRestriccBlandas(Instancia instancia, map<int, vector<Preferencia>> preferencias, map<int, vector<Solucion>> solucion);
        int calcularRestriccDuras(Instancia instancia, map<int, vector<Solucion>> solucion);
    private:
        Preferencia getPreferencia(vector<Preferencia> listaPreferencias, int dia, int trabajador);
        Solucion getSolucion(vector<Solucion> listaSoluciones, int dia, int trabajador);
};

#endif