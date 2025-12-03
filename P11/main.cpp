#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "point_set.hpp"
#include "point_types.hpp"

// Implementación de los operadores globales definidos en point_types.hpp
// (Se ponen aquí o en un cpp separado, por simplicidad los pongo aquí para cerrar el ciclo de
// dependencias)

std::ostream& operator<<(std::ostream& os, const CyA::point_vector& ps) {
  os << ps.size() << std::endl;
  for (const CyA::point& p : ps) {
    os << p << std::endl;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const CyA::point& p) {
  os << std::fixed << std::setprecision(MAX_PREC) << p.first << " " << p.second;
  return os;
}

std::istream& operator>>(std::istream& is, CyA::point_vector& ps) {
  int n;
  is >> n;
  ps.clear();
  for (int i = 0; i < n; ++i) {
    CyA::point p;
    is >> p;
    ps.push_back(p);
  }
  return is;
}

std::istream& operator>>(std::istream& is, CyA::point& p) {
  is >> p.first >> p.second;
  return is;
}

int main(int argc, char* argv[]) {
  CyA::point_vector points;
  bool output_dot = false;

  // Procesar argumentos
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-d") == 0) {
      output_dot = true;
    }
  }

  // Leer entrada estándar
  std::cin >> points;

  // Crear el conjunto y ejecutar EMST
  EMST::point_set ps(points);
  ps.EMST();

  // Salida estándar
  ps.write_tree(std::cout);

  // Salida opcional DOT
  if (output_dot) {
    std::ofstream dot_file("emst.dot");
    if (dot_file.is_open()) {
      ps.write_dot(dot_file);
      dot_file.close();
      // std::cerr << "Archivo emst.dot generado." << std::endl;
    }
  }

  return 0;
}