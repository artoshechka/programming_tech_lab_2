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
    switch (methodFlags &
            (codegen::ToFlags(codegen::MethodModifier::StaticModifier) |
             codegen::ToFlags(codegen::MethodModifier::VirtualModifier)))
    {
        case 0:
            return "";
        case codegen::ToFlags(codegen::MethodModifier::StaticModifier):
            return "static ";
        case codegen::ToFlags(codegen::MethodModifier::VirtualModifier):
            return "virtual ";
        case codegen::ToFlags(codegen::MethodModifier::StaticModifier) |
             codegen::ToFlags(codegen::MethodModifier::VirtualModifier):
            return "static ";
        default:
            throw std::invalid_argument("Unsupported C# method prefix modifier");
    }
}

std::string RenderCSharpMethodSecondaryPrefix(codegen::CodeUnit::Flags methodFlags)
{
    switch (methodFlags &
            (codegen::ToFlags(codegen::MethodModifier::AbstractModifier) |
             codegen::ToFlags(codegen::MethodModifier::FinalModifier)))
    {
        case 0:
            return "";
        case codegen::ToFlags(codegen::MethodModifier::AbstractModifier):
            return "abstract ";
        case codegen::ToFlags(codegen::MethodModifier::FinalModifier):
            return "sealed ";  // В C# используется sealed вместо final
        case codegen::ToFlags(codegen::MethodModifier::AbstractModifier) |
             codegen::ToFlags(codegen::MethodModifier::FinalModifier):
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
    return RenderCSharpMethodPrimaryPrefix(GetMethodFlags()) +
           RenderCSharpMethodSecondaryPrefix(GetMethodFlags());
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
