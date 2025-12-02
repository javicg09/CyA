#ifndef AUTOMATA_HPP
#define AUTOMATA_HPP

#include <string>
#include <vector>
#include <set>
#include <map>
#include <istream>

// Clase que representa un autómata finito no determinista con transiciones epsilon ('&')
class Automata {
public:
    static const char EPSILON = '&';

    // Carga el autómata desde un fichero .fa
    void loadFromFA(std::istream& in);

    // Comprueba si la definición cargada es válida
    bool validate(std::string& error) const;

    // Devuelve true si la cadena se acepta
    bool accepts(const std::string& input) const;

    // Getters
    const std::set<char>& alphabet() const { return alphabet_; }
    std::size_t stateCount() const { return state_count_; }

private:
    std::set<char> alphabet_;                    // Alfabeto (sin '&')
    std::size_t state_count_ = 0;                // Número de estados
    int start_state_ = -1;                       // Estado inicial
    std::set<int> accept_states_;                // Conjunto de estados de aceptación

    // transitions_[q][a] = { destinos }
    std::vector<std::map<char, std::set<int>>> transitions_;

    // Funciones auxiliares del NFA
    std::set<int> epsilonClosure(const std::set<int>& states) const;
    std::set<int> move(const std::set<int>& states, char symbol) const;

    // Utilidades
    static std::string trim(const std::string& s);
    static bool isSpaceOnly(const std::string& s);
};

#endif
