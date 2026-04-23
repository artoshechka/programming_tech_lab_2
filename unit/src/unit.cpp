/// @file
/// @brief Определение базовых абстракций для генерации кода.
/// @author Artemenko Anton
#include <codegen_types.hpp>
#include <stdexcept>
#include <unit.hpp>

using codegen::CodeUnit;

/// @brief Базовая реализация добавления элемента, которая сообщает об отсутствии поддержки.
/// @param[in] unit Вложенный элемент.
/// @param[in] flagsValue Флаги добавления.
void CodeUnit::Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue)
{
    (void)unit;
    (void)flagsValue;
    throw std::runtime_error("Not supported");
}

void CodeUnit::Append(const std::shared_ptr<CodeUnit>& unit, AccessModifier accessModifier)
{
    Append(unit, accessModifier | AccessModifier::Unknown);
}

/// @brief Создает строку отступа по заданному уровню.
/// @param[in] indentLevel Уровень отступа.
/// @return Строка из пробелов для отступа.
std::string CodeUnit::MakeIndent(unsigned int indentLevel) const
{
    static const auto DEFAULT_SHIFT = " ";
    std::string result;
    for (unsigned int i = 0; i < indentLevel; ++i)
    {
        result += DEFAULT_SHIFT;
    }
    return result;
}
