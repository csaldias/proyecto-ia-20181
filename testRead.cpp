#include "instancia.h"
#include <iostream>
#include <vector>
#include "demanda.h"

using namespace std;

int main(int argc, char const *argv[])
{
    //codigo
    //cout << "Recibidos " << argc << " argumentos" << endl;
    string pathNsp = "dataset/N100/";
    string ext = ".nsp";

    for (int i = 1; i <= 7290; i++) {
        Instancia instancia;
        string completePath = pathNsp + to_string(i) + ext;
        cout << completePath << endl;
        instancia.loadInstance(argv[1], completePath.c_str());

        //Obtenemos la demanda total del dia
        for(int dia = 1; dia <= instancia.getCantDias(); dia++) {
            int demandaTotal = 0;
            for (int turno = 1; turno <= instancia.getCantTurnos(); turno++) {
                demandaTotal += instancia.getDemandaDiaTurno(dia, turno).getCantidadRequerida();
                //cout << instancia.getDemandaDiaTurno(dia, turno).getCantidadRequerida() << "-";
            }
            //cout << endl;
            if (demandaTotal > instancia.getCantTrabajadores()) cout << "Archivo " << completePath << ", Dia " << dia << ": " << demandaTotal << ">" << instancia.getCantTrabajadores() << endl;
        }
    }
    return 0;
}
