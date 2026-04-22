/// @file
/// @brief Реализация генератора C#-класса.
#include <src/common/access_controlled_unit.hpp>
#include <src/csharp/csharp_class_unit.hpp>
#include <stdexcept>
#include <utility>

namespace codegen::csharp
{

namespace
{

std::string ResolveCSharpMemberAccessKeyword(codegen::CodeUnit::Flags flagsValue)
{
    switch (codegen::ToAccessModifierMask(flagsValue))
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
        case codegen::AccessModifier::Unknown:
        default:
            throw std::invalid_argument("Unsupported C# member access modifier");
    }
}

std::string ResolveCSharpClassAccessPrefix(codegen::CodeUnit::Flags flagsValue)
{
    const auto accessMask = codegen::AccessModifier::PublicAccess | codegen::AccessModifier::ProtectedAccess |
                            codegen::AccessModifier::PrivateAccess | codegen::AccessModifier::InternalAccess |
                            codegen::AccessModifier::FileAccess;
    const auto accessFlags = flagsValue & accessMask;

    switch (codegen::ToAccessModifierMask(accessFlags))
    {
        case codegen::AccessModifier::Unknown:
            return "";
        case codegen::AccessModifier::PublicAccess:
            return "public ";
        case codegen::AccessModifier::PrivateAccess:
            return "private ";
        case codegen::AccessModifier::ProtectedAccess:
            return "protected ";
        case codegen::AccessModifier::PrivateProtectedAccess:
            return "private protected ";
        case codegen::AccessModifier::InternalAccess:
            return "internal ";
        case codegen::AccessModifier::ProtectedInternalAccess:
            return "protected internal ";
        case codegen::AccessModifier::FileAccess:
            return "file ";
        default:
            throw std::invalid_argument("Unsupported C# class access modifier");
    }
}

std::string RenderCSharpClassModifiers(codegen::CodeUnit::Flags classFlags)
{
    switch (codegen::ToClassModifierMask(
        classFlags & (codegen::ClassModifier::AbstractModifier | codegen::ClassModifier::FinalModifier)))
    {
        case codegen::ClassModifier::Unknown:
            return "";
        case codegen::ClassModifier::AbstractModifier:
            return "abstract ";
        case codegen::ClassModifier::FinalModifier:
            return "sealed ";  // В C# используется sealed вместо final
        case codegen::ClassModifier::AbstractFinalModifier:
            return "abstract sealed ";
        default:
            throw std::invalid_argument("Unsupported C# class modifier");
    }
}

}  // namespace

CSharpClassUnit::CSharpClassUnit(std::string name, Flags accessFlagsValue)
    : codegen::detail::AbstractClassUnit(std::move(name), accessFlagsValue)
{
}

void CSharpClassUnit::Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue)
{
    if (!unit)
    {
        throw std::invalid_argument("Class member must not be null");
    }

    const std::string accessKeyword = ResolveCSharpMemberAccessKeyword(flagsValue);
    members_.push_back(std::make_shared<codegen::detail::AccessControlledUnit>(accessKeyword, unit));
}

std::string CSharpClassUnit::Render(unsigned int indentLevel) const
{
    std::string result = MakeIndent(indentLevel) + ResolveCSharpClassAccessPrefix(GetClassFlags());
    result += RenderCSharpClassModifiers(GetClassFlags());

    result += "class " + GetClassName() + " {\n";
    for (const auto& member : members_)
    {
        result += member->Render(indentLevel + 1);
    }
    result += MakeIndent(indentLevel) + "}\n";
    return result;
}

}  // namespace codegen::csharp
