/// @file
/// @brief Реализация генератора C#-метода.
#include <src/csharp/csharp_method_unit.hpp>
#include <utility>

namespace codegen::csharp
{

CSharpMethodUnit::CSharpMethodUnit(std::string name, std::string returnType, Flags flagsValue)
    : codegen::detail::AbstractMethodUnit(std::move(name), std::move(returnType), flagsValue)
{
}

std::string CSharpMethodUnit::RenderPrefixModifiers() const
{
    std::string result;
    if (GetMethodFlags() & codegen::ToFlags(codegen::MethodModifier::StaticModifier))
    {
        result += "static ";
    } else if (GetMethodFlags() & codegen::ToFlags(codegen::MethodModifier::VirtualModifier))
    {
        result += "virtual ";
    }
    if (GetMethodFlags() & codegen::ToFlags(codegen::MethodModifier::AbstractModifier))
    {
        result += "abstract ";
    }
    if (GetMethodFlags() & codegen::ToFlags(codegen::MethodModifier::FinalModifier))
    {
        result += "sealed ";  // В C# используется sealed вместо final
    }
    return result;
}

bool CSharpMethodUnit::IsAbstractMethod() const
{
    return (GetMethodFlags() & codegen::ToFlags(codegen::MethodModifier::AbstractModifier)) != 0;
}

std::string CSharpMethodUnit::RenderAbstractTerminator() const
{
    return ";\n";
}

}  // namespace codegen::csharp
