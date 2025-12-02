#include "../lib/automata.h"

#include <sstream>
#include <stdexcept>
#include <stack>
#include <cctype>
#include <iostream>

void Automata::loadFromFA(std::istream& in) {
    alphabet_.clear();
    state_count_ = 0;
    start_state_ = -1;
    accept_states_.clear();
    transitions_.clear();

    std::string line;

    // Línea 1: alfabeto
    if (!std::getline(in, line)) throw std::runtime_error("Fichero .fa incompleto (alfabeto).");
    {
        std::istringstream ls(line);
        std::string tok;
        while (ls >> tok) {
            if (tok.size() != 1)
                throw std::runtime_error("Símbolo inválido en el alfabeto.");
            char c = tok[0];
            if (c == EPSILON)
                throw std::runtime_error("'&' está reservado para epsilon.");
            alphabet_.insert(c);
        }
        if (alphabet_.empty())
            throw std::runtime_error("Alfabeto vacío.");
    }

    // Línea 2: número de estados
    if (!std::getline(in, line)) throw std::runtime_error("Fichero .fa incompleto (número de estados).");
    {
        line = trim(line);
        std::istringstream ls(line);
        if (!(ls >> state_count_) || state_count_ == 0)
            throw std::runtime_error("Número de estados inválido.");
    }

    // Línea 3: estado inicial
    if (!std::getline(in, line)) throw std::runtime_error("Fichero .fa incompleto (estado inicial).");
    {
        line = trim(line);
        std::istringstream ls(line);
        if (!(ls >> start_state_))
            throw std::runtime_error("Estado inicial inválido.");
    }

    // Inicializamos estructura de transiciones
    transitions_.clear();
    transitions_.resize(state_count_);

    // Líneas siguientes: definición de estados
    for (std::size_t i = 0; i < state_count_; ++i) {
        if (!std::getline(in, line))
            throw std::runtime_error("Faltan líneas de definición de estados.");
        if (isSpaceOnly(line)) { --i; continue; }

        std::istringstream ls(line);
        int id = -1;
        int is_accept = 0;
        int num_trans = 0;

        if (!(ls >> id >> is_accept >> num_trans))
            throw std::runtime_error("Línea de estado mal formada.");

        if (id < 0 || static_cast<std::size_t>(id) >= state_count_)
            throw std::runtime_error("Identificador de estado fuera de rango.");

        if (is_accept == 1)
            accept_states_.insert(id);

        for (int t = 0; t < num_trans; ++t) {
            std::string sym;
            int dst = -1;
            if (!(ls >> sym >> dst))
                throw std::runtime_error("Transición mal formada.");

            if (sym.size() != 1)
                throw std::runtime_error("Símbolo de transición inválido.");

            char a = sym[0];
            if (a != EPSILON && alphabet_.count(a) == 0)
                throw std::runtime_error("Símbolo fuera del alfabeto.");

            if (dst < 0 || static_cast<std::size_t>(dst) >= state_count_)
                throw std::runtime_error("Estado destino fuera de rango.");

            transitions_[id][a].insert(dst);
        }
    }

    std::string err;
    if (!validate(err))
        throw std::runtime_error("Definición inválida: " + err);
}

bool Automata::validate(std::string& error) const {
    if (start_state_ < 0 || static_cast<std::size_t>(start_state_) >= state_count_) {
        error = "El estado inicial no pertenece al autómata.";
        return false;
    }
    return true;
}

std::set<int> Automata::epsilonClosure(const std::set<int>& states) const {
    std::set<int> closure = states;
    std::stack<int> st;
    for (std::set<int>::const_iterator it = states.begin(); it != states.end(); ++it)
        st.push(*it);

    while (!st.empty()) {
        int q = st.top(); st.pop();
        std::map<char, std::set<int> >::const_iterator it_eps = transitions_[q].find(EPSILON);
        if (it_eps == transitions_[q].end()) continue;
        for (std::set<int>::const_iterator jt = it_eps->second.begin(); jt != it_eps->second.end(); ++jt) {
            if (closure.insert(*jt).second)
                st.push(*jt);
        }
    }
    return closure;
}

std::set<int> Automata::move(const std::set<int>& states, char symbol) const {
    std::set<int> result;
    for (std::set<int>::const_iterator it = states.begin(); it != states.end(); ++it) {
        std::map<char, std::set<int> >::const_iterator it_a = transitions_[*it].find(symbol);
        if (it_a == transitions_[*it].end()) continue;
        result.insert(it_a->second.begin(), it_a->second.end());
    }
    return result;
}

