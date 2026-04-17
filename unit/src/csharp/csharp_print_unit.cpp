/// @file
/// @brief Реализация генератора C# print statement.
#include <src/csharp/csharp_print_unit.hpp>
#include <utility>

namespace codegen::csharp
{

CSharpPrintUnit::CSharpPrintUnit(std::string text) : codegen::detail::AbstractPrintUnit(std::move(text))
{
}

std::string CSharpPrintUnit::RenderPrintExpression(const std::string& text) const
{
    return "Console.WriteLine(\"" + text + "\");";
}

}  // namespace codegen::csharp
