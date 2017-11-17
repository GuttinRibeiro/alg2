#include <iostream>
#include "src/registerparser.hh"
#include "src/dataparser.hh"

int main(int argc, char *argv[]) {

    RegisterParser *regParser = new RegisterParser("");
    DataParser *dataParser = new DataParser("data.dat", regParser);


    return 0;
}
