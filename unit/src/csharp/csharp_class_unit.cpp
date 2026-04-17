/// @file
/// @brief Реализация генератора C#-класса.
#include <src/common/access_controlled_unit.hpp>
#include <src/csharp/csharp_class_unit.hpp>
#include <utility>

namespace codegen::csharp
{

namespace
{

std::string ResolveCSharpMemberAccessKeyword(codegen::CodeUnit::Flags flagsValue)
{
    switch (static_cast<codegen::AccessModifier>(flagsValue))
    {
        case codegen::AccessModifier::publicAccess:
            return "public";
        case codegen::AccessModifier::protectedAccess:
            return "protected";
        case codegen::AccessModifier::privateProtectedAccess:
            return "private protected";
        case codegen::AccessModifier::internalAccess:
            return "internal";
        case codegen::AccessModifier::protectedInternalAccess:
            return "protected internal";
        case codegen::AccessModifier::privateAccess:
        default:
            return "private";
    }
}

std::string ResolveCSharpClassAccessPrefix(codegen::CodeUnit::Flags flagsValue)
{
    switch (static_cast<codegen::AccessModifier>(flagsValue))
    {
        case codegen::AccessModifier::publicAccess:
            return "public ";
        case codegen::AccessModifier::privateAccess:
            return "private ";
        case codegen::AccessModifier::protectedAccess:
            return "protected ";
        case codegen::AccessModifier::privateProtectedAccess:
            return "private protected ";
        case codegen::AccessModifier::internalAccess:
            return "internal ";
        case codegen::AccessModifier::protectedInternalAccess:
            return "protected internal ";
        case codegen::AccessModifier::fileAccess:
            return "file ";
        default:
            return "";
    }
}

}  // namespace

CSharpClassUnit::CSharpClassUnit(std::string name, Flags accessFlagsValue)
    : codegen::detail::AbstractClassUnit(std::move(name), accessFlagsValue)
{
}

void CSharpClassUnit::Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue)
{
    const std::string accessKeyword = ResolveCSharpMemberAccessKeyword(flagsValue);
    members_.push_back(std::make_shared<codegen::detail::AccessControlledUnit>(accessKeyword, unit));
}

std::string CSharpClassUnit::Render(unsigned int indentLevel) const
{
    std::string result = MakeIndent(indentLevel) + ResolveCSharpClassAccessPrefix(GetClassFlags());

    // Добавляем модификаторы класса
    if (GetClassFlags() & codegen::ToFlags(codegen::ClassModifier::abstractModifier))
    {
        result += "abstract ";
    }
    if (GetClassFlags() & codegen::ToFlags(codegen::ClassModifier::finalModifier))
    {
        result += "sealed ";  // В C# используется sealed вместо final
    }

    result += "class " + GetClassName() + " {\n";
    for (const auto& member : members_)
    {
        result += member->Render(indentLevel + 1);
    }
    result += MakeIndent(indentLevel) + "}\n";
    return result;
}

}  // namespace codegen::csharp
