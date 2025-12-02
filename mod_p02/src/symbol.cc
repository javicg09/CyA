#include "../lib/symbol.h"

// Constructor.
Symbol::Symbol(char c) : value_(c) {}

// Getter.
char Symbol::GetValue() const { return value_; }

// Operador < (para sets ordenados).
bool Symbol::operator<(const Symbol& other) const {
  return value_ < other.value_;
}

// Operador == (comparación por valor).
bool Symbol::operator==(const Symbol& other) const {
  return value_ == other.value_;
}

// Operador de salida.
std::ostream& operator<<(std::ostream& os, const Symbol& s) {
  os << s.value_;
  return os;
}