/// @file
/// @brief Определение класса для генерации оператора печати.
#include <src/cpp/cpp_print_unit.hpp>

namespace codegen::cpp
{

CppPrintUnit::CppPrintUnit(const std::string& text) : codegen::detail::AbstractPrintUnit(text)
{
}

std::string CppPrintUnit::RenderPrintExpression(const std::string& text) const
{
    return "printf( \"" + text + "\" );";
}

}  // namespace codegen::cpp