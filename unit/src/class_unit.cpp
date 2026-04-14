/// @file
/// @brief Определение класса для генерации кода C++ класса.
/// @author Artemenko Anton
#include <src/class_unit.hpp>

using codegen::ClassDeclarationUnit;

const std::vector<std::string> ClassDeclarationUnit::accessModifiers_ = {"public", "protected", "private"};

ClassDeclarationUnit::ClassDeclarationUnit(const std::string& name) : name_(name)
{
    fields_.resize(accessModifiers_.size());
}

void ClassDeclarationUnit::Append(const std::shared_ptr<CodeUnit>& unit, AccessModifier access_modifier)
{
    size_t modifierIndex = static_cast<size_t>(access_modifier);

    if (modifierIndex >= accessModifiers_.size())
    {
        modifierIndex = static_cast<size_t>(AccessModifier::privateAccess);
    }
    fields_[modifierIndex].push_back(unit);
}

void ClassDeclarationUnit::Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue)
{
    Append(unit, static_cast<AccessModifier>(flagsValue));
}

std::string ClassDeclarationUnit::Render(unsigned int indentLevel) const
{
    std::string result = MakeIndent(indentLevel) + "class " + name_ + " {\n";

    for (size_t i = 0; i < accessModifiers_.size(); ++i)
    {
        if (fields_[i].empty())
        {
            continue;
        }
        result += accessModifiers_[i] + ":\n";
        for (const auto& field : fields_[i])
        {
            result += field->Render(indentLevel + 1);
        }
        result += "\n";
    }
    result += MakeIndent(indentLevel) + "};\n";
    return result;
}
