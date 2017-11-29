#include <iostream>
#include "src/datahandle.hh"

int main(int argc, char *argv[]) {
    DataHandle data("data.dat", "index.dat", "log.dat");

//    RegisterParser::Register_t reg;
//    reg.id = 10;
//    memcpy(reg.titulo, "Titulo 10", 9);
//    memcpy(reg.genero, "Genero 10", 9);
//    data.insert(reg);

//    data.printBTree();

//    reg.id = 12;
//    memcpy(reg.titulo, "Titulo 12", 9);
//    memcpy(reg.genero, "Genero 12", 9);
//    data.insert(reg);

//    data.printBTree();

//    reg.id = 11;
//    memcpy(reg.titulo, "Titulo 11", 9);
//    memcpy(reg.genero, "Genero 11", 9);
//    data.insert(reg);

//    data.printBTree();
//    data.search(5);
//    data.printBTree();

    return 0;
}
