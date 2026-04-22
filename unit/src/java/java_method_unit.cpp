/// @file
/// @brief Реализация генератора Java-метода.
#include <src/java/java_method_unit.hpp>
#include <utility>

namespace codegen::java
{

JavaMethodUnit::JavaMethodUnit(std::string name, std::string returnType, Flags flagsValue)
    : codegen::detail::AbstractMethodUnit(std::move(name), std::move(returnType), flagsValue)
{
}

std::string JavaMethodUnit::RenderPrefixModifiers() const
{
    std::string result;
    if (GetMethodFlags() & codegen::ToFlags(codegen::MethodModifier::StaticModifier))
    {
        result += "static ";
    }
    if (GetMethodFlags() & codegen::ToFlags(codegen::MethodModifier::FinalModifier))
    {
        result += "final ";
    }
    if (GetMethodFlags() & codegen::ToFlags(codegen::MethodModifier::AbstractModifier))
    {
        result += "abstract ";
    }
    return result;
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
