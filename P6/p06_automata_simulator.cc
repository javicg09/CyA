#include <iostream>
#include <fstream>
#include <string>
#include "lib/automata.h"

void printUsage(const char* prog) {
    std::cerr << "Uso: " << prog << " input.fa input.txt\n";
    std::cerr << "Pruebe con '--help' para más información.\n";
}

void printHelp(const char* prog) {
    std::cout << prog << " — Simulador de autómatas finitos (NFA con epsilon)\n\n";
    std::cout << "Uso:  " << prog << " input.fa input.txt\n\n";
    std::cout << "Formato del fichero .fa:\n";
    std::cout << "  1: símbolos del alfabeto separados por espacios (sin '&')\n";
    std::cout << "  2: número total de estados\n";
    std::cout << "  3: estado inicial\n";
    std::cout << "  4..: por cada estado -> id acept(0/1) n_transiciones [simbolo destino]...\n\n";
    std::cout << "Nota: en el fichero de entrada, la cadena vacía se representa con '&'\n";
}

int main(int argc, char** argv) {
    if (argc == 2 && std::string(argv[1]) == "--help") {
        printHelp(argv[0]);
        return 0;
    }
    if (argc != 3) {
        printUsage(argv[0]);
        return 1;
    }

    std::ifstream faFile(argv[1]);
    if (!faFile.is_open()) {
        std::cerr << "No se pudo abrir el fichero " << argv[1] << "\n";
        return 2;
    }

    Automata A;
    try {
        A.loadFromFA(faFile);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 3;
    }

    std::ifstream inputFile(argv[2]);
    if (!inputFile.is_open()) {
        std::cerr << "No se pudo abrir el fichero " << argv[2] << "\n";
        return 4;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        bool accepted = A.accepts(line);
        std::cout << line << " --- " 
                  << (accepted ? "Accepted" : "Rejected") << "\n";
    }

    return 0;
}
