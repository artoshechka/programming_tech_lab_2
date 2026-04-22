/// @file
/// @brief Реализация генератора поля C#.
#include <src/csharp/csharp_field_unit.hpp>
#include <stdexcept>
#include <utility>

namespace codegen::csharp
{

namespace
{

std::string RenderCSharpFieldPrefixModifiers(codegen::CodeUnit::Flags fieldFlags)
{
    switch (fieldFlags &
            (codegen::ToFlags(codegen::MethodModifier::StaticModifier) |
             codegen::ToFlags(codegen::MethodModifier::FinalModifier)))
    {
        case 0:
            return "";
        case codegen::ToFlags(codegen::MethodModifier::StaticModifier):
            return "static ";
        case codegen::ToFlags(codegen::MethodModifier::FinalModifier):
            return "readonly ";
        case codegen::ToFlags(codegen::MethodModifier::StaticModifier) |
             codegen::ToFlags(codegen::MethodModifier::FinalModifier):
            return "static readonly ";
        default:
            throw std::invalid_argument("Unsupported C# field modifier");
    }
}

}  // namespace

CSharpFieldUnit::CSharpFieldUnit(std::string name, std::string type, Flags flagsValue)
    : codegen::detail::AbstractFieldUnit(std::move(name), std::move(type), flagsValue)
{
}

std::string CSharpFieldUnit::RenderPrefixModifiers() const
{
    return RenderCSharpFieldPrefixModifiers(GetFieldFlags());
}

}  // namespace codegen::csharp
