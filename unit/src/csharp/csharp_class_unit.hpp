/// @file
/// @brief Объявление класса для генерации C#-класса.
#ifndef GUID_08df7ef8_351b_45a5_bec3_f4bd7307ef66
#define GUID_08df7ef8_351b_45a5_bec3_f4bd7307ef66

#include <codegen_types.hpp>
#include <src/common/abstract_class_unit.hpp>
#include <string>
#include <vector>

namespace codegen::csharp
{

/// @brief Класс для генерации объявления C#-класса.
class CSharpClassUnit : public codegen::detail::AbstractClassUnit
{
   public:
    /// @brief Конструктор генератора объявления C#-класса.
    /// @param[in] name Имя создаваемого класса.
    /// @param[in] accessFlagsValue Флаги модификаторов доступа C#-класса.
    explicit CSharpClassUnit(std::string name, Flags accessFlagsValue = 0);

    /// @brief Добавляет элемент класса с указанным модификатором доступа.
    /// @param[in] unit Вложенный узел.
    /// @param[in] flagsValue Флаги доступа.
    void Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue) override;

    /// @brief Формирует текст C#-класса.
    /// @param[in] indentLevel Уровень отступа.
    /// @return Текст объявления класса.
    std::string Render(unsigned int indentLevel) const override;

   private:
    std::vector<std::shared_ptr<codegen::CodeUnit>> members_;  ///< Элементы (методы, свойства) класса.
};

}  // namespace codegen::csharp

#endif  // GUID_08df7ef8_351b_45a5_bec3_f4bd7307ef66
