#ifndef __solucion_h__
#define __solucion_h__

class Solucion {
    private:
        int trabajador;
        int dia;
        int turno;
    public:
        Solucion(int _trabajador, int _dia, int _turno):
            trabajador(_trabajador),
            dia(_dia),
            turno(_turno) {}
        int getTrabajador(void);
        int getDia(void);
        int getTurno(void);
        void setTrabajador(int trabajador);
        void setDia(int dia);
        void setTurno(int turno);
};

#endif