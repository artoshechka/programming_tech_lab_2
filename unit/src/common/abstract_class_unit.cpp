/// @file
/// @brief Реализация базового абстрактного класса для объявления класса.
#include <src/common/abstract_class_unit.hpp>
#include <utility>

namespace codegen::detail
{

AbstractClassUnit::AbstractClassUnit(std::string name, Flags classFlagsValue)
    : className_(std::move(name)), classFlags_(classFlagsValue)
{
}

const std::string& AbstractClassUnit::GetClassName() const
{
    return className_;
}

codegen::CodeUnit::Flags AbstractClassUnit::GetClassFlags() const
{
    return classFlags_;
}

}  // namespace codegen::detail
