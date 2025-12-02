// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmos
// Curso: 2º
// Práctica 2: Cadenas y lenguajes
// Autor: Francisco Javier Cañas González
// Correo: alu0101333021@ull.edu.es
// Fecha: 19/09/2025
// Archivo: alphabet.h
// Descripción: Definición de la clase Alphabet, que representa un alfabeto
// compuesto por símbolos.

#ifndef ALPHABET_H_
#define ALPHABET_H_

#include <iostream>
#include <set>
#include <string>

#include "symbol.h"

class Alphabet {
 public:
  // Constructores
  Alphabet() = default;
  explicit Alphabet(const std::string& alphabet);

  // Métodos
  std::string ToString() const;
  bool Contains(const Symbol& s) const;

  // Operador de salida
  friend std::ostream& operator<<(std::ostream& os, const Alphabet& alphabet);

 private:
  std::set<Symbol> symbols_;
};

#endif  // ALPHABET_H_
