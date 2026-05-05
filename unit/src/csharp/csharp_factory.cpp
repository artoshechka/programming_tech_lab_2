/// @file
/// @brief Реализация фабрики генерации кода на C#.
#include <src/csharp/csharp_class_unit.hpp>
#include <src/csharp/csharp_factory.hpp>
#include <src/csharp/csharp_field_unit.hpp>
#include <src/csharp/csharp_method_unit.hpp>
#include <src/csharp/csharp_print_unit.hpp>

namespace codegen::csharp
{

std::shared_ptr<codegen::CodeUnit> CSharpCodeFactory::CreateClass(const std::string& name,
                                                                  codegen::ClassModifier flagsValue) const
{
    return std::make_shared<CSharpClassUnit>(name, flagsValue);
}

std::shared_ptr<codegen::CodeUnit> CSharpCodeFactory::CreateMethod(const std::string& name,
                                                                   const std::string& returnType,
                                                                   codegen::MethodModifier flagsValue) const
{
    return std::make_shared<CSharpMethodUnit>(name, returnType, flagsValue);
}

std::shared_ptr<codegen::CodeUnit> CSharpCodeFactory::CreateField(const std::string& name, const std::string& type,
                                                                  codegen::MethodModifier flagsValue) const
{
    return std::make_shared<CSharpFieldUnit>(name, type, flagsValue);
}

std::shared_ptr<codegen::CodeUnit> CSharpCodeFactory::CreatePrintStatement(const std::string& text) const
{
    return std::make_shared<CSharpPrintUnit>(text);
}

std::string CSharpCodeFactory::GetLanguageName() const
{
    return "C#";
}

}  // namespace codegen::csharp
