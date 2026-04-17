/// @file
/// @brief Реализация базового абстрактного класса для генерации поля.
#include <src/common/abstract_field_unit.hpp>
#include <utility>

namespace codegen::detail
{

AbstractFieldUnit::AbstractFieldUnit(std::string name, std::string type, Flags flagsValue)
    : name_(std::move(name)), type_(std::move(type)), flags_(flagsValue)
{
}

std::string AbstractFieldUnit::Render(unsigned int indentLevel) const
{
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

codegen::CodeUnit::Flags AbstractFieldUnit::GetFieldFlags() const
{
    return flags_;
}

}  // namespace codegen::detail
