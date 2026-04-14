#include <unit/src/print_operator_unit.hpp>

PrintOperatorUnit::PrintOperatorUnit(const std::string &text) : text_(text)
{
}

std::string PrintOperatorUnit::Compile(unsigned int level) const
{
    return GenerateShift(level) + "printf( \"" + text_ + "\" );\n";
}
