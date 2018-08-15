#include "solucion.h"

int Solucion::getTrabajador() {
    return this->trabajador;
}

int Solucion::getDia() {
    return this->dia;
}

int Solucion::getTurno() {
    return this->turno;
}

void Solucion::setTrabajador(int trabajador) {
    this->trabajador = trabajador;
}
void Solucion::setDia(int dia) {
    this->dia = dia;
}
void Solucion::setTurno(int turno) {
    this->turno = turno;
}