/// @file
/// @brief Объявление класса для генерации Java-класса.
#ifndef GUID_b43697fd_9a67_4798_81db_38149df2f4b0
#define GUID_b43697fd_9a67_4798_81db_38149df2f4b0

#include <codegen_types.hpp>
#include <src/common/abstract_class_unit.hpp>
#include <string>
#include <utility>
#include <vector>

namespace codegen::java
{

/// @brief Класс для генерации объявления Java-класса.
class JavaClassUnit : public codegen::detail::AbstractClassUnit
{
   public:
    /// @brief Конструктор генератора объявления Java-класса.
    /// @param[in] name Имя создаваемого Java-класса.
    /// @param[in] classModifiersValue Флаги модификаторов класса (final, abstract и т.д.).
    explicit JavaClassUnit(std::string name, Flags classModifiersValue = 0);

    /// @brief Добавляет элемент класса с указанным модификатором доступа.
    /// @param[in] unit Вложенный узел.
    /// @param[in] flagsValue Флаги доступа.
    void Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue) override;

    /// @brief Формирует текст Java-класса.
    /// @param[in] indentLevel Уровень отступа.
    /// @return Текст объявления класса.
    std::string Render(unsigned int indentLevel) const override;

   private:
    using Member = std::pair<codegen::AccessModifier, std::shared_ptr<codegen::CodeUnit>>;
    std::vector<Member> members_;  ///< Элементы класса с их модификаторами доступа.
};

}  // namespace codegen::java

#endif  // GUID_b43697fd_9a67_4798_81db_38149df2f4b0
