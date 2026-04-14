/// @file
/// @brief Реализация генератора C#-класса.
#include <src/common/access_controlled_unit.hpp>
#include <src/csharp/csharp_class_unit.hpp>
#include <utility>

namespace codegen::csharp
{

CSharpClassUnit::CSharpClassUnit(std::string name) : name_(std::move(name))
{
}

void CSharpClassUnit::Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue)
{
    std::string accessKeyword = "private";
    if (flagsValue == static_cast<Flags>(codegen::AccessModifier::publicAccess))
    {
        accessKeyword = "public";
    }
    if (flagsValue == static_cast<Flags>(codegen::AccessModifier::protectedAccess))
    {
        accessKeyword = "protected";
    }
    members_.push_back(std::make_shared<codegen::detail::AccessControlledUnit>(accessKeyword, unit));
}

std::string CSharpClassUnit::Render(unsigned int indentLevel) const
{
    std::string result = MakeIndent(indentLevel) + "class " + name_ + " {\n";
    for (const auto& member : members_)
    {
        result += member->Render(indentLevel + 1);
    }
    result += MakeIndent(indentLevel) + "}\n";
    return result;
}

}  // namespace codegen::csharp
