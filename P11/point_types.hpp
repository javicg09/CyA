#pragma once

#include <iostream>
#include <set>
#include <utility>
#include <vector>

#define MAX_SZ 3
#define MAX_PREC 2

namespace CyA {
  // Definición de un punto como par de coordenadas (x, y)
  typedef std::pair<double, double> point;

  // Un arco conecta dos puntos
  typedef std::pair<point, point> arc;

  // Un arco ponderado tiene un peso (coste) y el arco en sí
  typedef std::pair<double, arc> weigthed_arc;

  // Vectores para manejar conjuntos de arcos
  typedef std::vector<weigthed_arc> arc_vector;
  typedef std::vector<arc> tree;
  typedef std::vector<point> point_vector;

  // Colección de puntos para búsquedas rápidas
  typedef std::set<point> point_collection;
}  // namespace CyA

// Sobrecarga de operadores para entrada/salida
std::ostream& operator<<(std::ostream& os, const CyA::point_vector& ps);
std::ostream& operator<<(std::ostream& os, const CyA::point& ps);
std::istream& operator>>(std::istream& is, CyA::point_vector& ps);
std::istream& operator>>(std::istream& is, CyA::point& ps);