/// @file
/// @brief Реализация генератора C#-метода.
#include <src/csharp/csharp_method_unit.hpp>
#include <stdexcept>
#include <utility>

namespace codegen::csharp
{

namespace
{

std::string RenderCSharpMethodPrimaryPrefix(codegen::CodeUnit::Flags methodFlags)
{
    switch (codegen::ToMethodModifierMask(
        methodFlags & (codegen::MethodModifier::StaticModifier | codegen::MethodModifier::VirtualModifier)))
    {
        case codegen::MethodModifier::Unknown:
            return "";
        case codegen::MethodModifier::StaticModifier:
            return "static ";
        case codegen::MethodModifier::VirtualModifier:
            return "virtual ";
        case codegen::MethodModifier::StaticVirtualModifier:
            return "static ";
        default:
            throw std::invalid_argument("Unsupported C# method prefix modifier");
    }
}

std::string RenderCSharpMethodSecondaryPrefix(codegen::CodeUnit::Flags methodFlags)
{
    switch (codegen::ToMethodModifierMask(
        methodFlags & (codegen::MethodModifier::AbstractModifier | codegen::MethodModifier::FinalModifier)))
    {
        case codegen::MethodModifier::Unknown:
            return "";
        case codegen::MethodModifier::AbstractModifier:
            return "abstract ";
        case codegen::MethodModifier::FinalModifier:
            return "sealed ";  // В C# используется sealed вместо final
        case codegen::MethodModifier::FinalAbstractModifier:
            return "abstract sealed ";
        default:
            throw std::invalid_argument("Unsupported C# method suffix modifier");
    }
}

}  // namespace

CSharpMethodUnit::CSharpMethodUnit(std::string name, std::string returnType, Flags flagsValue)
    : codegen::detail::AbstractMethodUnit(std::move(name), std::move(returnType), flagsValue)
{
}

std::string CSharpMethodUnit::RenderPrefixModifiers() const
{
    return RenderCSharpMethodPrimaryPrefix(GetMethodFlags()) + RenderCSharpMethodSecondaryPrefix(GetMethodFlags());
}

bool CSharpMethodUnit::IsAbstractMethod() const
{
    return (GetMethodFlags() & codegen::MethodModifier::AbstractModifier) != 0;
}

std::string CSharpMethodUnit::RenderAbstractTerminator() const
{
    return ";\n";
}

}  // namespace codegen::csharp
