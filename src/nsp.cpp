#include "nsp.h"
#include "temperatura.h"
#include "instancia.h"
#include "trabajador.h"
#include "solucion.h"
#include "preferencia.h"
#include "calidadSolucion.h"
#include "config.h"
#include "sa.h"

#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char const *argv[])
{
    //Codigo
    chrono::high_resolution_clock::time_point inicio = chrono::high_resolution_clock::now();
    srand(0); //time(NULL) para un random real

    Instancia instancia;
	Config config;
    SimulatedAnnealing sa;
	CalidadSolucion calidadSolucion;

	std::cout << "Resolviendo instancia: " << argv[1] << std::endl;
	std::cout << "Leyendo archivo..." << std::endl;
    instancia.loadInstance(argv[1], argv[2]); //Instancia instancia = parametros.loadInstance();
	//Trabajador-Preferencia
    map<int, vector<Preferencia> > preferencias = instancia.getPreferencias();
	std::cout << "Preferencias listas." << endl;
	//Trabajador-Solucion
	map<int, vector<Solucion> > mejorSolucion;
	int costoMejorSolucion = 10000000;
		
    
	//Trabajador-Solucion
	std::cout << "Generando solucion inicial..." << endl;
	map<int, vector<Solucion> > solucion = instancia.generarSolucion();
	std::cout << "Obteniendo costo..." << endl;
	int costoTotal = calidadSolucion.calcular(instancia, preferencias, solucion);
		
	mejorSolucion = solucion;
	costoMejorSolucion = costoTotal;
		
	std::cout << "Costo de Solucion Incial: " << costoTotal << endl;

    float temperaturaInicial = config.getTemperaturaInicial();
    float tasaEnfriamiento = config.getTasaEnfriamiento();
	float temperaturaMinima = config.getTemperaturaMinima();
    Temperatura temperatura(temperaturaInicial, tasaEnfriamiento);

	std::cout << "Temp Inicial: " << temperatura.getTemp() << ", Temp Minina: " << temperaturaMinima << endl;

	float temperaturaActual = temperatura.getTemp();
	while (temperaturaActual > temperaturaMinima) { //Definir stop criteria
		std::cout << "Temp Actual: " << temperaturaActual << endl;
		int iteraciones = config.getNumeroIteraciones();
		while (iteraciones >= 1) {
            //Trabajador-Solucion
			//std::cout << "------------------------- It " << iteraciones << " ----------------------------" << std::endl;
			//std::cout << "Generando nueva solucion..." << endl;
			map<int, vector<Solucion> > nuevaSolucion = instancia.generarSolucion();
            int costoTotalNuevaSolucion = calidadSolucion.calcular(instancia, preferencias, nuevaSolucion);
			//std::cout << "Costo nueva solucion: " << costoTotalNuevaSolucion << endl;

            float randomValue = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			if (sa.probAceptacion(costoTotal, costoTotalNuevaSolucion, temperaturaActual) > randomValue ){
				costoTotal = costoTotalNuevaSolucion;
				solucion = nuevaSolucion;
			}

			if (costoTotal > costoMejorSolucion) {
				std::cout << "Quitadas" << costoMejorSolucion - costoTotal << "penalizaciones" << std::endl;
				costoMejorSolucion = costoTotal;
				mejorSolucion = solucion;
				temperatura.setTemp(temperaturaInicial);
				iteraciones = config.getNumeroIteraciones();
			}
			iteraciones--;
		}
        temperatura.coolDown();
		temperaturaActual = temperatura.getTemp();
	}

    chrono::high_resolution_clock::time_point fin = chrono::high_resolution_clock::now();

    chrono::duration<double> time_span = chrono::duration_cast< chrono::duration<double> >(fin - inicio);

	std::cout << "Algoritmo terminado" << std::endl;
	std::cout << "Total penalizaciones:" << costoMejorSolucion << std::endl;
	std::cout << "Tiempo transcurrido:" << time_span.count() << std::endl;
		
	//Imprimir a archivo mejor solucion
    return 0;
}