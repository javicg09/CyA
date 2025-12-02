// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmos
// Curso: 2º
// Práctica 2: Cadenas y lenguajes
// Autor: Francisco Javier Cañas González
// Correo: alu0101333021@ull.edu.es
// Fecha: 19/09/2025
// Archivo: language.h
// Descripción: Definición de la clase Language, que representa un conjunto de
// cadenas (String).

#ifndef LANGUAGE_H_
#define LANGUAGE_H_

#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "string.h"

class Language {
 public:
  // Comparador: ordena primero por longitud, luego lexicográficamente.
  struct ByLengthThenLex {
    bool operator()(const String& a, const String& b) const {
      if (a.Length() != b.Length()) return a.Length() < b.Length();
      return a.ToStdString() < b.ToStdString();
    }
  };

  // Constructor principal: a partir de un vector de cadenas.
  explicit Language(const std::vector<String>& strings);

  // Representación en texto.
  std::string ToString() const;

  // Sobrecargade operadores
  Language operator+(const Language& other) const;

  // Operador de salida para imprimir el lenguaje.
  friend std::ostream& operator<<(std::ostream& os, const Language& L);

 private:
  std::set<String, ByLengthThenLex> strings_;
};

#endif  // LANGUAGE_H_
