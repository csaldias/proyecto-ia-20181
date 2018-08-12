#ifndef __preferencia_h__
#define __preferencia_h__

class Preferencia{
    private:
        int trabajador;
        int turno;
        int dia;
        int peso;
    public:
        Preferencia(int _trabajador, int _turno, int _dia, int _peso):
            trabajador(_trabajador),
            turno(_turno),
            dia(_dia),
            peso(_peso) {}
        int getTrabajador(void);
        int getTurno(void);
        int getPeso(void);
        int getDia(void);
};

#endif