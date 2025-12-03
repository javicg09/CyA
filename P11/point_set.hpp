#pragma once

#include "sub_tree.hpp"
#include "point_types.hpp"
#include <vector>
#include <iostream>

namespace EMST {
    // Definimos 'forest' como un vector de sub-árboles
    typedef std::vector<sub_tree> forest;

    class point_set : public CyA::point_vector {
    private:
        CyA::tree emst_; // El árbol resultante final
        double cost_;    // Coste total del EMST

    public:
        point_set(const CyA::point_vector &points);
        ~point_set(void);

        // Algoritmo principal
        void EMST(void);

        // Salida y visualización
        void write_tree(std::ostream &os) const;
        void write_dot(std::ostream &os) const; // Requisito opcional

        inline const CyA::tree& get_tree(void) const { return emst_; }
        inline double get_cost(void) const { return cost_; }

    private:
        // Métodos auxiliares privados para Kruskal
        void compute_arc_vector(CyA::arc_vector &av) const;
        void find_incident_subtrees(const forest& st, const CyA::arc &a, int& i, int& j) const;
        void merge_subtrees(forest& st, const CyA::arc &a, int i, int j) const;

        double euclidean_distance(const CyA::arc& a) const;
    };
}