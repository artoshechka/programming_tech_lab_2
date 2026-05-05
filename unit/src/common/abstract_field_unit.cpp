/// @file
/// @brief Реализация базового абстрактного класса для генерации поля.
#include <src/common/abstract_field_unit.hpp>
#include <stdexcept>
#include <utility>

namespace codegen::detail
{

AbstractFieldUnit::AbstractFieldUnit(std::string name, std::string type, MethodModifier flagsValue)
    : name_(std::move(name)), type_(std::move(type)), flags_(flagsValue)
{
}

std::string AbstractFieldUnit::Render(unsigned int indentLevel) const
{
    if (name_.empty())
    {
        throw std::runtime_error("Field name must not be empty");
    }
    if (type_.empty())
    {
        throw std::runtime_error("Field type must not be empty");
    }

    std::string result = MakeIndent(indentLevel);
    result += RenderPrefixModifiers();
    result += type_ + " " + name_;
    result += RenderSuffixModifiers();
    result += ";\n";
    return result;
}

std::string AbstractFieldUnit::RenderSuffixModifiers() const
{
    return "";
}

codegen::MethodModifier AbstractFieldUnit::GetFieldFlags() const
{
    return flags_;
}

}  // namespace codegen::detail
