#include "config.h"

int Config::getValorPenalizacionBlanda(void) {
    return this->penalizacionBlanda;
}
int Config::getValorPenalizacionDura(void) {
    return this->penalizacionDura;
}
float Config::getTemperaturaInicial(void) {
    return this->temperaturaInicial;
}
float Config::getTemperaturaMinima(void) {
    return this->temperaturaMinima;
}
float Config::getTasaEnfriamiento(void) {
    return this->tasaEnfriamiento;
}
int Config::getNumeroIteraciones(void) {
    return this->numeroIteraciones;
}