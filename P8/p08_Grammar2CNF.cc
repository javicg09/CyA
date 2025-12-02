#include "lib/chomsky.h"
#include <iostream>

// --- Funciones ---
void ShowMore(){
  std::cout << "$ ./Grammar2CNF\n"
            << "Modo de empleo: ./Grammar2CNF input.gra output.gra\n"
            << "Pruebe ‘Grammar2CNF --help’ para más información.\n";
}

void PrintHelp(){
  std::cout << "Grammar2CNF - Conversión de gramáticas a Forma Normal de Chomsky\n\n"
            << "Modo de empleo:\n"
            << "  ./Grammar2CNF input.gra output.gra\n\n"
            << "Descripción:\n"
            << "  Lee una gramática independiente del contexto desde el fichero 'input.gra'\n"
            << "  y genera un fichero 'output.gra' con una gramática equivalente escrita\n"
            << "  en Forma Normal de Chomsky (CNF).\n\n"
            << "Opciones:\n"
            << "  --help\tMuestra esta ayuda y termina.\n\n"
            << "Formato del fichero .gra:\n"
            << "  1. Número de símbolos terminales seguido de cada símbolo en una línea.\n"
            << "  2. Número de símbolos no terminales seguido de cada uno en una línea.\n"
            << "     El primero es el símbolo inicial.\n"
            << "  3. Número de producciones seguido de cada producción en formato:\n"
            << "       A α\n"
            << "     donde A es un no terminal y α una secuencia de símbolos del alfabeto o no terminales.\n"
            << "     La cadena vacía se representa con '&'.\n"
            << std::endl;
}

int main (int argc, const char* argv[]){
  // Gestor de entrada por consola
  if(argc == 2 && std::string(argv[1]) == "--help") {
    PrintHelp();
    return 0;
  }
  if(argc != 3){
    ShowMore();
    return 0;
  }

  // Convertir a CNF
  // Creamos la gramatica a partir del fichero
  Gramatica g;
  if (!g.LeerFichero(argv[1])) {
    std::cerr << "Error: no se pudo leer el fichero " << argv[1] << "\n";
    return 1;
  }

  // Modificacion: Símbolos alcanzables
  std::set<std::string> alcanzables = g.SimbolosAlcanzables();
  std::cout << "Símbolos alcanzables desde S:\n";
  for (std::set<std::string>::iterator it = alcanzables.begin(); it != alcanzables.end(); ++it) {
    std::cout << " - " << *it << "\n";
  }

  Chomsky conversor;
  conversor.Transformar(g);

  if (!g.EscribirFichero(argv[2])) {
    std::cerr << "Error: no se pudo escribir el fichero " << argv[2] << "\n";
    return 1;
  }
  
  std::cout << "Conversión completada. Gramática CNF escrita en " << argv[2] << "\n";
  return 0;
}