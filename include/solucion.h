#ifndef __solucion_h__
#define __solucion_h__

class Solucion {
    private:
        int trabajador;
        int dia;
        int turno;
        int asignacion;
    public:
        Solucion(int _trabajador, int _dia, int _turno, int _asignacion):
            trabajador(_trabajador),
            dia(_dia),
            turno(_turno),
            asignacion(_asignacion) {}
        int getTrabajador(void);
        int getDia(void);
        int getTurno(void);
        int getAsignacion(void);
};

#endif