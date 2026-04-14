/// @file
/// @brief Определение класса для генерации кода метода.
#include <src/cpp/cpp_method_unit.hpp>

using codegen::MethodDeclarationUnit;

MethodDeclarationUnit::MethodDeclarationUnit(const std::string& name, const std::string& returnType, Flags flagsValue)
    : name_(name), returnType_(returnType), flags_(flagsValue)
{
}

void MethodDeclarationUnit::Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue)
{
    (void)flagsValue;
    body_.push_back(unit);
}

std::string MethodDeclarationUnit::Render(unsigned int indentLevel) const
{
    std::string result = MakeIndent(indentLevel);
    if (flags_ & ToFlags(MethodModifier::staticModifier))
    {
        result += "static ";
    } else if (flags_ & ToFlags(MethodModifier::virtualModifier))
    {
        result += "virtual ";
    }

    result += returnType_ + " ";
    result += name_ + "()";
    if (flags_ & ToFlags(MethodModifier::finalModifier))
    {
        result += " final";
    }
    if (flags_ & ToFlags(MethodModifier::constModifier))
    {
        result += " const";
    }

    // Abstract методы в C++ используют = 0 без тела
    if (flags_ & ToFlags(MethodModifier::abstractModifier))
    {
        result += " = 0;\n";
    }
    else
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