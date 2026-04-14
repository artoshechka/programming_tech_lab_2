/// @file
/// @brief Реализация генератора C# print statement.
#include <src/csharp/csharp_print_unit.hpp>
#include <utility>

namespace codegen::csharp
{

CSharpPrintUnit::CSharpPrintUnit(std::string text) : text_(std::move(text))
{
}

std::string CSharpPrintUnit::Render(unsigned int indentLevel) const
{
    return MakeIndent(indentLevel) + "Console.WriteLine(\"" + text_ + "\");\n";
}

}  // namespace codegen::csharp
