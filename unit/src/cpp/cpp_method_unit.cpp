/// @file
/// @brief Определение класса для генерации кода метода.
#include <src/cpp/cpp_method_unit.hpp>

using codegen::CppMethodUnit;

CppMethodUnit::CppMethodUnit(const std::string& name, const std::string& returnType, Flags flagsValue)
    : codegen::detail::AbstractMethodUnit(name, returnType, flagsValue)
{
}

std::string CppMethodUnit::RenderPrefixModifiers() const
{
    std::string result;
    if (GetMethodFlags() & ToFlags(MethodModifier::staticModifier))
    {
        result += "static ";
    } else if (GetMethodFlags() & ToFlags(MethodModifier::virtualModifier))
    {
        result += "virtual ";
    }
    return result;
}

std::string CppMethodUnit::RenderSuffixModifiers() const
{
    std::string result;
    if (GetMethodFlags() & ToFlags(MethodModifier::finalModifier))
    {
        result += " final";
    }
    if (GetMethodFlags() & ToFlags(MethodModifier::constModifier))
    {
        result += " const";
    }
    return result;
}

bool CppMethodUnit::IsAbstractMethod() const
{
    return (GetMethodFlags() & ToFlags(MethodModifier::abstractModifier)) != 0;
}

std::string CppMethodUnit::RenderAbstractTerminator() const
{
    return " = 0;\n";
}