// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmos
// Curso: 2º
// Práctica 2: Cadenas y lenguajes
// Autor: Francisco Javier Cañas González
// Correo: alu0101333021@ull.edu.es
// Fecha: 19/09/2025
// Archivo: symbol.h
// Descripción: Definición de la clase Symbol, que representa un símbolo
// de un alfabeto.

#ifndef SYMBOL_H_
#define SYMBOL_H_

#include <iostream>

class Symbol {
 public:
  // Constructor.
  Symbol(char c = '&');  // Por defecto, '&' (aunque luego no se permitirá en alfabetos).

  // Getter.
  char GetValue() const;

  // Sobrecarga de operadores.
  bool operator<(const Symbol& other) const;   // Para sets ordenados.
  bool operator==(const Symbol& other) const;  // Comparación por valor.

  // Operador de salida.
  friend std::ostream& operator<<(std::ostream& os, const Symbol& s);

 private:
  char value_;
};

#endif  // SYMBOL_H_