bool Automata::accepts(const std::string& input) const {
    std::set<int> current;
    current.insert(start_state_);
    current = epsilonClosure(current);

    if (input.size() == 1 && input[0] == EPSILON) {
        for (std::set<int>::const_iterator it = current.begin(); it != current.end(); ++it)
            if (accept_states_.count(*it)) return true;
        return false;
    }

    for (std::size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        if (alphabet_.count(c) == 0)
            return false;

        std::set<int> afterMove = move(current, c);
        if (afterMove.empty()) return false;
        current = epsilonClosure(afterMove);
    }

    for (std::set<int>::const_iterator it = current.begin(); it != current.end(); ++it)
        if (accept_states_.count(*it)) return true;
    return false;
}

std::string Automata::trim(const std::string& s) {
    std::size_t i = 0, j = s.size();
    while (i < j && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    while (j > i && std::isspace(static_cast<unsigned char>(s[j - 1]))) --j;
    return s.substr(i, j - i);
}

bool Automata::isSpaceOnly(const std::string& s) {
    for (std::size_t i = 0; i < s.size(); ++i)
        if (!std::isspace(static_cast<unsigned char>(s[i]))) return false;
    return true;
}

std::string Automata::setToString(const std::set<int>& s) {
  std::ostringstream os;
  os << "{";
    for (std::set<int>::const_iterator it = s.begin(); it != s.end(); ++it) {
      if (it != s.begin()) os << ",";
      os << *it;
    }
      os << "";
      os << "}";
      return os.str();
}

std::string Automata::rulesToString(const std::vector<std::string>& rules) {
    std::ostringstream os;
    for (std::size_t i = 0; i < rules.size(); ++i) {
        if (i > 0) os << "; ";
        os << rules[i];
    }
    return os.str();
}

void Automata::trace(const std::string& input) const {
    // Preparar conjunto inicial con epsilon-closure
    std::set<int> current;
    current.insert(start_state_);
    current = epsilonClosure(current);

    std::cout << "TRACE — simulacion paso a paso" << std::endl;
    std::cout << "Estado(s) inicial(es) tras epsilon-closure: " << setToString(current) << std::endl;
    std::cout << "\nTABLA:" << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << "Estados actuales\tEntrada\tReglas aplicadas\t\tSiguientes (tras eps)" << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;

    // Caso cadena vacía representada por '&'
    if (input.size() == 1 && input[0] == EPSILON) {
        std::vector<std::string> rules;
        std::cout << setToString(current) << "\t\t" << EPSILON << "\t" 
                  << "-" << "\t\t\t" << setToString(current) << std::endl;

        std::cout << "\nResultado: " 
                  << (accepts(input) ? "Accepted" : "Rejected") 
                  << std::endl;
        return;
    }

    for (std::size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        // Validación de símbolo
        if (alphabet_.count(c) == 0) {
            std::cout << setToString(current) << "\t\t" << c 
                      << "\t" << "[simbolo fuera de alfabeto]" 
                      << "\t\t" << "{}" << std::endl;
            std::cout << "\nResultado: Rejected" << std::endl;
            return;
        }

        // Construir lista de reglas aplicadas (q, c) = r
        std::vector<std::string> rules;
        for (std::set<int>::const_iterator it = current.begin(); it != current.end(); ++it) {
            int q = *it;
            std::map<char, std::set<int> >::const_iterator it_a = transitions_[q].find(c);
            if (it_a == transitions_[q].end()) continue;
            for (std::set<int>::const_iterator jt = it_a->second.begin(); jt != it_a->second.end(); ++jt) {
                std::ostringstream osr;
                osr << "(q" << q << ", " << c << ") = q" << *jt;
                rules.push_back(osr.str());
            }
        }

        // Calcular move y luego epsilon-closure
        std::set<int> afterMove = move(current, c);
        std::set<int> afterClosure = epsilonClosure(afterMove);

        // Imprimir fila
        std::cout << setToString(current) << "\t\t" << c << "\t" 
                  << (rules.empty() ? "-" : rulesToString(rules)) << "\t\t" 
                  << setToString(afterClosure) << std::endl;

        // Pausa: pide pulsar una letra para continuar
        std::cout << "Pulsa una letra para continuar..." << std::endl;
        char cont;
        std::cin >> cont;

        // Avanzar
        current = afterClosure;
    }

    // Fin: comprobar aceptación
    bool ok = false;
    for (std::set<int>::const_iterator it = current.begin(); it != current.end(); ++it) {
        if (accept_states_.count(*it)) { ok = true; break; }
    }

    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << "Resultado: " << (ok ? "Accepted" : "Rejected") << std::endl;
}