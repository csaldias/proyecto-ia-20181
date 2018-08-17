# proyecto-ia-20181
Resolución del Nurse Scheduling Problem mediante la técnica de Simmulated Annealing. Proyecto para el curso de Inteligencia Artificial 2018-1.

#Compilación
Compilar con (desde la raíz de la carpeta del proyecto): g++ src/*.cpp -o nsp -I include/ -Wall

#Uso
./nsp PATH_GEN PATH_NSP
donde:
    -PATH_GEN corresponde a la ubicación del archivo de restricciones .gen a utilizar
    -PATH_NSP corresponde a la ubicación del archivo de instancia .nsp a utilizar

Por ejemplo, para utilizar el archivo de restricción 6.gen, junto con la instancia N25/10.nsp, el comando es: (desde la raíz de la carpeta del proyecto)

./nsp dataset/Cases/6.gen dataset/N25/10.nsp