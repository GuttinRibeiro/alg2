#ifndef COMMONHEADER_H
#define COMMONHEADER_H

#include <iostream>
#include <fstream>
#include <string.h>

#define INVALID_OFFSET (-1)
#define INVALID_RRN (-1)

#define ORDER (5)
#define TITLE_LENGTH (30)
#define GENDER_LENGTH (20)

// the size of struct + delimiters
#define BUFFER_SIZE (sizeof(Register_t) + 4)
// size of the struct
#define NODE_SIZE (sizeof(node_t))
// size of key
#define KEY_SIZE (sizeof(key_t))

typedef long offset_t;
typedef long rrn_t;
typedef unsigned long bufferptr_t;

#endif // COMMONHEADER_H
