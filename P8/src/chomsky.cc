#include "../lib/chomsky.h"
#include <algorithm>
#include <cctype>
#include <sstream>


// Convierte la gramática g a Forma Normal de Chomsky.
void Chomsky::Transformar(Gramatica& g) {
  // Copiamos el contenido de la gramática al estado interno
  producciones_ = g.Producciones();
  no_terminales_ = g.NoTerminales();
  const std::set<char>& terminales = g.Terminales();

  // Sustituir terminales por no terminales equivalentes
  std::map<char, std::string> reemplazos;
  for (std::map<std::string, std::vector<std::string>>::iterator it = producciones_.begin();
       it != producciones_.end(); ++it) {
    for (std::vector<std::string>::iterator jt = it->second.begin();
         jt != it->second.end(); ++jt) {
      if (jt->size() <= 1) continue;
      for (std::size_t i = 0; i < jt->size(); ++i) {
        char c = (*jt)[i];
        if (terminales.find(c) != terminales.end()) {
          if (reemplazos.find(c) == reemplazos.end()) {
            std::string NT(1, std::toupper(c));
            reemplazos[c] = NT;
            no_terminales_.insert(NT);
            producciones_[NT].push_back(std::string(1, c));
          }
          (*jt).replace(i, 1, reemplazos[c]);
          i += reemplazos[c].size() - 1;
        }
      }
    }
  }

  // Binarización iterativa
  bool seguir = true;
  while (seguir) {
    seguir = false;
    std::string X, Y;

    // Buscar el primer par derecho de una RHS larga
    for (std::map<std::string, std::vector<std::string>>::iterator it = producciones_.begin();
         it != producciones_.end() && !seguir; ++it) {
      for (std::vector<std::string>::iterator jt = it->second.begin();
           jt != it->second.end() && !seguir; ++jt) {
        std::vector<std::string> tok;
        std::string actual;
        for (std::size_t i = 0; i < jt->size(); ++i) {
          char c = (*jt)[i];
          actual += c;
          if (i + 1 == jt->size() ||
              (std::isupper((*jt)[i + 1]) && !(std::isupper(c) && std::isdigit((*jt)[i + 1])))) {
            tok.push_back(actual);
            actual.clear();
          }
        }
        if (tok.size() > 2) {
          X = tok[tok.size() - 2];
          Y = tok[tok.size() - 1];
          seguir = true;
        }
      }
    }

    if (!seguir) break;

    // Crear nuevo no terminal Dk -> XY
    std::ostringstream nombre;
    nombre << "D" << contador_++;
    std::string nuevo = nombre.str();
    no_terminales_.insert(nuevo);
    producciones_[nuevo].push_back(X + Y);

    // Sustituir XY por Dk en RHS largas
    for (std::map<std::string, std::vector<std::string>>::iterator it = producciones_.begin();
         it != producciones_.end(); ++it) {
      for (std::vector<std::string>::iterator jt = it->second.begin();
           jt != it->second.end(); ++jt) {
        std::vector<std::string> tok;
        std::string actual;
        for (std::size_t i = 0; i < jt->size(); ++i) {
          char c = (*jt)[i];
          actual += c;
          if (i + 1 == jt->size() ||
              (std::isupper((*jt)[i + 1]) && !(std::isupper(c) && std::isdigit((*jt)[i + 1])))) {
            tok.push_back(actual);
            actual.clear();
          }
        }
        if (tok.size() <= 2) continue;

        std::string nueva;
        for (std::size_t k = 0; k < tok.size(); ++k) {
          if (k + 1 < tok.size() && tok[k] == X && tok[k + 1] == Y) {
            nueva += nuevo;
            ++k;
          } else {
            nueva += tok[k];
          }
        }
        *jt = nueva;
      }
    }
  }

  // Actualizamos la gramática con el estado final
  const_cast<std::set<std::string>&>(g.NoTerminales()) = no_terminales_;
  const_cast<std::map<std::string, std::vector<std::string>>&>(g.Producciones()) = producciones_;
}
