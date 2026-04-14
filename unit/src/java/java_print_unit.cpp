/// @file
/// @brief Реализация генератора Java print statement.
#include <src/java/java_print_unit.hpp>
#include <utility>

namespace codegen::java
{

JavaPrintUnit::JavaPrintUnit(std::string text) : text_(std::move(text))
{
}

std::string JavaPrintUnit::Render(unsigned int indentLevel) const
{
    return MakeIndent(indentLevel) + "System.out.println(\"" + text_ + "\");\n";
}

}  // namespace codegen::java
