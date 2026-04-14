/// @file
/// @brief Реализация генератора Java-класса.
#include <src/common/access_controlled_unit.hpp>
#include <src/java/java_class_unit.hpp>
#include <utility>

namespace codegen::java
{

JavaClassUnit::JavaClassUnit(std::string name, Flags classModifiersValue)
    : name_(std::move(name)), classModifiers_(classModifiersValue)
{
}

void JavaClassUnit::Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue)
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

std::string JavaClassUnit::Render(unsigned int indentLevel) const
{
    std::string result = MakeIndent(indentLevel);

    // Добавляем модификаторы класса
    if (classModifiers_ & codegen::ToFlags(codegen::ClassModifier::abstractModifier))
    {
        result += "abstract ";
    }
    if (classModifiers_ & codegen::ToFlags(codegen::ClassModifier::finalModifier))
    {
        result += "final ";
    }

    result += "class " + name_ + " {\n";
    for (const auto& member : members_)
    {
        result += member->Render(indentLevel + 1);
    }
    result += MakeIndent(indentLevel) + "}\n";
    return result;
}

}  // namespace codegen::java
