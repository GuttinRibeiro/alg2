#include<stdio.h>
#include<string.h>

#define MAX 10

struct endereco {
    char nome[15];
    char sobrenome[15];
    char rua[25];
    int numero;
} e[MAX] = {{"Joao", "Souza", "XV Novembro", 151},
            {"Eduardo", "Martins", "Sao Sebastiao", 201},
            {"Maria", "Caldas", "Aquidabam", 45},
            {"Janaina", "Souto", "Narciso Romano", 5567},
            {"Elisa", "Milare", "Coronal Spinola", 420},
            {"Claudia", "Ferraz", "25 de marco", 311},
            {"Diogo", "Oliveira", "Santiago Sanches", 1000},
            {"Rafael", "Soares", "Sgt. Menezes", 501},
            {"Cristiano", "Facelli", "Larga", 40},
            {"Paulo", "Kuhl", "13 de maio", 1004}};

int camp_var_reg_var(endereco e, char *buffer) {
     sprintf(buffer, "%s|%s|%s|%d|", e.nome, e.sobrenome, e.rua, e.numero);
     return strlen(buffer);
}

char *parser(char *buffer, int *pos) {
    int posi = *pos;

    while(buffer[*pos]!='|')
        (*pos)++;
    buffer[*pos] = '\0';
    (*pos)++;
    return &buffer[posi];
}

int main() {
    FILE *fd;

    struct endereco e2;
    char buffer[1000];
    int i, pos;
    char size;

    fd = fopen("agenda.dad", "w+b");
    for (i=0; i<MAX; i++) {
        size = camp_var_reg_var(e[i], buffer);
        fwrite(&size, sizeof(size), 1, fd);
        fwrite(buffer, size, 1, fd);
    }

    fseek(fd, 0, 0);
    while (fread(&size, sizeof(size), 1, fd)) {
        fread(buffer, size, 1, fd);
        pos = 0;
        strcpy(e2.nome, parser(buffer, &pos));
        strcpy(e2.sobrenome, parser(buffer, &pos));
        strcpy(e2.rua, parser(buffer, &pos));
        sscanf(parser(buffer, &pos), "%d", &e2.numero);
        printf("%s %s %s %d\n", e2.nome, e2.sobrenome, e2.rua, e2.numero);
    }
    fclose(fd);

}
