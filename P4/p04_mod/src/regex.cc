#include <fstream>
#include <iostream>
#include <string>
#include <regex>

#include "../lib/regex.h"

// Constructor
CodeAnalyzer::CodeAnalyzer(const std::string& filein, const std::string& fileout)
    : input_file_(filein), output_file_(fileout) {}

// Analizar archivo entrada
void CodeAnalyzer::Analyze() {
  // Manejador de archivo de entrada
  std::ifstream fin(input_file_);
  if (!fin.is_open()) {
    std::cerr << "Error: no se pudo abrir el archivo de entrada: " << input_file_ << '\n';
    return;
  }

  std::string line;
  int line_number = 1;
  
  // Analizamos linea a linea
  while (std::getline(fin, line)) {
    DetectVariables(line, line_number);
    DetectLoops(line, line_number);
    DetectComments(line, line_number);
    DetectMain(line);
    // MOdificacion
    DetectCondicional(line, line_number);
    ++line_number;
  }
  fin.close();
}

// Escribir informe en el archivo de salida
void CodeAnalyzer::WriteReport() const {
  // Manejador de archivo de salida
  std::ofstream fout(output_file_);
  if (!fout.is_open()) {
    std::cerr << "Error: no se pudo abrir el fichero de salida: " << output_file_ << '\n';
    return;
  }

  // PROGRAM
  fout << "PROGRAM : " << input_file_ << "\n\n";

  // DESCRIPTION
  fout << "DESCRIPTION :\n";
  if (!description_.empty()) {
    fout << description_ << "\n";
  }
  fout << '\n';

  // VARIABLES
  fout << "VARIABLES :\n";
  for (size_t i = 0; i < variables_.size(); ++i) {
    const VariableInfo& var = variables_[i];

    fout << "[ Line " << var.line << "] ";
    if (var.type == "int") {
      fout << "INT : ";
    } if (var.type == "double") {
      fout << "DOUBLE : ";
    }

    fout << var.name;
    if (var.initialized) {
      // Capturamos el valor de la variable, para evitar descuidos
      if (!var.init_value.empty()) {
        fout << " = " << var.init_value;
      }
    }
    fout << '\n';
  }
  fout << '\n';

  // STATEMENTS
  fout << "STATEMENTS :\n";
  for (size_t i = 0; i < loops_.size(); ++i) {
    const LoopInfo& loop = loops_[i];
    fout << "[ Line " << loop.line << "] LOOP : " << loop.type << '\n';
  }
  fout << '\n';

  // MAIN
  fout << "MAIN :\n";
  if (has_main_) {
    fout << "True\n\n";
  } else {
    fout << "False\n\n";
  }

  // COMMENTS
  fout << "COMMENTS :\n";
  for (size_t i = 0; i < comments_.size(); ++i) {
    const CommentInfo& cmt = comments_[i];

    if (cmt.start_line == cmt.end_line) {
      // Comentario de una línea
      fout << "[ Line " << cmt.start_line << "] " << cmt.content << '\n';
    } else {
      // Comentario multilínea: etiqueta como DESCRIPTION si coincide con la descripción guardada
      if (!description_.empty() && cmt.content == description_) {
        fout << "[ Line " << cmt.start_line << "-" << cmt.end_line << "] DESCRIPTION\n";
      } else {
        // Si quieres listar también otros bloques multilínea, imprímelos completos:
        fout << "[ Line " << cmt.start_line << "-" << cmt.end_line << "] " << cmt.content << '\n';
      }
    }
  }
  fout << '\n';

  // MODIFICACION: CONDICIONALES
  fout << "CONDICIONALES :\n";
  for (size_t i = 0; i < condicional_.size(); ++i) {
    const CondicionalInfo& condicional = condicional_[i];
    fout << "IF: " << "[ Line " << condicional.line << "] " << "\n";
    fout << "Condicion: " << condicional.condicion << "\n";
    fout << "ELSE: " << (condicional.has_else ? "true" : "false") << '\n';
  }
  fout << '\n';
}

// --- Detecciones ---

