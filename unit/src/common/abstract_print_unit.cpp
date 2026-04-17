/// @file
/// @brief Реализация базового абстрактного класса для генерации печати.
#include <src/common/abstract_print_unit.hpp>
#include <utility>

namespace codegen::detail
{

AbstractPrintUnit::AbstractPrintUnit(std::string text) : text_(std::move(text))
{
}

std::string AbstractPrintUnit::Render(unsigned int indentLevel) const
{
    return MakeIndent(indentLevel) + RenderPrintExpression(text_) + "\n";
}

const std::string& AbstractPrintUnit::GetPrintText() const
{
    return text_;
}

}  // namespace codegen::detail
