/// @file
/// @brief Объявление класса для генерации кода метода.
/// @author Artemenko Anton
#ifndef GUID_6e60ed6b_94e5_41ea_9be8_6d15d770f0a1
#define GUID_6e60ed6b_94e5_41ea_9be8_6d15d770f0a1

#include <string>
#include <unit.hpp>
#include <vector>

namespace codegen
{

/// @brief Модификаторы объявления метода.
enum class MethodModifier : CodeUnit::Flags { staticModifier = 1, constModifier = 1 << 1, virtualModifier = 1 << 2 };

/// @brief Класс, который формирует объявление метода C++ класса.
class MethodDeclarationUnit : public CodeUnit
{
   public:
    /// @brief Конструктор класса генератора объявления метода.
    /// @param[in] name Имя метода.
    /// @param[in] returnType Тип возвращаемого значения.
    /// @param[in] flags_value Флаги модификаторов метода.
    MethodDeclarationUnit(const std::string& name, const std::string& returnType, Flags flagsValue);
    /// @brief Добавляет вложенный элемент в тело метода.
    /// @param[in] unit Вложенный узел-инструкция.
    /// @param[in] flagsValue Дополнительные флаги (не используются).
    void Append(const std::shared_ptr<CodeUnit>& unit, Flags /* flagsValue */ = 0) override;
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

#endif