/// @file
/// @brief Реализация генератора поля C++.
#include <src/cpp/cpp_field_unit.hpp>
#include <stdexcept>

using codegen::CppFieldUnit;

namespace
{

std::string RenderCppFieldPrefixModifiers(codegen::CodeUnit::Flags fieldFlags)
{
    switch (fieldFlags)
    {
        case 0:
            return "";
        case codegen::ToFlags(codegen::MethodModifier::StaticModifier):
            return "static ";
        case codegen::ToFlags(codegen::MethodModifier::ConstModifier):
            return "const ";
        case codegen::ToFlags(codegen::MethodModifier::StaticModifier) |
             codegen::ToFlags(codegen::MethodModifier::ConstModifier):
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
