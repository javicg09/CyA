// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmos
// Curso: 2º
// Práctica 2: Cadenas y lenguajes
// Autor: Francisco Javier Cañas González
// Correo: alu0101333021@ull.edu.es
// Fecha: 16/09/2025
// Archivo: cya-P02-strings.cc
// Descripción: Programa cliente. Gestiona la interfaz de línea de comandos
// y delega en las clases (Symbol, Alphabet, String, Language) para
// realizar las operaciones sobre cadenas y lenguajes.
// Historial de revisiones:
// 16/09/2025 - Creación primera versión del código (Manejador de archivos y Parseo de argumentos)
// 17/09/2025 - Segunda versión (Realizar el programa en el main)
// 19/09/2025 - Creación de clases + migración a POO
// 20/09/2025 - Versión definitiva del programa.

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../lib/alphabet.h"
#include "../lib/language.h"
#include "../lib/string.h"
#include "../lib/symbol.h"

// Funciones de ayuda.
void PrintUsage() {
  std::cerr << "Modo de empleo: ./p02_strings filein.txt fileout.txt opcode\n"
               "Pruebe './p02_strings --help' para más información.\n";
}

void PrintHelp() {
  std::cout
      << "p02_strings — Práctica 2: Cadenas y lenguajes\n\n"
      << "Uso:\n"
      << "  ./p02_strings filein.txt fileout.txt opcode\n\n"
      << "Parámetros:\n"
      << "  filein.txt   Fichero de entrada (cada línea: <cadena> <alfabeto>)\n"
      << "  fileout.txt  Fichero de salida\n"
      << "  opcode       Operación a aplicar a cada cadena:\n"
      << "               1: Mostrar alfabeto asociado\n"
      << "               2: Longitud de la cadena\n"
      << "               3: Inversa de la cadena\n"
      << "               4: Conjunto de prefijos (como lenguaje)\n"
      << "               5: Conjunto de sufijos (como lenguaje)\n\n"
      << "Notas:\n"
      << "  - La cadena vacía se representa con '&' y no puede pertenecer a "
         "ningún alfabeto.\n"
      << "  - El alfabeto se da como lista de símbolos contiguos (p. ej., 'ab' "
         "o '123').\n";
}

bool IsValidOpcode(int opcode) { return opcode >= 1 && opcode <= 6; }

bool ParseInputLine(const std::string& line, int opcode,
                    std::string& w, std::string& alphabet,
                    std::string& w2, std::string& alphabet2) {
  std::istringstream iss(line);

  if (opcode >= 1 && opcode <= 5) {
    if (!(iss >> w >> alphabet)) return false;
    std::string extra;
    if (iss >> extra) return false;  // Si hay más tokens -> error
    return true;
  }

  if (opcode == 6) {
    if (!(iss >> w >> alphabet >> w2 >> alphabet2)) return false;
    std::string extra;
    if (iss >> extra) return false;  // Sobran tokens
    return true;
  }

  return false;  // Opcode inválido
}

// --- main ---
int main(int argc, char* argv[]) {
  // Mostrar ayuda / uso.
  if (argc == 2 && std::string(argv[1]) == "--help") {
    PrintHelp();
    return 0;
  }
  if (argc != 4) {
    PrintUsage();
    return 1;
  }

  // Validación de opcode.
  int opcode = 0;
  try {
    opcode = std::stoi(argv[3]);
  } catch (...) {
    std::cerr << "Error: opcode debe ser un entero entre 1 y 5.\n";
    return 1;
  }
  if (!IsValidOpcode(opcode)) {
    std::cerr << "Error: opcode debe estar entre 1 y 5.\n";
    return 1;
  }

  // Apertura de archivos.
  const std::string input_file = argv[1];
  const std::string output_file = argv[2];

  std::ifstream fin(input_file);
  if (!fin) {
    std::cerr << "Error: no se pudo abrir el fichero de entrada: " << input_file
              << "\n";
    return 1;
  }

  std::ofstream fout(output_file);
  if (!fout) {
    std::cerr << "Error: no se pudo abrir ni crear el archivo de salida: "
              << output_file << "\n";
    return 1;
  }

  // Procesamiento línea a línea.
  std::string line, w_text, alphabet_text;
  int line_number = 0;

  while (std::getline(fin, line)) {
    ++line_number;

    std::string w_text, alphabet_text, w2_text, alphabet2_text;

    if (!ParseInputLine(line, opcode, w_text, alphabet_text, w2_text, alphabet2_text)) {
      std::cerr << "Error en línea " << line_number << ": formato inválido\n";
      continue;
    }

    // Construcción de objetos.
    Alphabet A(alphabet_text);
    String W(w_text, A);

    switch (opcode) {
      case 1: {  // Alfabeto
        fout << A << "\n";
        break;
      }
      case 2: {  // Longitud
        fout << W.Length() << "\n";
        break;
      }
      case 3: {  // Inversa
        fout << W.Reverse() << "\n";
        break;
      }
      case 4: {  // Prefijos como lenguaje
        Language L(W.Prefixes());
        fout << L << "\n";
        break;
      }
      case 5: {  // Sufijos como lenguaje
        Language L(W.Suffixes());
        fout << L << "\n";
        break;
      }
      case 6: {  // Concatenación de lenguajes
        Alphabet A1(alphabet_text);
        Alphabet A2(alphabet2_text);
        String W1(w_text, A1);
        String W2(w2_text, A2);

        Language L1(W1.Prefixes());
        Language L2(W2.Suffixes());


        // Concatenamos y mostramos
        Language result = L1 + L2;
        fout << result << "\n";
        break;
      }

      default:
        std::cerr << "Error interno: opcode inválido.\n";
        return 1;
    }
  }

  return 0;
}