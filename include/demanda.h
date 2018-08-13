#ifndef __demanda_h__
#define __demanda_h__

class Demanda {
    private:
        int dia;
        int turno;
        int cantidadRequerida;
    public:
        Demanda(int _dia, int _turno, int _cantidad): dia(_dia), turno(_turno), cantidadRequerida(_cantidad) {}
        int getCantidadRequerida(void);
        int getTurno(void);
        int getDia(void);
};

#endif