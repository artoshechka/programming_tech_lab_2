/// @file
/// @brief Определение класса для генерации оператора печати.
#include <src/cpp/cpp_print_unit.hpp>

using codegen::PrintStatementUnit;

PrintStatementUnit::PrintStatementUnit(const std::string& text) : codegen::detail::AbstractPrintUnit(text)
{
}

std::string PrintStatementUnit::RenderPrintExpression(const std::string& text) const
{
    return "printf( \"" + text + "\" );";
}