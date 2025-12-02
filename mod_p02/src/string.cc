#include "../lib/string.h"

#include <algorithm>  // Para std::reverse

// Constructor: si w == "&", representamos cadena vacía con symbols_ vacío.
String::String(const std::string& w, const Alphabet& alphabet)
    : alphabet_(alphabet) {
  if (w == "&") return;  // Cadena vacía -> no añadimos símbolos.
  symbols_.reserve(w.size());

  for (char c : w) {
    Symbol s(c);

    // Validación.
    if (!alphabet_.Contains(s)) {
      // Lanza excepción si el símbolo es inválido.
      throw std::invalid_argument("Símbolo '" + std::string(1, c) +
                                  "' no pertenece al alfabeto");
    }

    symbols_.emplace_back(c);
  }
}

size_t String::Length() const { return symbols_.size(); }

String String::Reverse() const {
  std::string s = ToStdString();
  // s será "&" si está vacía; en ese caso, devuelve también vacía.
  if (s == "&") return String("&", alphabet_);
  std::reverse(s.begin(), s.end());
  return String(s, alphabet_);
}

std::string String::ToStdString() const {
  if (symbols_.empty()) {
    return "&";  // Si no hay símbolos, cadena vacía "&".
  }
  std::string result;
  result.reserve(symbols_.size());
  for (const Symbol& sym : symbols_) {
    result += sym.GetValue();
  }
  return result;  // Devuelve el vector de símbolos como una cadena.
}

std::vector<String> String::Prefixes() const {
  std::vector<String> res;
  const std::string s = ToStdString();
  if (s == "&") {  // La cadena vacía solo tiene {&}.
    res.emplace_back("&", alphabet_);
    return res;
  }
  res.emplace_back("&", alphabet_);  // Primero la vacía.
  for (size_t i = 1; i <= s.size(); ++i) {
    res.emplace_back(s.substr(0, i), alphabet_);  // Prefijos por longitud.
  }
  return res;
}

std::vector<String> String::Suffixes() const {
  std::vector<String> res;
  const std::string s = ToStdString();
  if (s == "&") {
    res.emplace_back("&", alphabet_);
    return res;
  }
  res.emplace_back("&", alphabet_);  // Primero la vacía.
  for (size_t i = 1; i <= s.size(); ++i) {
    res.emplace_back(s.substr(s.size() - i, i), alphabet_);  // Sufijos por longitud.
  }
  return res;
}

String operator+(const String& s1, const String& s2) {
  // Si alguno es cadena vacía (&), devolvemos el otro.
  if (s1.ToStdString() == "&") return s2;
  if (s2.ToStdString() == "&") return s1;

  // Concatenamos usando std::string
  std::string concatenated = s1.ToStdString() + s2.ToStdString();

  // Creamos un nuevo String usando el mismo alfabeto que s1
  return String(concatenated, s1.alphabet_);
}


std::ostream& operator<<(std::ostream& os, const String& s) {
  os << s.ToStdString();
  return os;
}
