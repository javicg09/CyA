#include "../lib/language.h"

// Constructor: inicializa el lenguaje insertando las cadenas en el set.
Language::Language(const std::vector<String>& strings) {
  for (const auto& s : strings) {
    strings_.insert(s);
  }
}

// Convierte el lenguaje a string en formato {a , ab , abc}.
std::string Language::ToString() const {
  if (strings_.empty()) return "{}";

  std::string out = "{";
  bool first = true;
  for (const auto& s : strings_) {
    if (!first) out += " , ";
    out += s.ToStdString();
    first = false;
  }
  out += " }";
  return out;
}

// Operador +
Language Language::operator+(const Language& other) const {
  std::vector<String> result_strings;
  for (const String& s1 : strings_) {
    for (const String& s2 : other.strings_) {
      result_strings.push_back(s1 + s2);
    }
  }
  return Language(result_strings);
}



// Operador de salida para imprimir el lenguaje.
std::ostream& operator<<(std::ostream& os, const Language& L) {
  os << L.ToString();
  return os;
}
