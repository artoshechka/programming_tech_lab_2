/// @file
/// @brief Реализация генератора поля C++.
#include <src/cpp/cpp_field_unit.hpp>

using codegen::FieldDeclarationUnit;

FieldDeclarationUnit::FieldDeclarationUnit(const std::string& name, const std::string& type, Flags flagsValue)
    : codegen::detail::AbstractFieldUnit(name, type, flagsValue)
{
}

std::string FieldDeclarationUnit::RenderPrefixModifiers() const
{
    std::string result;
    if (GetFieldFlags() & ToFlags(MethodModifier::staticModifier))
    {
        result += "static ";
    }
    if (GetFieldFlags() & ToFlags(MethodModifier::constModifier))
    {
        result += "const ";
    }
    return result;
}
