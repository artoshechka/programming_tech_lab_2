/// @file
/// @brief Реализация базового абстрактного класса для объявления класса.
#include <src/common/abstract_class_unit.hpp>
#include <stdexcept>
#include <utility>

namespace codegen::detail
{

void AbstractClassUnit::Append(const std::shared_ptr<CodeUnit>& unit, AccessModifier accessModifier)
{
    Append(unit, accessModifier | AccessModifier::Unknown);
}

AbstractClassUnit::AbstractClassUnit(std::string name, Flags classFlagsValue)
    : className_(std::move(name)), classFlags_(classFlagsValue)
{
}

const std::string& AbstractClassUnit::GetClassName() const
{
    if (className_.empty())
    {
        throw std::runtime_error("Class name must not be empty");
    }
    return className_;
}

codegen::CodeUnit::Flags AbstractClassUnit::GetClassFlags() const
{
    return classFlags_;
}

}  // namespace codegen::detail
