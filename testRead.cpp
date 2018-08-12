#include "instancia.h"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    //codigo
    Instancia instancia;
    cout << "Recibidos " << argc << " argumentos" << endl;
    
    instancia.loadInstance(argv[1], argv[2]);
    return 0;
}
