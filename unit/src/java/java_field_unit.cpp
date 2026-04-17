/// @file
/// @brief Реализация генератора поля Java.
#include <src/java/java_field_unit.hpp>
#include <utility>

namespace codegen::java
{

JavaFieldUnit::JavaFieldUnit(std::string name, std::string type, Flags flagsValue)
    : codegen::detail::AbstractFieldUnit(std::move(name), std::move(type), flagsValue)
{
}

std::string JavaFieldUnit::RenderPrefixModifiers() const
{
    std::string result;
    if (GetFieldFlags() & codegen::ToFlags(codegen::MethodModifier::staticModifier))
    {
        result += "static ";
    }
    if (GetFieldFlags() & codegen::ToFlags(codegen::MethodModifier::finalModifier))
    {
        result += "final ";
    }
    return result;
}

}  // namespace codegen::java
