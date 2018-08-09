#include "nsp.h"
#include "temperatura.h"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    Temperatura temperatura(100.0f, 0.999f);
    for (int i = 0; i < 100; i++) {
        cout << "Temperatura:" << temperatura.get_temp() << endl;
    }
    return 0;
}