/// @file
/// @brief Реализация генератора поля C#.
#include <src/csharp/csharp_field_unit.hpp>
#include <stdexcept>
#include <utility>

namespace codegen::csharp
{

namespace
{

std::string RenderCSharpFieldPrefixModifiers(codegen::MethodModifier fieldFlags)
{
    switch (codegen::ToMethodModifierMask(
        fieldFlags & (codegen::MethodModifier::StaticModifier | codegen::MethodModifier::FinalModifier)))
    {
        case codegen::MethodModifier::Unknown:
            return "";
        case codegen::MethodModifier::StaticModifier:
            return "static ";
        case codegen::MethodModifier::FinalModifier:
            return "readonly ";
        case codegen::MethodModifier::StaticFinalModifier:
            return "static readonly ";
        default:
            throw std::invalid_argument("Unsupported C# field modifier");
    }
}

}  // namespace

CSharpFieldUnit::CSharpFieldUnit(std::string name, std::string type, MethodModifier flagsValue)
    : codegen::detail::AbstractFieldUnit(std::move(name), std::move(type), flagsValue)
{
}

std::string CSharpFieldUnit::RenderPrefixModifiers() const
{
    return RenderCSharpFieldPrefixModifiers(GetFieldFlags());
}

}  // namespace codegen::csharp
