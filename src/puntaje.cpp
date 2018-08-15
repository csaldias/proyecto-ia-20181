#include "puntaje.h"

int Puntaje::getPuntaje() {
    return this->puntaje;
}
int Puntaje::getCantRestricciones(){
    return this->cantRestricciones;
}
void Puntaje::setPuntaje(int nuevoPuntaje){
    this->puntaje = nuevoPuntaje;
}
void Puntaje::setCantRestricciones(int nuevaCantRestricciones){
    this->cantRestricciones = nuevaCantRestricciones;
}
void Puntaje::addPuntaje(int adicional) {
    this->puntaje += adicional;
}
void Puntaje::addRestricciones(int adicional) {
    this->cantRestricciones += adicional;
}