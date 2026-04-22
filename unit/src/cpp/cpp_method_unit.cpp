/// @file
/// @brief Определение класса для генерации кода метода.
#include <src/cpp/cpp_method_unit.hpp>
#include <stdexcept>

using codegen::CppMethodUnit;

namespace
{

std::string RenderCppMethodPrefixModifiers(codegen::CodeUnit::Flags methodFlags)
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
            throw std::invalid_argument("Unsupported C++ method modifier");
    }
}

std::string RenderCppMethodSuffixModifiers(codegen::CodeUnit::Flags methodFlags)
{
    switch (methodFlags &
            (codegen::ToFlags(codegen::MethodModifier::FinalModifier) |
             codegen::ToFlags(codegen::MethodModifier::ConstModifier)))
    {
        case 0:
            return "";
        case codegen::ToFlags(codegen::MethodModifier::FinalModifier):
            return " final";
        case codegen::ToFlags(codegen::MethodModifier::ConstModifier):
            return " const";
        case codegen::ToFlags(codegen::MethodModifier::FinalModifier) |
            codegen::ToFlags(codegen::MethodModifier::ConstModifier):
            return " final const";
        default:
            throw std::invalid_argument("Unsupported C++ method modifier");
    }
}

}  // namespace

CppMethodUnit::CppMethodUnit(const std::string& name, const std::string& returnType, Flags flagsValue)
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
    return (GetMethodFlags() & ToFlags(MethodModifier::AbstractModifier)) != 0;
}

std::string CppMethodUnit::RenderAbstractTerminator() const
{
    return " = 0;\n";
}