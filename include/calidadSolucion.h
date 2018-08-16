#ifndef __calidadSolucion_h__
#define __calidadSolucion_h__

#include "preferencia.h"
#include "solucion.h"
#include "instancia.h"
#include "demanda.h"
#include "puntaje.h"

#include <map>
#include <vector>

using namespace std;

class CalidadSolucion {
    public:
        map<int, Puntaje> calcular(Instancia instancia, map<int, vector<Preferencia> > preferencias, map<int, vector<Solucion> > solucion);
        map<int, Puntaje> calcularRestriccBlandas(Instancia instancia, map<int, vector<Preferencia> > preferencias, map<int, vector<Solucion> > solucion);
        map<int, Puntaje> calcularRestriccDuras(Instancia instancia, map<int, vector<Solucion> > solucion);
    private:
        Preferencia getPreferencia(vector<Preferencia> listaPreferencias, int dia, int trabajador, int turno);
        Solucion getSolucion(vector<Solucion> listaSoluciones, int dia);
};

#endif