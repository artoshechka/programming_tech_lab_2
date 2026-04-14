#include <src/print_operator_unit.hpp>

using codegen::PrintStatementUnit;
PrintStatementUnit::PrintStatementUnit(const std::string& text) : text_(text)
{
}

std::string PrintStatementUnit::Render(unsigned int indent_level) const
{
    return MakeIndent(indent_level) + "printf( \"" + text_ + "\" );\n";
}
