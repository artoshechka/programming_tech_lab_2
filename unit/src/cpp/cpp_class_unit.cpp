/// @file
/// @brief Определение класса для генерации кода C++ класса.
#include <src/cpp/cpp_class_unit.hpp>
#include <stdexcept>

using codegen::CppClassUnit;

const std::vector<std::string> CppClassUnit::accessModifiers_ = {"public", "protected", "private"};

namespace
{

size_t ResolveAccessSectionIndex(codegen::AccessModifier accessModifier)
{
    switch (accessModifier)
    {
        case codegen::AccessModifier::PublicAccess:
            return 0;
        case codegen::AccessModifier::ProtectedAccess:
            return 1;
        case codegen::AccessModifier::PrivateAccess:
            return 2;
        default:
            throw std::invalid_argument("Unsupported C++ class access modifier");
    }
}

std::string RenderCppClassModifierSuffix(codegen::CodeUnit::Flags classFlags)
{
    switch (classFlags & codegen::ToFlags(codegen::ClassModifier::FinalModifier))
    {
        case 0:
            return "";
        case codegen::ToFlags(codegen::ClassModifier::FinalModifier):
            return " final";
        default:
            throw std::invalid_argument("Unsupported C++ class modifier");
    }
}

}  // namespace

CppClassUnit::CppClassUnit(const std::string& name, Flags classModifiersValue)
    : codegen::detail::AbstractClassUnit(name, classModifiersValue)
{
    fields_.resize(accessModifiers_.size());
}

void CppClassUnit::Append(const std::shared_ptr<CodeUnit>& unit, AccessModifier accessModifier)
{
    if (!unit)
    {
        throw std::invalid_argument("Class member must not be null");
    }

    const size_t modifierIndex = ResolveAccessSectionIndex(accessModifier);
    fields_[modifierIndex].push_back(unit);
}

void CppClassUnit::Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue)
{
    Append(unit, static_cast<AccessModifier>(flagsValue));
}

std::string CppClassUnit::Render(unsigned int indentLevel) const
{
    std::string result = MakeIndent(indentLevel) + "class " + GetClassName();
    result += RenderCppClassModifierSuffix(GetClassFlags());

    result += " {\n";

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