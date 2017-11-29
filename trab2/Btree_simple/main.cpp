#include <iostream>
#include "src/datahandle.hh"

int main(int argc, char *argv[]) {
    DataHandle data("data.dat", "index.dat", "log.dat");

    RegisterParser::Register_t reg;
    reg.id = 6;
    memcpy(reg.titulo, "Titulo 6", 9);
    memcpy(reg.genero, "Genero 6", 9);
    data.insert(reg);
//    data.rebuildIndexFile();

    return 0;
}
