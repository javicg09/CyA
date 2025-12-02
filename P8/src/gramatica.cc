#include "../lib/gramatica.h"
#include <fstream>
#include <sstream>
#include <queue>

bool Gramatica::LeerFichero(const std::string& nombre_fichero) {
  std::ifstream fichero(nombre_fichero);
  if (!fichero.is_open()) {
    return false;
  }

  // Lectura de símbolos terminales
  int n_terminales = 0;
  fichero >> n_terminales;
  for (int i = 0; i < n_terminales; ++i) {
    char simbolo;
    fichero >> simbolo;
    terminales_.insert(simbolo);
  }

  // Lectura de símbolos no terminales
  int n_no_terminales = 0;
  fichero >> n_no_terminales;
  for (int i = 0; i < n_no_terminales; ++i) {
    std::string simbolo;
    fichero >> simbolo;
    noTerminales_.insert(simbolo);
    if (i == 0) {
      simboloInicial_ = simbolo;  // El primero es el símbolo inicial
    }
  }

  // Lectura de producciones
  int n_producciones = 0;
  fichero >> n_producciones;

  for (int i = 0; i < n_producciones; ++i) {
    std::string izquierda;
    std::string derecha;
    fichero >> izquierda >> derecha;
    producciones_[izquierda].push_back(derecha);
  }

  fichero.close();
  return true;
}

bool Gramatica::EscribirFichero(const std::string& nombre_fichero){
  std::ofstream fichero(nombre_fichero);
  if (!fichero.is_open()) {
    return false;
  }

  // Escritura de símbolos terminales
  fichero << terminales_.size() << "\n";
  for (std::set<char>::const_iterator it = terminales_.begin(); it != terminales_.end(); ++it) {
    fichero << *it << "\n";
  }

  // Escritura de símbolos no terminales
  fichero << noTerminales_.size() << "\n";
  for (std::set<std::string>::const_iterator it = noTerminales_.begin(); it != noTerminales_.end(); ++it) {
    fichero << *it << "\n";
  }

  // Conteo total de producciones
  int total_producciones = 0;
  for (std::map<std::string, std::vector<std::string>>::const_iterator it = producciones_.begin();
    it != producciones_.end(); ++it) {
    total_producciones += static_cast<int>(it->second.size());
  }

  fichero << total_producciones << "\n";

  // Escritura de las producciones
  for (std::map<std::string, std::vector<std::string>>::const_iterator it = producciones_.begin();
    it != producciones_.end(); ++it) {
    const std::string& izquierda = it->first;
    const std::vector<std::string>& derechos = it->second;

    for (std::vector<std::string>::const_iterator jt = derechos.begin(); jt != derechos.end(); ++jt) {
      fichero << izquierda << " " << *jt << "\n";
    }
  }

  fichero.close();
  return true;
}

std::set<std::string> Gramatica::SimbolosAlcanzables() const {
  std::set<std::string> alcanzables;
  std::queue<std::string> cola;

  cola.push(simboloInicial_);
  alcanzables.insert(simboloInicial_);

  while (!cola.empty()) {
    std::string actual = cola.front();
    cola.pop();

    std::map<std::string, std::vector<std::string> >::const_iterator it = producciones_.find(actual);
    if (it != producciones_.end()) {
      const std::vector<std::string>& derechos = it->second;
      for (std::vector<std::string>::const_iterator jt = derechos.begin(); jt != derechos.end(); ++jt) {
        const std::string& prod = *jt;
        // recorremos símbolos de la producción
        for (std::size_t i = 0; i < prod.size(); ++i) {
          char c = prod[i];
          if (isupper(c)) {  // símbolo no terminal
            std::string s(1, c);
            if (alcanzables.find(s) == alcanzables.end()) {
              alcanzables.insert(s);
              cola.push(s);
            }
          }
        }
      }
    }
  }
  return alcanzables;
}

void Gramatica::AñadirProduccion(const std::string& izquierda, const std::string& derecha) {
  producciones_[izquierda].push_back(derecha);
}


const std::set<char>& Gramatica::Terminales() const {
  return terminales_;
}

const std::set<std::string>& Gramatica::NoTerminales() const {
  return noTerminales_;
}

const std::string& Gramatica::SimboloInicial() const {
  return simboloInicial_;
}

const std::map<std::string, std::vector<std::string>>& Gramatica::Producciones() const {
  return producciones_;
}
