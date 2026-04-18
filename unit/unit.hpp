
/// @file
/// @brief Объявление базовых абстракций для генерации кода.
/// @author Artemenko Anton
#ifndef GUID_0f5d2f9d_8b4a_4e8f_96cf_f3e301ab6b21
#define GUID_0f5d2f9d_8b4a_4e8f_96cf_f3e301ab6b21

#include <memory>
#include <string>

namespace codegen
{

/// @brief Базовый класс для элементов генерации кода.
class CodeUnit
{
   public:
    /// @brief Битовая маска флагов для элементов генерации.
    using Flags = unsigned int;

   public:
    /// @brief Виртуальный деструктор базового класса.
    virtual ~CodeUnit() = default;

    /// @brief Добавляет вложенный узел.
    /// @param[in] unit Вложенный узел.
    /// @param[in] flagsValue Дополнительные флаги добавления.

    virtual void Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue);
    /// @brief Формирует текстовое представление узла.
    /// @param[in] indentLevel Уровень отступа.
    /// @return Сгенерированный текст узла.
    virtual std::string Render(unsigned int indentLevel = 0) const = 0;

   protected:
    /// @brief Создает строку отступа для заданного уровня.
    /// @param[in] indentLevel Уровень отступа.
    /// @return Строка пробелов для отступа.
    virtual std::string MakeIndent(unsigned int indentLevel) const;
};

}  // namespace codegen

#endif  // GUID_0f5d2f9d_8b4a_4e8f_96cf_f3e301ab6b21