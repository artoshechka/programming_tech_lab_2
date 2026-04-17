/// @file
/// @brief Объявление класса для генерации поля C++.
#ifndef GUID_17258fdb_5682_468b_97e6_1f07aedf8a2e
#define GUID_17258fdb_5682_468b_97e6_1f07aedf8a2e

#include <codegen_types.hpp>
#include <src/common/abstract_field_unit.hpp>
#include <string>

namespace codegen
{

/// @brief Класс для генерации объявления поля C++.
class FieldDeclarationUnit : public codegen::detail::AbstractFieldUnit
{
   public:
    /// @brief Конструктор генератора поля C++.
    /// @param[in] name Имя поля.
    /// @param[in] type Тип поля.
    /// @param[in] flagsValue Флаги модификаторов.
    FieldDeclarationUnit(const std::string& name, const std::string& type, Flags flagsValue);

   protected:
    /// @brief Формирует префикс модификаторов поля C++.
    /// @return Строка модификаторов.
    std::string RenderPrefixModifiers() const override;
};

}  // namespace codegen

#endif  // GUID_17258fdb_5682_468b_97e6_1f07aedf8a2e
