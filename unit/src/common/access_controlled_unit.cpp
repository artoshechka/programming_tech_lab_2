/// @file
/// @brief Реализация вспомогательного узла для префиксирования доступа.
#include <src/common/access_controlled_unit.hpp>
#include <stdexcept>
#include <utility>

namespace codegen::detail
{

AccessControlledUnit::AccessControlledUnit(std::string accessKeyword, std::shared_ptr<codegen::CodeUnit> unit)
    : accessKeyword_(std::move(accessKeyword)), unit_(std::move(unit))
{
}

std::string AccessControlledUnit::Render(unsigned int indentLevel) const
{
    if (accessKeyword_.empty())
    {
        throw std::runtime_error("Access keyword must not be empty");
    }
    if (!unit_)
    {
        throw std::runtime_error("Wrapped code unit must not be null");
    }

    std::string rendered = unit_->Render(indentLevel);
    const std::string indent = MakeIndent(indentLevel);

    if (rendered.rfind(indent, 0) == 0)
    {
        rendered.insert(indent.size(), accessKeyword_ + " ");
    }
    return rendered;
}

}  // namespace codegen::detail
