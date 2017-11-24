#include "registerparser.hh"

RegisterParser::RegisterParser() {

}

const char *RegisterParser::registerParser(Register_t reg) {
    // the buffer will be [size of register]|[id]|[title]|[gender]
    // bufferPointer is here ~~~~~~~~~~~~~~~^
    bufferptr_t bufferPointer = sizeof(bufferPointer);

    // delimiter
    _registerBuffer[bufferPointer++] = '|';

    // put id into buffer
    memcpy(&(_registerBuffer[bufferPointer]), &(reg.id), sizeof(reg.id));
    bufferPointer += sizeof(reg.id);
    // delimiter
    _registerBuffer[bufferPointer++] = '|';

    // put title into buffer
    bufferPointer += stringCopy(&(_registerBuffer[bufferPointer]), reg.titulo, TITLE_LENGTH);
    // delimiter
    _registerBuffer[bufferPointer++] = '|';

    // put gender into buffer
    bufferPointer += stringCopy(&(_registerBuffer[bufferPointer]), reg.genero, GENDER_LENGTH);
    // delimiter
    _registerBuffer[bufferPointer++] = '|';

    // put the size of the register into buffer
    memcpy(_registerBuffer, &(bufferPointer), sizeof(bufferPointer));

    return _registerBuffer;
}

RegisterParser::Register_t &RegisterParser::registerParser(const char *reg) {
    bufferptr_t size = 0;
    memcpy(&size, reg, sizeof(size));

    bufferptr_t bufferPointer = sizeof(size) + 1;

    if(size > bufferPointer) {
        memcpy(&(_register.id), &(reg[bufferPointer]), sizeof(_register.id));
        bufferPointer += sizeof(_register.id) + 1;
    }

    if(size > bufferPointer) {
        bufferPointer += stringCopy(_register.titulo, &(reg[bufferPointer]), TITLE_LENGTH);
        bufferPointer += 1; // delimiter
    }

    if(size > bufferPointer) {
        bufferPointer += stringCopy(_register.genero, &(reg[bufferPointer]), GENDER_LENGTH);
        bufferPointer += 1; // delimiter
    }

    return _register;
}

int RegisterParser::stringCopy(char *dest, const char *src, int maxLength) {
    int i = 0;
    while(src[i] != '\0' && i < maxLength) {
        dest[i] = src[i];
        i++;
    }

    dest[i] = '\0';
    return (++i);
}
