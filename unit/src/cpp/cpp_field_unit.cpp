/// @file
/// @brief Реализация генератора поля C++.
#include <src/cpp/cpp_field_unit.hpp>
#include <stdexcept>
#include <string>

namespace codegen::cpp
{

namespace
{

std::string RenderCppFieldPrefixModifiers(codegen::MethodModifier fieldFlags)
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
            throw std::invalid_argument("Unsupported C++ field modifier: " +
                                        std::to_string(static_cast<unsigned int>(fieldFlags)));
    }
}

}  // namespace

CppFieldUnit::CppFieldUnit(const std::string& name, const std::string& type, MethodModifier flagsValue)
    : codegen::detail::AbstractFieldUnit(name, type, flagsValue)
{
}

std::string CppFieldUnit::RenderPrefixModifiers() const
{
    return RenderCppFieldPrefixModifiers(GetFieldFlags());
}

}  // namespace codegen::cpp
