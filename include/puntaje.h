#ifndef __puntaje_h__
#define __puntaje_h__

class Puntaje{
    private:
        int cantRestricciones;
        int puntaje;
    public:
        Puntaje() {
            cantRestricciones = 0;
            puntaje = 0;
        }
        int getPuntaje();
        int getCantRestricciones();
        void setPuntaje(int nuevoPuntaje);
        void setCantRestricciones(int nuevaCantRestricciones);
        void addPuntaje(int adicional);
        void addRestricciones(int adicional);
};

#endif