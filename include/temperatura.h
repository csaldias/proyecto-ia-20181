#ifndef __temperatura_h__
#define __temperatura_h__

class Temperatura {
    private:
        float temp;
        float decay_factor;
    public:
        Temperatura(float _temp, float _decay_factor): temp(_temp), decay_factor(_decay_factor) {}
        float getTemp(void);
        void coolDown(void);
        void setTemp(float newTemp);
};

#endif