#ifndef GRAMATICA_H
#define GRAMATICA_H

#include <string>
#include <iostream>
#include <set>
#include <vector>
#include <map>

class Gramatica {
  public:
    // Manejadores de archivo
    bool LeerFichero(const std::string& fichero_entrada);
    bool EscribirFichero(const std::string& fichero_salida);

    // Getters
    const std::set<char>& Terminales() const;
    const std::set<std::string>& NoTerminales() const;
    const std::string& SimboloInicial() const;
    const std::map<std::string, std::vector<std::string>>& Producciones() const;

    // Añadir producción a la gramatica
    void AñadirProduccion(const std::string& izquierda, const std::string& derecha);

    // Modificacion: Simbolos alcanzables
    std::set<std::string> SimbolosAlcanzables() const;

  private:
    // Atributos de la gramática
    std::set<char> terminales_;
    std::set<std::string> noTerminales_;
    std::string simboloInicial_;
    std::map<std::string, std::vector<std::string>> producciones_;
};

#endif //GRAMATICA_H