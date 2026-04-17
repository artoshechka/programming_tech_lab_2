/// @file
/// @brief Объявление класса для генерации оператора печати.
#ifndef GUID_2cc37e3f_1e21_43f6_87ca_86320db03ddd
#define GUID_2cc37e3f_1e21_43f6_87ca_86320db03ddd

#include <src/common/abstract_print_unit.hpp>
#include <string>

namespace codegen
{

/// @brief Класс, который формирует оператор печати.
class PrintStatementUnit : public codegen::detail::AbstractPrintUnit
{
   public:
    /// @brief Конструктор генератора инструкции печати на C++.
    /// @param[in] text Текст для вывода на печать.
    explicit PrintStatementUnit(const std::string& text);

   protected:
    /// @brief Формирует C++-инструкцию печати без отступа.
    /// @param[in] text Текст для печати.
    /// @return Выражение печати.
    std::string RenderPrintExpression(const std::string& text) const override;
};

}  // namespace codegen

#endif  // GUID_2cc37e3f_1e21_43f6_87ca_86320db03ddd