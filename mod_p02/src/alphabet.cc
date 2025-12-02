#include "../lib/alphabet.h"

// Constructor.
Alphabet::Alphabet(const std::string& alphabet) {
  for (char c : alphabet) {
    if (c != '&') {
      symbols_.insert(Symbol(c));
    }
  }
}

// Convierte el alfabeto a string con formato {a , b , c}.
std::string Alphabet::ToString() const {
  std::string out = "{";
  bool first = true;
  for (const Symbol& symbol : symbols_) {
    if (!first) out += " , ";
    out += symbol.GetValue();
    first = false;
  }
  out += "}";
  return out;
}

// Comprueba si un símbolo pertenece al alfabeto.
bool Alphabet::Contains(const Symbol& s) const {
  return symbols_.find(s) != symbols_.end();
}

// Operador de salida para imprimir el alfabeto.
std::ostream& operator<<(std::ostream& os, const Alphabet& alphabet) {
  os << alphabet.ToString();
  return os;
}
