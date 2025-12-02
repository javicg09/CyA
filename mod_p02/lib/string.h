// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmos
// Curso: 2º
// Práctica 2: Cadenas y lenguajes
// Autor: Francisco Javier Cañas González
// Correo: alu0101333021@ull.edu.es
// Fecha: 19/09/2025
// Archivo: string.h
// Descripción: Definición de la clase String, que representa una cadena
// compuesta por símbolos de un alfabeto.

#ifndef STRING_H_
#define STRING_H_

#include <iostream>
#include <string>
#include <vector>

#include "alphabet.h"
#include "symbol.h"

class String {
 public:
  // Constructor.
  explicit String(const std::string& w, const Alphabet& alphabet);

  // Métodos.
  size_t Length() const;
  String Reverse() const;
  std::string ToStdString() const;
  std::vector<String> Prefixes() const;
  std::vector<String> Suffixes() const;

  friend String operator+(const String& s1, const String& s2);

  // Operador de salida.
  friend std::ostream& operator<<(std::ostream& os, const String& w);

 private:
  std::vector<Symbol> symbols_;
  Alphabet alphabet_;
};

#endif  // STRING_H_
