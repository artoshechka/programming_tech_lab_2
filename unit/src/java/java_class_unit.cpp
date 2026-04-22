/// @file
/// @brief Реализация генератора Java-класса.
#include <src/common/access_controlled_unit.hpp>
#include <src/java/java_class_unit.hpp>
#include <stdexcept>
#include <utility>

namespace codegen::java
{

namespace
{

std::string ResolveJavaClassAccessKeyword(codegen::CodeUnit::Flags flagsValue)
{
    switch (static_cast<codegen::AccessModifier>(flagsValue))
    {
        case codegen::AccessModifier::PublicAccess:
            return "public";
        case codegen::AccessModifier::ProtectedAccess:
            return "protected";
        case codegen::AccessModifier::PrivateAccess:
            return "private";
        case codegen::AccessModifier::Unknown:
        default:
            throw std::invalid_argument("Unsupported Java class access modifier");
    }
}

std::string RenderJavaClassModifiers(codegen::CodeUnit::Flags classFlags)
{
    switch (classFlags)
    {
        case 0:
            return "";
        case codegen::ToFlags(codegen::ClassModifier::AbstractModifier):
            return "abstract ";
        case codegen::ToFlags(codegen::ClassModifier::FinalModifier):
            return "final ";
        case codegen::ToFlags(codegen::ClassModifier::AbstractModifier) |
             codegen::ToFlags(codegen::ClassModifier::FinalModifier):
            return "abstract final ";
        default:
            throw std::invalid_argument("Unsupported Java class modifier");
    }
}

}  // namespace

JavaClassUnit::JavaClassUnit(std::string name, Flags classModifiersValue)
    : codegen::detail::AbstractClassUnit(std::move(name), classModifiersValue)
{
}

void JavaClassUnit::Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue)
{
    if (!unit)
    {
        throw std::invalid_argument("Class member must not be null");
    }

    const std::string accessKeyword = ResolveJavaClassAccessKeyword(flagsValue);
    members_.push_back(std::make_shared<codegen::detail::AccessControlledUnit>(accessKeyword, unit));
}

std::string JavaClassUnit::Render(unsigned int indentLevel) const
{
    std::string result = MakeIndent(indentLevel) + RenderJavaClassModifiers(GetClassFlags());

    result += "class " + GetClassName() + " {\n";
    for (const auto& member : members_)
    {
        result += member->Render(indentLevel + 1);
    }
    result += MakeIndent(indentLevel) + "}\n";
    return result;
}

}  // namespace codegen::java
