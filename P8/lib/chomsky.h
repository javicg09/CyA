#ifndef CHOMSKY_H_
#define CHOMSKY_H_

#include "gramatica.h"
#include <map>
#include <set>
#include <string>
#include <vector>

class Chomsky {
 public:
  
  Chomsky() : contador_(1) {}

  void Transformar(Gramatica& g);

 private:
  // Atributos internos usados durante la transformación
  int contador_;
  std::set<std::string> no_terminales_;  ///< Conjunto de no terminales.
  std::map<std::string, std::vector<std::string>> producciones_;  ///< Producciones temporales.
};

#endif  // CHOMSKY_H_
