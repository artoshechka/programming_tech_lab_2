/// @file
/// @brief Определение класса для генерации оператора печати.
/// @author Artemenko Anton
#include <src/print_operator_unit.hpp>

using codegen::PrintStatementUnit;
PrintStatementUnit::PrintStatementUnit(const std::string& text) : text_(text)
{
}

std::string PrintStatementUnit::Render(unsigned int indentLevel) const
{
    return MakeIndent(indentLevel) + "printf( \"" + text_ + "\" );\n";
}
