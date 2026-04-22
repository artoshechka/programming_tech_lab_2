/// @file
/// @brief Реализация генератора поля C++.
#include <src/cpp/cpp_field_unit.hpp>

using codegen::CppFieldUnit;

CppFieldUnit::CppFieldUnit(const std::string& name, const std::string& type, Flags flagsValue)
    : codegen::detail::AbstractFieldUnit(name, type, flagsValue)
{
}

std::string CppFieldUnit::RenderPrefixModifiers() const
{
    std::string result;
    if (GetFieldFlags() & ToFlags(MethodModifier::StaticModifier))
    {
        result += "static ";
    }
    if (GetFieldFlags() & ToFlags(MethodModifier::ConstModifier))
    {
        result += "const ";
    }
    return result;
}
