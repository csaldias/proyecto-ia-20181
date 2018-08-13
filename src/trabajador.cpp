#include "trabajador.h"

int Trabajador::getID() {
    return this->id;
}

bool Trabajador::operator== (const Trabajador& trab2) {
    if(this->id == trab2.id) return true;
    return false;
}