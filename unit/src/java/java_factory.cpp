/// @file
/// @brief Реализация фабрики генерации кода на Java.
#include <src/java/java_class_unit.hpp>
#include <src/java/java_field_unit.hpp>
#include <src/java/java_factory.hpp>
#include <src/java/java_method_unit.hpp>
#include <src/java/java_print_unit.hpp>

namespace codegen::java
{

std::shared_ptr<codegen::CodeUnit> JavaCodeFactory::CreateClass(const std::string& name,
                                                                codegen::CodeUnit::Flags flagsValue) const
{
    return std::make_shared<JavaClassUnit>(name, flagsValue);
}

std::shared_ptr<codegen::CodeUnit> JavaCodeFactory::CreateMethod(const std::string& name, const std::string& returnType,
                                                                 codegen::CodeUnit::Flags flagsValue) const
{
    return std::make_shared<JavaMethodUnit>(name, returnType, flagsValue);
}

std::shared_ptr<codegen::CodeUnit> JavaCodeFactory::CreateField(const std::string& name, const std::string& type,
                                                                codegen::CodeUnit::Flags flagsValue) const
{
    return std::make_shared<JavaFieldUnit>(name, type, flagsValue);
}

std::shared_ptr<codegen::CodeUnit> JavaCodeFactory::CreatePrintStatement(const std::string& text) const
{
    return std::make_shared<JavaPrintUnit>(text);
}

std::string JavaCodeFactory::GetLanguageName() const
{
    return "Java";
}

}  // namespace codegen::java
