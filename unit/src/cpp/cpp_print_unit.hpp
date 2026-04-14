/// @file
/// @brief Объявление класса для генерации оператора печати.
#ifndef GUID_2cc37e3f_1e21_43f6_87ca_86320db03ddd
#define GUID_2cc37e3f_1e21_43f6_87ca_86320db03ddd

#include <string>
#include <unit.hpp>

namespace codegen
{

/// @brief Класс, который формирует оператор печати.
class PrintStatementUnit : public CodeUnit
{
   public:
    /// @brief Конструктор генератора инструкции печати на C++.
    /// @param[in] text Текст для вывода на печать.
    explicit PrintStatementUnit(const std::string& text);

    /// @brief Формирует текст инструкции печати.
    /// @param[in] indentLevel Уровень отступа.
    /// @return Текст инструкции печати.
    std::string Render(unsigned int indentLevel = 0) const override;

   private:
    std::string text_;  ///< Печатаемый текст.
};

}  // namespace codegen

#endif  // GUID_2cc37e3f_1e21_43f6_87ca_86320db03ddd