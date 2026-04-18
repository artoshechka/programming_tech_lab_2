/// @file
/// @brief Реализация фабрики генерации кода на C++.
#include <src/cpp/cpp_class_unit.hpp>
#include <src/cpp/cpp_factory.hpp>
#include <src/cpp/cpp_field_unit.hpp>
#include <src/cpp/cpp_method_unit.hpp>
#include <src/cpp/cpp_print_unit.hpp>

namespace codegen::cpp
{

std::shared_ptr<codegen::CodeUnit> CppCodeFactory::CreateClass(const std::string& name,
                                                               codegen::CodeUnit::Flags flagsValue) const
{
    return std::make_shared<codegen::CppClassUnit>(name, flagsValue);
}

std::shared_ptr<codegen::CodeUnit> CppCodeFactory::CreateMethod(const std::string& name, const std::string& returnType,
                                                                codegen::CodeUnit::Flags flagsValue) const
{
    return std::make_shared<codegen::CppMethodUnit>(name, returnType, flagsValue);
}

std::shared_ptr<codegen::CodeUnit> CppCodeFactory::CreateField(const std::string& name, const std::string& type,
                                                               codegen::CodeUnit::Flags flagsValue) const
{
    return std::make_shared<codegen::CppFieldUnit>(name, type, flagsValue);
}

std::shared_ptr<codegen::CodeUnit> CppCodeFactory::CreatePrintStatement(const std::string& text) const
{
    return std::make_shared<codegen::CppPrintUnit>(text);
}

std::string CppCodeFactory::GetLanguageName() const
{
    return "C++";
}

}  // namespace codegen::cpp