void CodeAnalyzer::DetectVariables(const std::string& line, int line_number) {
  // Quitar comentarios de línea y bloques en la misma línea
  std::string work = line.substr(0, line.find("//"));
  work = std::regex_replace(work, std::regex(R"(/\*.*?\*/)"), "");

  // Evitar variables declaradas en la cabecera de un for
  if (std::regex_search(work, std::regex(R"(\bfor\s*\()"))) {
    return;
  }

  // int|double <name> [= <valor>] ;
  std::smatch m;
  std::regex pat(R"(^\s*(int|double)\s+([A-Za-z_]\w*)(\s*=\s*([^;]+))?\s*;)");
  if (std::regex_search(work, m, pat)) {
    VariableInfo v;
    v.type = m.str(1);
    v.name = m.str(2);
    v.line = line_number;
    v.initialized = m[3].matched;
    if (v.initialized) {
      v.init_value = m.str(4);
    }
    variables_.push_back(v);
  }
}

void CodeAnalyzer::DetectLoops(const std::string& line, int line_number) {
  // Quitar comentarios de línea y bloques en la misma línea
  std::string work = line.substr(0, line.find("//"));
  work = std::regex_replace(work, std::regex(R"(/\*.*?\*/)"), "");

  // Busca for(...) y while(...)
  if (std::regex_search(work, std::regex(R"(\bfor\s*\()"))) {
    LoopInfo lp;
    lp.type = "for";
    lp.line = line_number;
    loops_.push_back(lp);
  }
  if (std::regex_search(work, std::regex(R"(\bwhile\s*\()"))) {
    LoopInfo lp;
    lp.type = "while";
    lp.line = line_number;
    loops_.push_back(lp);
  }
}

void CodeAnalyzer::DetectComments(const std::string& line, int line_number) {
  // Estado mínimo para bloques /* ... */
  static bool inside_multi = false;
  static int multi_start = 0;
  static std::string buffer;

  if (inside_multi) {
    // Seguimos acumulando hasta cerrar */
    buffer += line;
    buffer += "\n";
    if (line.find("*/") != std::string::npos) {
      CommentInfo c;
      c.type = "multi-line";
      c.start_line = multi_start;
      c.end_line = line_number;
      c.content = buffer;
      comments_.push_back(c);
      if (description_.empty() && multi_start <= 5) {
        description_ = c.content;
      }
      inside_multi = false;
      buffer.clear();
    }
    return; // no procesar // mientras estamos en bloque
  }

  // Bloque /* ... */ en una sola línea
  std::size_t open_pos = line.find("/*");
  std::size_t close_pos = line.find("*/");
  if (open_pos != std::string::npos && close_pos != std::string::npos && close_pos > open_pos) {
    CommentInfo c;
    c.type = "multi-line";
    c.start_line = line_number;
    c.end_line = line_number;
    c.content = line.substr(open_pos, (close_pos - open_pos) + 2);
    comments_.push_back(c);
    if (description_.empty() && line_number <= 5) {
      description_ = c.content;
    }
  } else if (open_pos != std::string::npos) {
    // Inicio de bloque multilínea
    inside_multi = true;
    multi_start = line_number;
    buffer.clear();
    buffer += line;
    buffer += "\n";
    return;
  }

  // Comentario de una línea: ignora '//' si va justo después de ':' (http://, https://)
  std::size_t pos = line.find("//");
  if (pos != std::string::npos) {
    if (pos == 0 || line[pos - 1] != ':') {
      CommentInfo c2;
      c2.type = "single-line";
      c2.start_line = line_number;
      c2.end_line = line_number;
      c2.content = line.substr(pos);
      comments_.push_back(c2);
    }
  }
}

void CodeAnalyzer::DetectMain(const std::string& line) {
  // Quitar comentarios de línea y bloques
  std::string work = line.substr(0, line.find("//"));
  work = std::regex_replace(work, std::regex(R"(/\*.*?\*/)"), "");

  if (std::regex_search(work, std::regex(R"(\bint\s+main\s*\()"))) {
    has_main_ = true;
  }
}

// Modificación
void CodeAnalyzer::DetectCondicional(const std::string& line, int line_number) {
  // Quitar comentarios de linea y bloques
  std::string work = line.substr(0, line.find("//"));
  work = std::regex_replace(work, std::regex(R"(/\*.*?\*/)"), "");

  // Detectar if
  std::smatch m;
  if (std::regex_search(work, m, std::regex(R"(\bif\s*\(([^)]*)\))"))) {
    CondicionalInfo c;
    c.line = line_number;
    c.condicion = m.str(1);
    c.has_else = false;
    condicional_.push_back(c);
    return;
  }

  // Detectar else
  std::size_t pos = work.find("else");
  if (pos != std::string::npos) {
    // Marca el último if pendiente
    for (int i = (int)condicional_.size() - 1; i >= 0; --i) {
      if (!condicional_[i].has_else) {
        condicional_[i].has_else = true;
        break;
      }
    }
  }
}