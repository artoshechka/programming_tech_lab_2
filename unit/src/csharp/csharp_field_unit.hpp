/// @file
/// @brief Объявление класса для генерации поля C#.
#ifndef GUID_0546fc14_5180_41ce_be30_34b11de5334a
#define GUID_0546fc14_5180_41ce_be30_34b11de5334a

#include <codegen_types.hpp>
#include <src/common/abstract_field_unit.hpp>
#include <string>

namespace codegen::csharp
{

/// @brief Класс для генерации объявления поля C#.
class CSharpFieldUnit : public codegen::detail::AbstractFieldUnit
{
   public:
    /// @brief Конструктор генератора поля C#.
    /// @param[in] name Имя поля.
    /// @param[in] type Тип поля.
    /// @param[in] flagsValue Флаги модификаторов.
    CSharpFieldUnit(std::string name, std::string type, Flags flagsValue);

   protected:
    /// @brief Формирует префикс модификаторов поля C#.
    /// @return Строка модификаторов.
    std::string RenderPrefixModifiers() const override;
};

}  // namespace codegen::csharp

#endif  // GUID_0546fc14_5180_41ce_be30_34b11de5334a
