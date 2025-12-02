#include <iostream>
#include <fstream>
#include <string>
#include "lib/automata.h"

static void printUsage(const char* prog) {
    std::cerr << "Uso:\n"
              << "  " << prog << " input.fa input.txt\n"
              << "  " << prog << " input.fa --trace CADENA\n";
}

static void printHelp(const char* prog) {
    std::cout << prog << " — Simulador de autómatas finitos (NFA con epsilon)\n\n";
    std::cout << "USO:\n";
    std::cout << "  " << prog << " input.fa input.txt\n";
    std::cout << "  " << prog << " input.fa --trace CADENA\n\n";
    std::cout << "Formato del fichero .fa:\n";
    std::cout << "  1: símbolos del alfabeto separados por espacios (sin '&')\n";
    std::cout << "  2: número total de estados\n";
    std::cout << "  3: estado inicial\n";
    std::cout << "  4: por cada estado -> id acept(0/1) n_transiciones [simbolo destino]...\n\n";
    std::cout << "Notas:\n";
    std::cout << "  - '&' representa la transición vacía (epsilon).\n";
    std::cout << "  - En input.txt, la cadena vacía se representa como '&'.\n";
    std::cout << "  - Con --trace CADENA, se muestra una tabla paso a paso y se pide pulsar una letra en cada paso.\n";
}

int main(int argc, char** argv) {
    if (argc >= 2 && std::string(argv[1]) == "--help") {
        printHelp(argv[0]);
        return 0;
    }

    if (argc < 3) {
        printUsage(argv[0]);
        return 1;
    }

    // Abrir el .fa
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

    // Modifiacion: Modo traza
    if (argc == 4 && std::string(argv[2]) == "--trace") {
        std::string cadena = argv[3];
        A.trace(cadena);
        return 0;
    }

    // Modo normal: input.fa input.txt
    if (argc != 3) {
        printUsage(argv[0]);
        return 1;
    }

    std::ifstream inputFile(argv[2]);
    if (!inputFile.is_open()) {
        std::cerr << "No se pudo abrir el fichero " << argv[2] << "\n";
        return 4;
    }

    std::string line;
    int lineNum = 0;
    while (std::getline(inputFile, line)) {
        ++lineNum;
        bool accepted = A.accepts(line);
        std::cout << lineNum << " " << line << " --- "
                  << (accepted ? "Accepted" : "Rejected") << "\n";
    }

    return 0;
}

