#include <unit/src/print_operator_unit.hpp>

PrintOperatorUnit::PrintOperatorUnit(const std::string &text) : m_text(text)
{
}

std::string PrintOperatorUnit::compile(unsigned int level) const
{
    return generateShift(level) + "printf( \"" + m_text + "\" );\n";
}
