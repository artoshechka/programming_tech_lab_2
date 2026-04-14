#include <src/class_unit.hpp>

using codegen::AccessModifier;
using codegen::ClassDeclarationUnit;
using codegen::CodeUnit;
const std::vector<std::string> ClassDeclarationUnit::accessModifiers_ = {"public", "protected", "private"};

ClassDeclarationUnit::ClassDeclarationUnit(const std::string& name) : name_(name)
{
    fields_.resize(accessModifiers_.size());
}

void ClassDeclarationUnit::Append(const std::shared_ptr<CodeUnit>& unit, AccessModifier access_modifier)
{
    size_t modifier_index = static_cast<size_t>(access_modifier);

    if (modifier_index >= accessModifiers_.size())
    {
        modifier_index = static_cast<size_t>(AccessModifier::privateAccess);
    }
    fields_[modifier_index].push_back(unit);
}

void ClassDeclarationUnit::Append(const std::shared_ptr<CodeUnit>& unit, flags flags_value)
{
    Append(unit, static_cast<AccessModifier>(flags_value));
}

std::string ClassDeclarationUnit::Render(unsigned int indent_level) const
{
    std::string result = MakeIndent(indent_level) + "class " + name_ + " {\n";

    for (size_t i = 0; i < accessModifiers_.size(); ++i)
    {
        if (fields_[i].empty())
        {
            continue;
        }
        result += accessModifiers_[i] + ":\n";
        for (const auto& field : fields_[i])
        {
            result += field->Render(indent_level + 1);
        }
        result += "\n";
    }
    result += MakeIndent(indent_level) + "};\n";
    return result;
}
