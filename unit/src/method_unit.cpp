#include <src/method_unit.hpp>

using codegen::MethodUnit;
namespace
{
codegen::Unit::flags to_flags(codegen::MethodModifier modifier)
{
    return static_cast<codegen::Unit::flags>(modifier);
}
}  // namespace

MethodUnit::MethodUnit(const std::string& name, const std::string& return_type, flags flags_value)
    : name_(name), return_type_(return_type), flags_(flags_value)
{
}

void MethodUnit::Add(const std::shared_ptr<Unit>& unit, flags /* flags_value */)
{
    body_.push_back(unit);
}

std::string MethodUnit::Compile(unsigned int level) const
{
    std::string result = GenerateShift(level);
    if (flags_ & to_flags(MethodModifier::staticModifier))
    {
        result += "static ";
    } else if (flags_ & to_flags(MethodModifier::virtualModifier))
    {
        result += "virtual ";
    }
    result += return_type_ + " ";
    result += name_ + "()";
    if (flags_ & to_flags(MethodModifier::constModifier))
    {
        result += " const";
    }
    result += " {\n";
    for (const auto& statement : body_)
    {
        result += statement->Compile(level + 1);
    }
    result += GenerateShift(level) + "}\n";
    return result;
}
