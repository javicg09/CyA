#include <iostream>
#include <string>
#include "lib/regex.h"

// Mensaje de uso breve
void MoreInfo() {
  std::cout << "Utilización del comando: ./p04_regex <fichero_entrada> <fichero_salida>\n"
            << "Para más información, usa ./p04_regex --help\n";
}

// Ayuda detallada
void ShowHelp() {
  std::cout << "Uso: ./p04_regex <fichero_entrada> <fichero_salida>\n\n";
  std::cout << "Descripción:\n";
  std::cout << "  Analiza un fichero de código C++ utilizando expresiones regulares.\n";
  std::cout << "  Extrae información sobre variables, bucles, comentarios y la existencia de main().\n\n";
  std::cout << "Parámetros:\n";
  std::cout << "  <fichero_entrada>   Nombre del fichero de código fuente a analizar.\n";
  std::cout << "  <fichero_salida>    Nombre del fichero donde se guardará el informe generado.\n\n";
  std::cout << "Ejemplo:\n";
  std::cout << "  ./p04_regex code.cc report.txt\n";
}

int main(int argc, char* argv[]) {
  // Si no hay los dos argumentos o se pide ayuda
  if (argc == 2 && std::string(argv[1]) == "--help") {
    ShowHelp();
    return 0;
  }

  if (argc != 3) {
    MoreInfo();
    return 1;
  }

  std::string filein = argv[1];
  std::string fileout = argv[2];

  // Crear analizador y ejecutar
  CodeAnalyzer analyzer(filein, fileout);
  analyzer.Analyze();
  analyzer.WriteReport();

  std::cout << "Análisis completado. Informe guardado en " << fileout << '\n';
  return 0;
}
