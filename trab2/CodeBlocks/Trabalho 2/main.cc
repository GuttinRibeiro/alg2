#include <iostream>
#include <limits>
#include "src/datahandle.hh"

enum { // menu options
    REBUID_INDEX = 1,
    INSERT       = 2,
    SEARCH       = 3,
    REMOVE       = 4,
    PRINT        = 5,
    EXIT         = 6
};

int main() {
    DataHandle data("data.dat", "index.dat", "log.dat");

    int option = -1;

    while(option != EXIT) {
        std::cout << "Menu\n";
        std::cout << "Opcao ..... Descricao\n";
        std::cout << REBUID_INDEX << " ......... Criar arquivo de indice.\n";
        std::cout << INSERT << " ......... Inserir musica.\n";
        std::cout << SEARCH << " ......... Buscar musica.\n";
        std::cout << REMOVE << " ......... Remover musica.\n";
        std::cout << PRINT << " ......... Imprimir arvore-B.\n";
        std::cout << EXIT << " ......... Sair.\n";

        std::cout << "Digite a opcao: ";

        std::cin >> option;

        switch(option) {
            case REBUID_INDEX: {
                data.rebuildIndexFile();
            } break;
            case INSERT: {
                RegisterParser::Register_t reg;

                std::cout << "Digite a ID da musica: ";
                std::cin >> reg.id;

                // "clean" buffer, operator>> dosn't read '\n'
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cout << "Digite o titulo: ";
                std::cin.getline(reg.titulo, TITLE_LENGTH);

                if(std::cin.fail()) {
                    cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                std::cout << "Digite o genero: ";
                std::cin.getline(reg.genero, GENDER_LENGTH);

                if(std::cin.fail()) {
                    cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                data.insert(reg);
            } break;
            case SEARCH: {
                int id = -1;
                std::cout << "Digite o ID a ser buscado: ";
                std::cin >> id;

                data.search(id);
            } break;
            case REMOVE: {
//                std::cout << "Funcao nao implementada.\n";
                int id = -1;
                std::cout << "Digite o ID a ser removida: ";
                std::cin >> id;

                data.remove(id);
            } break;
            case PRINT: {
                data.printBTree();
            } break;
            case EXIT: {

            } break;
            default: {
                std::cout << "Opcao invalida.\n";

                if(std::cin.fail()) {
                    cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            } break;
        }

        std::cout << "\n";
    }

    return 0;
}
