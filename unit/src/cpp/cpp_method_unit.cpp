/// @file
/// @brief Определение класса для генерации кода метода.
#include <src/cpp/cpp_method_unit.hpp>
#include <stdexcept>

using codegen::CppMethodUnit;

namespace
{

std::string RenderCppMethodPrefixModifiers(codegen::MethodModifier methodFlags)
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
            throw std::invalid_argument("Unsupported C++ method modifier");
    }
}

std::string RenderCppMethodSuffixModifiers(codegen::MethodModifier methodFlags)
{
    switch (codegen::ToMethodModifierMask(
        methodFlags & (codegen::MethodModifier::FinalModifier | codegen::MethodModifier::ConstModifier)))
    {
        case codegen::MethodModifier::Unknown:
            return "";
        case codegen::MethodModifier::FinalModifier:
            return " final";
        case codegen::MethodModifier::ConstModifier:
            return " const";
        case codegen::MethodModifier::FinalConstModifier:
            return " final const";
        default:
            throw std::invalid_argument("Unsupported C++ method modifier");
    }
}

}  // namespace

CppMethodUnit::CppMethodUnit(const std::string& name, const std::string& returnType, MethodModifier flagsValue)
    : codegen::detail::AbstractMethodUnit(name, returnType, flagsValue)
{
}

std::string CppMethodUnit::RenderPrefixModifiers() const
{
    return RenderCppMethodPrefixModifiers(GetMethodFlags());
}

std::string CppMethodUnit::RenderSuffixModifiers() const
{
    return RenderCppMethodSuffixModifiers(GetMethodFlags());
}

bool CppMethodUnit::IsAbstractMethod() const
{
    return (GetMethodFlags() & MethodModifier::AbstractModifier) != 0;
}

std::string CppMethodUnit::RenderAbstractTerminator() const
{
    return " = 0;\n";
}