/// @file
/// @brief Реализация генератора поля C++.
#include <src/cpp/cpp_field_unit.hpp>
#include <stdexcept>

using codegen::CppFieldUnit;

namespace
{

std::string RenderCppFieldPrefixModifiers(codegen::CodeUnit::Flags fieldFlags)
{
    switch (codegen::ToMethodModifierMask(fieldFlags))
    {
        case codegen::MethodModifier::Unknown:
            return "";
        case codegen::MethodModifier::StaticModifier:
            return "static ";
        case codegen::MethodModifier::ConstModifier:
            return "const ";
        case codegen::MethodModifier::StaticConstModifier:
            return "static const ";
        default:
            throw std::invalid_argument("Unsupported C++ field modifier");
    }
}

}  // namespace

CppFieldUnit::CppFieldUnit(const std::string& name, const std::string& type, Flags flagsValue)
    : codegen::detail::AbstractFieldUnit(name, type, flagsValue)
{
}

std::string CppFieldUnit::RenderPrefixModifiers() const
{
    return RenderCppFieldPrefixModifiers(GetFieldFlags());
}
