/// @file
/// @brief Объявление класса для генерации C#-метода.
#ifndef GUID_95f18fd5_30c3_4295_8698_60bf56494d12
#define GUID_95f18fd5_30c3_4295_8698_60bf56494d12

#include <codegen_types.hpp>
#include <string>
#include <unit.hpp>
#include <vector>

namespace codegen::csharp
{

/// @brief Класс для генерации объявления C#-метода.
class CSharpMethodUnit : public codegen::CodeUnit
{
   public:
    /// @brief Конструктор генератора объявления C#-метода.
    /// @param[in] name Имя метода.
    /// @param[in] returnType Тип возвращаемого значения.
    /// @param[in] flagsValue Флаги модификаторов.
    CSharpMethodUnit(std::string name, std::string returnType, Flags flagsValue);

    /// @brief Добавляет выражение/инструкцию в тело метода.
    /// @param[in] unit Вложенный узел.
    /// @param[in] flagsValue Дополнительные флаги (не используются).
    void Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue) override;

    /// @brief Формирует текст C#-метода.
    /// @param[in] indentLevel Уровень отступа.
    /// @return Текст объявления метода.
    std::string Render(unsigned int indentLevel) const override;

   private:
    std::string name_;                                      ///< Имя C#-метода.
    std::string returnType_;                                ///< Тип возвращаемого значения.
    Flags flags_;                                           ///< Флаги модификаторов метода.
    std::vector<std::shared_ptr<codegen::CodeUnit>> body_;  ///< Инструкции в теле метода.
};

}  // namespace codegen::csharp

#endif  // GUID_95f18fd5_30c3_4295_8698_60bf56494d12
