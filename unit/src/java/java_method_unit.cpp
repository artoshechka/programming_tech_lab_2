/// @file
/// @brief Реализация генератора Java-метода.
#include <src/java/java_method_unit.hpp>
#include <stdexcept>
#include <utility>

namespace codegen::java
{

namespace
{

std::string RenderJavaMethodPrefixModifiers(codegen::MethodModifier methodFlags)
{
    switch (codegen::ToMethodModifierMask(methodFlags))
    {
        case codegen::MethodModifier::Unknown:
            return "";
        case codegen::MethodModifier::StaticModifier:
            return "static ";
        case codegen::MethodModifier::FinalModifier:
            return "final ";
        case codegen::MethodModifier::AbstractModifier:
            return "abstract ";
        case codegen::MethodModifier::StaticFinalModifier:
            return "static final ";
        case codegen::MethodModifier::StaticAbstractModifier:
            return "static abstract ";
        case codegen::MethodModifier::FinalAbstractModifier:
            return "final abstract ";
        case codegen::MethodModifier::StaticFinalAbstractModifier:
            return "static final abstract ";
        default:
            throw std::invalid_argument("Unsupported Java method modifier");
    }
}

}  // namespace

JavaMethodUnit::JavaMethodUnit(std::string name, std::string returnType, MethodModifier flagsValue)
    : codegen::detail::AbstractMethodUnit(std::move(name), std::move(returnType), flagsValue)
{
}

std::string JavaMethodUnit::RenderPrefixModifiers() const
{
    return RenderJavaMethodPrefixModifiers(GetMethodFlags());
}

bool JavaMethodUnit::IsAbstractMethod() const
{
    return (GetMethodFlags() & codegen::MethodModifier::AbstractModifier) != 0;
}

std::string JavaMethodUnit::RenderAbstractTerminator() const
{
    return ";\n";
}

}  // namespace codegen::java
