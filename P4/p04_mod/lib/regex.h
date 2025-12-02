#ifndef REGEX_H
#define REGEX_H

#include <vector>
#include "estructuras.h"

class CodeAnalyzer {
  public:
    CodeAnalyzer(const std::string& input, const std::string& output);
    void Analyze();
    void WriteReport() const;

    void DetectVariables(const std::string& line, int line_number);
    void DetectLoops(const std::string& line, int line_number);
    void DetectComments(const std::string& line, int line_number);
    void DetectMain(const std::string& line);
    // Modificacion
    void DetectCondicional(const std::string&line, int line_number);

  private:
    std::string input_file_;
    std::string output_file_;
    std::vector<VariableInfo> variables_;
    std::vector<LoopInfo> loops_;
    std::vector<CommentInfo> comments_;
    bool has_main_ = false;
    std::string description_;
    // Modificación
    std::vector<CondicionalInfo> condicional_;
};

#endif
