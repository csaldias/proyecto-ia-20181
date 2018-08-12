#ifndef __trabajador_h__
#define __trabajador_h__

class Trabajador {
    private:
        int id;
    public:
        Trabajador(int _id): id(_id) {}
        int getID(void);
        bool operator== (const Trabajador& trab2);
};

#endif