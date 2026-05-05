/// @file
/// @brief Реализация генератора Java-класса.
#include <src/java/java_class_unit.hpp>
#include <stdexcept>
#include <utility>

namespace codegen::java
{

namespace
{

codegen::AccessModifier ResolveJavaClassAccess(codegen::CodeUnit::Flags flagsValue)
{
    switch (codegen::ToAccessModifierMask(flagsValue))
    {
        case codegen::AccessModifier::PublicAccess:
        case codegen::AccessModifier::ProtectedAccess:
        case codegen::AccessModifier::PrivateAccess:
            return codegen::ToAccessModifierMask(flagsValue);
        case codegen::AccessModifier::Unknown:
        default:
            throw std::invalid_argument("Unsupported Java class access modifier");
    }
}

std::string ResolveJavaAccessKeyword(codegen::AccessModifier access)
{
    switch (access)
    {
        case codegen::AccessModifier::PublicAccess:
            return "public";
        case codegen::AccessModifier::ProtectedAccess:
            return "protected";
        case codegen::AccessModifier::PrivateAccess:
            return "private";
        default:
            throw std::invalid_argument("Unsupported Java access modifier");
    }
}

std::string RenderJavaClassModifiers(codegen::ClassModifier classFlags)
{
    switch (codegen::ToClassModifierMask(static_cast<codegen::CodeUnit::Flags>(classFlags)))
    {
        case codegen::ClassModifier::Unknown:
            return "";
        case codegen::ClassModifier::AbstractModifier:
            return "abstract ";
        case codegen::ClassModifier::FinalModifier:
            return "final ";
        case codegen::ClassModifier::AbstractFinalModifier:
            return "abstract final ";
        default:
            throw std::invalid_argument("Unsupported Java class modifier");
    }
}

}  // namespace

JavaClassUnit::JavaClassUnit(std::string name, ClassModifier classModifiersValue)
    : codegen::detail::AbstractClassUnit(std::move(name), classModifiersValue)
{
}

void JavaClassUnit::Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue)
{
    if (!unit)
    {
        throw std::invalid_argument("Class member must not be null");
    }

    members_.emplace_back(ResolveJavaClassAccess(flagsValue), unit);
}

std::string JavaClassUnit::Render(unsigned int indentLevel) const
{
    std::string result = MakeIndent(indentLevel) + RenderJavaClassModifiers(GetClassFlags());

    result += "class " + GetClassName() + " {\n";
    for (const auto& [access, member] : members_)
    {
        const std::string accessKeyword = ResolveJavaAccessKeyword(access);
        std::string rendered = member->Render(indentLevel + 1);
        const std::string indent = MakeIndent(indentLevel + 1);
        if (rendered.rfind(indent, 0) == 0)
        {
            rendered.insert(indent.size(), accessKeyword + " ");
        }
        result += rendered;
    }
    result += MakeIndent(indentLevel) + "}\n";
    return result;
}

}  // namespace codegen::java
