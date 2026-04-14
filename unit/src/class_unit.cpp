#include <unit/src/class_unit.hpp>

const std::vector<std::string> ClassUnit::access_modifiers_ = {"public", "protected", "private"};

ClassUnit::ClassUnit(const std::string &name) : name_(name)
{
    fields_.resize(access_modifiers_.size());
}

void ClassUnit::Add(const std::shared_ptr<Unit> &unit, flags flags_value)
{
    int modifier = privateAccess;

    if (flags_value < access_modifiers_.size())
    {
        modifier = flags_value;
    }
    fields_[modifier].push_back(unit);
}

std::string ClassUnit::Compile(unsigned int level) const
{
    std::string result = GenerateShift(level) + "class " + name_ + " {\n";

    for (size_t i = 0; i < access_modifiers_.size(); ++i)
    {
        if (fields_[i].empty())
        {
            continue;
        }
        result += access_modifiers_[i] + ":\n";
        for (const auto &field : fields_[i])
        {
            result += field->Compile(level + 1);
        }
        result += "\n";
    }
    result += GenerateShift(level) + "};\n";
    return result;
}
