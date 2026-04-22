/// @file
/// @brief Реализация генератора Java-метода.
#include <src/java/java_method_unit.hpp>
#include <utility>

namespace codegen::java
{

namespace
{

std::string RenderJavaMethodPrefixModifiers(codegen::CodeUnit::Flags methodFlags)
{
    switch (methodFlags)
    {
        case 0:
            return "";
        case codegen::ToFlags(codegen::MethodModifier::StaticModifier):
            return "static ";
        case codegen::ToFlags(codegen::MethodModifier::FinalModifier):
            return "final ";
        case codegen::ToFlags(codegen::MethodModifier::AbstractModifier):
            return "abstract ";
        case codegen::ToFlags(codegen::MethodModifier::StaticModifier) |
             codegen::ToFlags(codegen::MethodModifier::FinalModifier):
            return "static final ";
        case codegen::ToFlags(codegen::MethodModifier::StaticModifier) |
             codegen::ToFlags(codegen::MethodModifier::AbstractModifier):
            return "static abstract ";
        case codegen::ToFlags(codegen::MethodModifier::FinalModifier) |
             codegen::ToFlags(codegen::MethodModifier::AbstractModifier):
            return "final abstract ";
        case codegen::ToFlags(codegen::MethodModifier::StaticModifier) |
             codegen::ToFlags(codegen::MethodModifier::FinalModifier) |
             codegen::ToFlags(codegen::MethodModifier::AbstractModifier):
            return "static final abstract ";
        default:
            throw std::invalid_argument("Unsupported Java method modifier");
    }
}

}  // namespace

JavaMethodUnit::JavaMethodUnit(std::string name, std::string returnType, Flags flagsValue)
    : codegen::detail::AbstractMethodUnit(std::move(name), std::move(returnType), flagsValue)
{
}

std::string JavaMethodUnit::RenderPrefixModifiers() const
{
    return RenderJavaMethodPrefixModifiers(GetMethodFlags());
}

bool JavaMethodUnit::IsAbstractMethod() const
{
    return (GetMethodFlags() & codegen::ToFlags(codegen::MethodModifier::AbstractModifier)) != 0;
}

std::string JavaMethodUnit::RenderAbstractTerminator() const
{
    return ";\n";
}

}  // namespace codegen::java
