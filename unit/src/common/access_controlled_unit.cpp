/// @file
/// @brief Реализация вспомогательного узла для префиксирования доступа.
#include <src/common/access_controlled_unit.hpp>
#include <utility>

namespace codegen::detail
{

AccessControlledUnit::AccessControlledUnit(std::string accessKeyword, std::shared_ptr<codegen::CodeUnit> unit)
    : accessKeyword_(std::move(accessKeyword)), unit_(std::move(unit))
{
}

std::string AccessControlledUnit::Render(unsigned int indentLevel) const
{
    std::string rendered = unit_->Render(indentLevel);
    const std::string indent = MakeIndent(indentLevel);

    if (rendered.rfind(indent, 0) == 0)
    {
        rendered.insert(indent.size(), accessKeyword_ + " ");
    }
    return rendered;
}

}  // namespace codegen::detail
