/// @file
/// @brief Объявление класса для генерации кода C++ класса.
#ifndef GUID_8d9f6710_0cba_4e86_9c9f_32f88d7cff2f
#define GUID_8d9f6710_0cba_4e86_9c9f_32f88d7cff2f

#include <codegen_types.hpp>
#include <src/common/abstract_class_unit.hpp>
#include <string>
#include <vector>

namespace codegen
{

/// @brief Класс, который собирает и формирует объявление C++ класса.
class CppClassUnit : public codegen::detail::AbstractClassUnit
{
   public:
    /// @brief Конструктор генератора объявления C++ класса.
    /// @param[in] name Имя класса.
    /// @param[in] classModifiersValue Флаги модификаторов класса (final и т.д.).
    explicit CppClassUnit(const std::string& name, Flags classModifiersValue = 0);

    /// @brief Добавляет вложенный элемент в указанную секцию доступа.
    /// @param[in] unit Вложенный узел.
    /// @param[in] accessModifier Целевая секция доступа.
    void Append(const std::shared_ptr<CodeUnit>& unit, AccessModifier accessModifier);

    /// @brief Добавляет вложенный элемент с доступом, заданным через флаги.
    /// @param[in] unit Вложенный узел.
    /// @param[in] flagsValue Флаги, интерпретируемые как AccessModifier.
    void Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue) override;

    /// @brief Формирует текст объявления класса.
    /// @param[in] indentLevel Уровень отступа.
    /// @return Текст объявления класса.
    std::string Render(unsigned int indentLevel = 0) const override;

   private:
    static const std::vector<std::string> accessModifiers_;  ///< Имена модификаторов доступа.
    using Fields = std::vector<std::shared_ptr<CodeUnit>>;
    std::vector<Fields> fields_;  ///< Набор секций доступа и их элементов.
};

}  // namespace codegen

#endif  // GUID_8d9f6710_0cba_4e86_9c9f_32f88d7cff2f