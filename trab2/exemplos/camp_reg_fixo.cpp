#include<stdio.h>
#include<string>

#define MAX 10

int main() {
    FILE *fd;
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
    
    struct endereco e2[MAX];
    int i;
    
    fd = fopen("fixo.dad", "w+b");
    fwrite(e, sizeof(endereco), 10, fd);
    
    fseek(fd, 0, 0);
    fread(&e2, sizeof(endereco), 10, fd);    

    for (i=0; i<MAX; i++) {
        printf("%s %s %s %d\n", e2[i].nome, e2[i].sobrenome, e2[i].rua, e2[i].numero);
    }        
    fclose(fd);

    getchar();
}
