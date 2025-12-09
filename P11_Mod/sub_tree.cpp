#include "sub_tree.hpp"

#include <algorithm>

namespace EMST {

  sub_tree::sub_tree(void) : arcs_(), points_(), cost_(0) {}

  sub_tree::~sub_tree(void) {}

  void sub_tree::add_arc(const CyA::arc& a) {
    arcs_.push_back(a);
    points_.insert(a.first);
    points_.insert(a.second);
  }

  void sub_tree::add_point(const CyA::point& p) { points_.insert(p); }

  bool sub_tree::contains(const CyA::point& p) const {
    // Buscamos si el punto existe en la colección (set)
    return points_.find(p) != points_.end();
  }

  void sub_tree::merge(const sub_tree& st, const CyA::weigthed_arc& a) {
    // 1. Añadimos los arcos del otro sub-árbol
    arcs_.insert(arcs_.end(), st.arcs_.begin(), st.arcs_.end());

    // 2. Añadimos el arco que une ambos árboles
    arcs_.push_back(a.second);

    // 3. Añadimos los puntos del otro sub-árbol
    points_.insert(st.points_.begin(), st.points_.end());

    // 4. Sumamos el coste del otro árbol + el coste del arco de unión
    cost_ += st.get_cost() + a.first;
  }
}  // namespace EMST