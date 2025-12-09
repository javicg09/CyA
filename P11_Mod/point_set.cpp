#include "point_set.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>

namespace EMST {

  point_set::point_set(const CyA::point_vector& points) : CyA::point_vector(points), cost_(0) {}

  point_set::~point_set(void) {}

  // Lógica principal de Kruskal
  void point_set::EMST(void) {
    CyA::arc_vector av;
    // 1. Calcular todos los posibles arcos y sus costes, y ordenarlos
    compute_arc_vector(av);

    forest st;

    // 2. Inicializar el bosque: cada punto es un árbol independiente
    for (const CyA::point& p : *this) {
      sub_tree s;
      s.add_point(p);
      st.push_back(s);
    }

    // 3. Iterar sobre los arcos ordenados (Voraz)
    for (const CyA::weigthed_arc& a : av) {
      int i, j;
      // Buscar a qué sub-árboles pertenecen los puntos del arco
      find_incident_subtrees(st, a.second, i, j);

      // Si pertenecen a árboles distintos, fusionarlos (evita ciclos)
      if (i != j) {
        merge_subtrees(st, a.second, i, j);
      }
    }

    // Al final, el bosque debería tener un solo componente (el MST)
    if (!st.empty()) {
      emst_ = st[0].get_arcs();
      cost_ = st[0].get_cost();
    }
  }

  // Lógica principal de Kruskal con aristas del fichero
  void point_set::EMST_from_file(const std::string& filename) {
    CyA::arc_vector av;
    // 1. Leer aristas desde el fichero y calcular sus costes, ordenarlas
    read_arc_vector_from_file(filename, av);

    forest st;

    // 2. Inicializar el bosque: cada punto es un árbol independiente
    for (const CyA::point& p : *this) {
      sub_tree s;
      s.add_point(p);
      st.push_back(s);
    }

    // 3. Iterar sobre los arcos ordenados (Voraz)
    for (const CyA::weigthed_arc& a : av) {
      int i, j;
      // Buscar a qué sub-árboles pertenecen los puntos del arco
      find_incident_subtrees(st, a.second, i, j);

      // Si pertenecen a árboles distintos, fusionarlos (evita ciclos)
      if (i != j) {
        merge_subtrees(st, a.second, i, j);
      }
    }

    // Al final, el bosque debería tener un solo componente (el MST)
    if (!st.empty()) {
      emst_ = st[0].get_arcs();
      cost_ = st[0].get_cost();
    }
  }

  void point_set::compute_arc_vector(CyA::arc_vector& av) const {
    av.clear();
    const int n = size();

    for (int i = 0; i < n - 1; ++i) {
      const CyA::point& p_i = (*this)[i];
      for (int j = i + 1; j < n; ++j) {
        const CyA::point& p_j = (*this)[j];
        const double dist = euclidean_distance(std::make_pair(p_i, p_j));

        // Guardamos: (coste, arco)
        av.push_back(std::make_pair(dist, std::make_pair(p_i, p_j)));
      }
    }
    // Ordenamos por peso (requisito Greedy)
    std::sort(av.begin(), av.end());
  }

  // Modificacion
  void point_set::read_arc_vector_from_file(const std::string& filename, CyA::arc_vector& av) const {
    av.clear();
    std::ifstream file(filename);
    
    if (!file.is_open()) {
      std::cerr << "Error: No se pudo abrir el fichero " << filename << std::endl;
      return;
    }

    double x1, y1, x2, y2;
    while (file >> x1 >> y1 >> x2 >> y2) {
      CyA::point p1 = std::make_pair(x1, y1);
      CyA::point p2 = std::make_pair(x2, y2);
      CyA::arc arc = std::make_pair(p1, p2);
      double dist = euclidean_distance(arc);
      
      // Guardamos: (coste, arco)
      av.push_back(std::make_pair(dist, arc));
    }

    file.close();
    
    // Ordenamos por peso (requisito Greedy)
    std::sort(av.begin(), av.end());
  }

  void point_set::find_incident_subtrees(const forest& st, const CyA::arc& a, int& i,
                                         int& j) const {
    i = -1;
    j = -1;

    // Buscamos linealmente en el bosque qué árbol contiene los puntos
    for (size_t k = 0; k < st.size(); ++k) {
      if (st[k].contains(a.first)) {
        i = k;
      }
      if (st[k].contains(a.second)) {
        j = k;
      }
      // Si ya encontramos ambos, podemos salir (optimización leve)
      if (i != -1 && j != -1) break;
    }
  }

  void point_set::merge_subtrees(forest& st, const CyA::arc& a, int i, int j) const {
    // Crear un arco ponderado temporal para pasar al merge (el peso ya no importa para la
    // estructura, solo para el coste total) Recalculamos el peso solo para pasarlo correctamente al
    // merge
    double w = euclidean_distance(a);
    CyA::weigthed_arc wa = std::make_pair(w, a);

    // Fusionamos el árbol j dentro del i
    st[i].merge(st[j], wa);

    // Eliminamos el árbol j del bosque ya que ha sido absorbido
    st.erase(st.begin() + j);
  }

  double point_set::euclidean_distance(const CyA::arc& a) const {
    double dx = a.first.first - a.second.first;
    double dy = a.first.second - a.second.second;
    return std::sqrt(dx * dx + dy * dy);
  }

  void point_set::write_tree(std::ostream& os) const {
    // os << cost_ << std::endl; // Opcional: imprimir coste primero
    for (const auto& arc : emst_) {
      os << "(" << arc.first.first << ", " << arc.first.second << ") -> "
         << "(" << arc.second.first << ", " << arc.second.second << ")" << std::endl;
    }
    os << std::endl << cost_ << std::endl;
  }

  // Requisito Opcional: Generar archivo DOT
  void point_set::write_dot(std::ostream& os) const {
    os << "graph EMST {" << std::endl;
    os << "  node [shape=point];" << std::endl;  // Nodos como puntos

    // Definir nodos con posiciones para neato
    int id = 0;
    for (const CyA::point& p : *this) {
      // Usamos las coordenadas para el atributo pos
      os << "  n" << id++ << " [pos=\"" << p.first << "," << p.second << "!\"];" << std::endl;
    }

    // Dibujar aristas
    // Estrategia robusta: Recorrer el árbol y escribir los arcos
    for (const auto& arc : emst_) {
      os << "  \"" << arc.first.first << "," << arc.first.second << "\" -- \"" << arc.second.first
         << "," << arc.second.second << "\";" << std::endl;
    }

    os << "}" << std::endl;
  }
}  // namespace EMST