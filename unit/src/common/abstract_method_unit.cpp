/// @file
/// @brief Реализация базового абстрактного класса для генерации метода.
#include <src/common/abstract_method_unit.hpp>
#include <stdexcept>
#include <utility>

namespace codegen::detail
{

AbstractMethodUnit::AbstractMethodUnit(std::string name, std::string returnType, MethodModifier flagsValue)
    : name_(std::move(name)), returnType_(std::move(returnType)), flags_(flagsValue)
{
}

void AbstractMethodUnit::Append(const std::shared_ptr<codegen::CodeUnit>& unit, Flags flagsValue)
{
    (void)flagsValue;
    if (!unit)
    {
        throw std::invalid_argument("Method body statement must not be null");
    }
    body_.push_back(unit);
}

std::string AbstractMethodUnit::Render(unsigned int indentLevel) const
{
    if (name_.empty())
    {
        throw std::runtime_error("Method name must not be empty");
    }
    if (returnType_.empty())
    {
        throw std::runtime_error("Method return type must not be empty");
    }

    std::string result = MakeIndent(indentLevel);
    result += RenderPrefixModifiers();
    result += returnType_ + " " + name_ + "()";
    result += RenderSuffixModifiers();

    if (IsAbstractMethod())
    {
        result += RenderAbstractTerminator();
        return result;
    }

    result += " {\n";
    for (const auto& statement : body_)
    {
        if (!statement)
        {
            throw std::runtime_error("Method body contains null statement");
        }
        result += statement->Render(indentLevel + 1);
    }
    result += MakeIndent(indentLevel) + "}\n";
    return result;
}

std::string AbstractMethodUnit::RenderSuffixModifiers() const
{
    return "";
}

codegen::MethodModifier AbstractMethodUnit::GetMethodFlags() const
{
    return flags_;
}

}  // namespace codegen::detail
