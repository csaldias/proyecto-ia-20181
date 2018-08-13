#include "sa.h"
#include <cmath>

float SimulatedAnnealing::probAceptacion(int costoAtual, int costoNuevaSolucion, float temperatura) {
    if (costoAtual > costoNuevaSolucion) {
		return 1.0;
	}

	return exp((costoAtual - costoNuevaSolucion) / temperatura);
}