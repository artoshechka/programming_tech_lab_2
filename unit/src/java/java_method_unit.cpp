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
    result += returnType_ + " " + name_ + "() {\n";
    for (const auto& statement : body_)
    {
        result += statement->Render(indentLevel + 1);
    }
    result += MakeIndent(indentLevel) + "}\n";
    return result;
}

}  // namespace codegen::java
