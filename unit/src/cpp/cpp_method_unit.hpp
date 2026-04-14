/// @file
/// @brief Объявление класса для генерации кода метода.
#ifndef GUID_55f5a170_8f2a_4112_842f_c0f6af77ff6e
#define GUID_55f5a170_8f2a_4112_842f_c0f6af77ff6e

#include <codegen_types.hpp>
#include <string>
#include <unit.hpp>
#include <vector>

namespace codegen
{

/// @brief Класс, который формирует объявление метода C++ класса.
class MethodDeclarationUnit : public CodeUnit
{
   public:
    /// @brief Конструктор генератора объявления C++ метода.
    /// @param[in] name Имя метода.
    /// @param[in] returnType Тип возвращаемого значения.
    /// @param[in] flagsValue Флаги модификаторов метода.
    MethodDeclarationUnit(const std::string& name, const std::string& returnType, Flags flagsValue);

    /// @brief Добавляет вложенный элемент в тело метода.
    /// @param[in] unit Вложенный узел-инструкция.
    /// @param[in] flagsValue Дополнительные флаги (не используются).
    void Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue = 0) override;

    /// @brief Формирует текст объявления метода.
    /// @param[in] indentLevel Уровень отступа.
    /// @return Текст объявления метода.
    std::string Render(unsigned int indentLevel = 0) const override;

   private:
    std::string name_;                             ///< Имя метода.
    std::string returnType_;                       ///< Тип возвращаемого значения.
    Flags flags_;                                  ///< Битовая маска модификаторов метода.
    std::vector<std::shared_ptr<CodeUnit>> body_;  ///< Список элементов тела метода.
};

}  // namespace codegen

#endif  // GUID_55f5a170_8f2a_4112_842f_c0f6af77ff6e