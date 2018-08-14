#ifndef __asignacion_h__
#define __asignacion_h__

class Asignacion {
    private:
        int trabajador;
        int tipoAsignacion;
    public:
        Asignacion(int _trabajador, int _tipoAsignacion): trabajador(_trabajador), tipoAsignacion(_tipoAsignacion) {}
        int getTrabajador();
        int getTipoAsignacion();
};

#endif