/// @file
/// @brief Реализация генератора поля Java.
#include <src/java/java_field_unit.hpp>
#include <stdexcept>
#include <string>
#include <utility>

namespace codegen::java
{

namespace
{

std::string RenderJavaFieldPrefixModifiers(codegen::MethodModifier fieldFlags)
{
    switch (codegen::ToMethodModifierMask(fieldFlags))
    {
        case codegen::MethodModifier::Unknown:
            return "";
        case codegen::MethodModifier::StaticModifier:
            return "static ";
        case codegen::MethodModifier::FinalModifier:
            return "final ";
        case codegen::MethodModifier::StaticFinalModifier:
            return "static final ";
        default:
            throw std::invalid_argument("Unsupported Java field modifier: " +
                                        std::to_string(static_cast<unsigned int>(fieldFlags)));
    }
}

}  // namespace

JavaFieldUnit::JavaFieldUnit(std::string name, std::string type, MethodModifier flagsValue)
    : codegen::detail::AbstractFieldUnit(std::move(name), std::move(type), flagsValue)
{
}

std::string JavaFieldUnit::RenderPrefixModifiers() const
{
    return RenderJavaFieldPrefixModifiers(GetFieldFlags());
}

}  // namespace codegen::java
