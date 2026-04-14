#include <src/method_unit.hpp>

using codegen::MethodDeclarationUnit;
namespace
{
codegen::CodeUnit::flags ToFlags(codegen::MethodModifier modifier)
{
    return static_cast<codegen::CodeUnit::flags>(modifier);
}
}  // namespace

MethodDeclarationUnit::MethodDeclarationUnit(const std::string& name, const std::string& return_type, flags flags_value)
    : name_(name), return_type_(return_type), flags_(flags_value)
{
}

void MethodDeclarationUnit::Append(const std::shared_ptr<CodeUnit>& unit, flags /* flags_value */)
{
    body_.push_back(unit);
}

std::string MethodDeclarationUnit::Render(unsigned int indent_level) const
{
    std::string result = MakeIndent(indent_level);
    if (flags_ & ToFlags(MethodModifier::staticModifier))
    {
        result += "static ";
    } else if (flags_ & ToFlags(MethodModifier::virtualModifier))
    {
        result += "virtual ";
    }
    result += return_type_ + " ";
    result += name_ + "()";
    if (flags_ & ToFlags(MethodModifier::constModifier))
    {
        result += " const";
    }
    result += " {\n";
    for (const auto& statement : body_)
    {
        result += statement->Render(indent_level + 1);
    }
    result += MakeIndent(indent_level) + "}\n";
    return result;
}
