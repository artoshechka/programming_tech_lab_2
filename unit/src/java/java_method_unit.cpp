/// @file
/// @brief Реализация генератора Java-метода.
#include <src/java/java_method_unit.hpp>
#include <utility>

namespace codegen::java
{

JavaMethodUnit::JavaMethodUnit(std::string name, std::string returnType, Flags flagsValue)
    : name_(std::move(name)), returnType_(std::move(returnType)), flags_(flagsValue)
{
}

void JavaMethodUnit::Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue)
{
    (void)flagsValue;
    body_.push_back(unit);
}

std::string JavaMethodUnit::Render(unsigned int indentLevel) const
{
    std::string result = MakeIndent(indentLevel);
    if (flags_ & codegen::ToFlags(codegen::MethodModifier::staticModifier))
    {
        result += "static ";
    }
    if (flags_ & codegen::ToFlags(codegen::MethodModifier::finalModifier))
    {
        result += "final ";
    }
    if (flags_ & codegen::ToFlags(codegen::MethodModifier::abstractModifier))
    {
        result += "abstract ";
    }
    result += returnType_ + " " + name_ + "()";

    // Abstract методы завершаются точкой с запятой без тела
    if (flags_ & codegen::ToFlags(codegen::MethodModifier::abstractModifier))
    {
        result += ";\n";
    } else
    {
        result += " {\n";
        for (const auto& statement : body_)
        {
            result += statement->Render(indentLevel + 1);
        }
        result += MakeIndent(indentLevel) + "}\n";
    }

    return result;
}

}  // namespace codegen::java
