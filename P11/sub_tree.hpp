#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include "point_types.hpp"

namespace EMST {
    class sub_tree {
    private:
        CyA::tree arcs_;              // Aristas que forman este sub-árbol
        CyA::point_collection points_; // Puntos contenidos en este sub-árbol
        double cost_;                 // Coste total acumulado

    public:
        sub_tree(void);
        ~sub_tree(void);

        void add_arc(const CyA::arc &a);
        void add_point(const CyA::point &p);
        bool contains(const CyA::point &p) const;
        
        // Fusiona otro sub-árbol dentro de este
        void merge(const sub_tree &st, const CyA::weigthed_arc &a);

        inline const CyA::tree& get_arcs(void) const { return arcs_; }
        inline double get_cost(void) const { return cost_; }
    };

    typedef std::vector<sub_tree> sub_tree_vector;
}