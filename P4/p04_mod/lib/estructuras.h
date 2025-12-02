#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <string>

// Estructura de variable
struct VariableInfo {
  std::string type;
  std::string name;
  int line;
  bool initialized;
  std::string init_value;
};

// Estructura de bucle
struct LoopInfo {
  std::string type;
  int line;
};

// Estructura de comentario
struct CommentInfo {
  std::string type;
  std::string content;
  int start_line;
  int end_line;
};

// Modificación
struct CondicionalInfo {
  int line;
  std::string condicion;
  bool has_else;
};

#endif
