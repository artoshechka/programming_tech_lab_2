/// @file
/// @brief Реализация генератора Java print statement.
#include <src/java/java_print_unit.hpp>
#include <utility>

namespace codegen::java
{

JavaPrintUnit::JavaPrintUnit(std::string text) : codegen::detail::AbstractPrintUnit(std::move(text))
{
}

std::string JavaPrintUnit::RenderPrintExpression(const std::string& text) const
{
    return "System.out.println(\"" + text + "\");";
}

}  // namespace codegen::java
