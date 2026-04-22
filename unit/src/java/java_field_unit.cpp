/// @file
/// @brief Реализация генератора поля Java.
#include <src/java/java_field_unit.hpp>
#include <utility>

namespace codegen::java
{

namespace
{

std::string RenderJavaFieldPrefixModifiers(codegen::CodeUnit::Flags fieldFlags)
{
    switch (fieldFlags)
    {
        case 0:
            return "";
        case codegen::ToFlags(codegen::MethodModifier::StaticModifier):
            return "static ";
        case codegen::ToFlags(codegen::MethodModifier::FinalModifier):
            return "final ";
        case codegen::ToFlags(codegen::MethodModifier::StaticModifier) |
             codegen::ToFlags(codegen::MethodModifier::FinalModifier):
            return "static final ";
        default:
            throw std::invalid_argument("Unsupported Java field modifier");
    }
}

}  // namespace

JavaFieldUnit::JavaFieldUnit(std::string name, std::string type, Flags flagsValue)
    : codegen::detail::AbstractFieldUnit(std::move(name), std::move(type), flagsValue)
{
}

std::string JavaFieldUnit::RenderPrefixModifiers() const
{
    return RenderJavaFieldPrefixModifiers(GetFieldFlags());
}

}  // namespace codegen::java
