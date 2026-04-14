#include <src/class_unit.hpp>

using codegen::ClassUnit;
const std::vector<std::string> ClassUnit::accessModifiers_ = {"public", "protected", "private"};

ClassUnit::ClassUnit(const std::string& name) : name_(name)
{
    fields_.resize(accessModifiers_.size());
}

void ClassUnit::Add(const std::shared_ptr<Unit>& unit, AccessModifier access_modifier)
{
    size_t modifier_index = static_cast<size_t>(access_modifier);

    if (modifier_index >= accessModifiers_.size())
    {
        modifier_index = static_cast<size_t>(AccessModifier::privateAccess);
    }
    fields_[modifier_index].push_back(unit);
}

void ClassUnit::Add(const std::shared_ptr<Unit>& unit, flags flags_value)
{
    Add(unit, static_cast<AccessModifier>(flags_value));
}

std::string ClassUnit::Compile(unsigned int level) const
{
    std::string result = GenerateShift(level) + "class " + name_ + " {\n";

    for (size_t i = 0; i < accessModifiers_.size(); ++i)
    {
        if (fields_[i].empty())
        {
            continue;
        }
        result += accessModifiers_[i] + ":\n";
        for (const auto& field : fields_[i])
        {
            result += field->Compile(level + 1);
        }
        result += "\n";
    }
    result += GenerateShift(level) + "};\n";
    return result;
}
