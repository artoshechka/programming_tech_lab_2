/// @file
/// @brief Реализация генератора поля C#.
#include <src/csharp/csharp_field_unit.hpp>
#include <utility>

namespace codegen::csharp
{

CSharpFieldUnit::CSharpFieldUnit(std::string name, std::string type, Flags flagsValue)
    : codegen::detail::AbstractFieldUnit(std::move(name), std::move(type), flagsValue)
{
}

std::string CSharpFieldUnit::RenderPrefixModifiers() const
{
    std::string result;
    if (GetFieldFlags() & codegen::ToFlags(codegen::MethodModifier::StaticModifier))
    {
        result += "static ";
    }
    if (GetFieldFlags() & codegen::ToFlags(codegen::MethodModifier::FinalModifier))
    {
        result += "readonly ";
    }
    return result;
}

}  // namespace codegen::csharp
