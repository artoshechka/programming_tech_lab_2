/// @file
/// @brief Реализация генератора C#-класса.
#include <src/csharp/csharp_class_unit.hpp>
#include <stdexcept>
#include <string>
#include <utility>

namespace codegen::csharp
{

namespace
{

codegen::AccessModifier ResolveCSharpMemberAccess(codegen::CodeUnit::Flags flagsValue)
{
    switch (codegen::ToAccessModifierMask(flagsValue))
    {
        case codegen::AccessModifier::PublicAccess:
        case codegen::AccessModifier::ProtectedAccess:
        case codegen::AccessModifier::PrivateAccess:
        case codegen::AccessModifier::PrivateProtectedAccess:
        case codegen::AccessModifier::InternalAccess:
        case codegen::AccessModifier::ProtectedInternalAccess:
            return codegen::ToAccessModifierMask(flagsValue);
        case codegen::AccessModifier::Unknown:
        default:
            throw std::invalid_argument("Unsupported C# member access modifier: " + std::to_string(flagsValue));
    }
}

std::string ResolveCSharpMemberAccessKeyword(codegen::AccessModifier access)
{
    switch (access)
    {
        case codegen::AccessModifier::PublicAccess:
            return "public";
        case codegen::AccessModifier::ProtectedAccess:
            return "protected";
        case codegen::AccessModifier::PrivateProtectedAccess:
            return "private protected";
        case codegen::AccessModifier::InternalAccess:
            return "internal";
        case codegen::AccessModifier::ProtectedInternalAccess:
            return "protected internal";
        case codegen::AccessModifier::PrivateAccess:
            return "private";
        default:
            throw std::invalid_argument("Unsupported C# member access modifier: " +
                                        std::to_string(static_cast<unsigned int>(access)));
    }
}

std::string RenderCSharpClassModifiers(codegen::ClassModifier classFlags)
{
    switch (codegen::ToClassModifierMask(static_cast<codegen::CodeUnit::Flags>(classFlags)))
    {
        case codegen::ClassModifier::Unknown:
            return "";
        case codegen::ClassModifier::AbstractModifier:
            return "abstract ";
        case codegen::ClassModifier::FinalModifier:
            return "sealed ";
        case codegen::ClassModifier::AbstractFinalModifier:
            return "abstract sealed ";
        default:
            throw std::invalid_argument("Unsupported C# class modifier: " +
                                        std::to_string(static_cast<unsigned int>(classFlags)));
    }
}

}  // namespace

CSharpClassUnit::CSharpClassUnit(std::string name, ClassModifier classModifiersValue)
    : codegen::detail::AbstractClassUnit(std::move(name), classModifiersValue)
{
}

void CSharpClassUnit::Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue)
{
    if (!unit)
    {
        throw std::invalid_argument("Class member must not be null");
    }

    members_.emplace_back(ResolveCSharpMemberAccess(flagsValue), unit);
}

std::string CSharpClassUnit::Render(unsigned int indentLevel) const
{
    std::string result = MakeIndent(indentLevel) + RenderCSharpClassModifiers(GetClassFlags());

    result += "class " + GetClassName() + " {\n";
    for (const auto& [access, member] : members_)
    {
        const std::string accessKeyword = ResolveCSharpMemberAccessKeyword(access);
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

}  // namespace codegen::csharp
