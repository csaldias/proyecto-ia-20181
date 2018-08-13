#ifndef __config_h__
#define __config_h__

class Config {
    private:
        //Penalizaciones
        int penalizacionBlanda;
        int penalizacionDura;
        //Datos para Algoritmo
        float temperaturaInicial;
        float temperaturaMinima;
        float tasaEnfriamiento;
        int numeroIteraciones;
    public:
        Config() {
            //Penalizaciones
            penalizacionBlanda = 100;
            penalizacionDura = 1000;
            //Datos para Algoritmo
            temperaturaInicial = 100.0f;
            temperaturaMinima = 0.0f;
            tasaEnfriamiento = 0.999f;
            numeroIteraciones = 5000;
        }
        int getValorPenalizacionBlanda(void);
        int getValorPenalizacionDura(void);
        float getTemperaturaInicial(void);
        float getTemperaturaMinima(void);
        float getTasaEnfriamiento(void);
        int getNumeroIteraciones(void);
};

#endif