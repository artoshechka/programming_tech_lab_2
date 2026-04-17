/// @file
/// @brief Объявление класса для генерации поля Java.
#ifndef GUID_7793d047_3e0b_4467_86ca_d9f4af592f9f
#define GUID_7793d047_3e0b_4467_86ca_d9f4af592f9f

#include <codegen_types.hpp>
#include <src/common/abstract_field_unit.hpp>
#include <string>

namespace codegen::java
{

/// @brief Класс для генерации объявления поля Java.
class JavaFieldUnit : public codegen::detail::AbstractFieldUnit
{
   public:
    /// @brief Конструктор генератора поля Java.
    /// @param[in] name Имя поля.
    /// @param[in] type Тип поля.
    /// @param[in] flagsValue Флаги модификаторов.
    JavaFieldUnit(std::string name, std::string type, Flags flagsValue);

   protected:
    /// @brief Формирует префикс модификаторов поля Java.
    /// @return Строка модификаторов.
    std::string RenderPrefixModifiers() const override;
};

}  // namespace codegen::java

#endif  // GUID_7793d047_3e0b_4467_86ca_d9f4af592f9f
