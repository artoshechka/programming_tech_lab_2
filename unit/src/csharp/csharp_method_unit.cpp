/// @file
/// @brief Реализация генератора C#-метода.
#include <src/csharp/csharp_method_unit.hpp>
#include <utility>

namespace codegen::csharp
{

CSharpMethodUnit::CSharpMethodUnit(std::string name, std::string returnType, Flags flagsValue)
    : name_(std::move(name)), returnType_(std::move(returnType)), flags_(flagsValue)
{
}

void CSharpMethodUnit::Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue)
{
    (void)flagsValue;
    body_.push_back(unit);
}

std::string CSharpMethodUnit::Render(unsigned int indentLevel) const
{
    std::string result = MakeIndent(indentLevel);
    if (flags_ & codegen::ToFlags(codegen::MethodModifier::staticModifier))
    {
        result += "static ";
    } else if (flags_ & codegen::ToFlags(codegen::MethodModifier::virtualModifier))
    {
        result += "virtual ";
    }
    if (flags_ & codegen::ToFlags(codegen::MethodModifier::abstractModifier))
    {
        result += "abstract ";
    }
    if (flags_ & codegen::ToFlags(codegen::MethodModifier::finalModifier))
    {
        result += "sealed ";  // В C# используется sealed вместо final
    }
    result += returnType_ + " " + name_ + "()";
    
    // Abstract методы завершаются точкой с запятой без тела
    if (flags_ & codegen::ToFlags(codegen::MethodModifier::abstractModifier))
    {
        result += ";\n";
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

}  // namespace codegen::csharp
