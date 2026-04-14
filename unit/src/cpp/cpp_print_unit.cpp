/// @file
/// @brief Определение класса для генерации оператора печати.
#include <src/cpp/cpp_print_unit.hpp>

using codegen::PrintStatementUnit;

PrintStatementUnit::PrintStatementUnit(const std::string& text) : text_(text)
{
}

std::string PrintStatementUnit::Render(unsigned int indentLevel) const
{
    return MakeIndent(indentLevel) + "printf( \"" + text_ + "\" );\n";
}