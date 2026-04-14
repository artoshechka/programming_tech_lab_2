/// @file
/// @brief Объявление класса для генерации Java print statement.
#ifndef GUID_2cc13e63_efaa_4a4c_b760_4d72daf66796
#define GUID_2cc13e63_efaa_4a4c_b760_4d72daf66796

#include <string>
#include <unit.hpp>

namespace codegen::java
{

/// @brief Класс для генерации инструкции вывода на печать на Java.
class JavaPrintUnit : public codegen::CodeUnit
{
   public:
    /// @brief Конструктор генератора инструкции печати на Java.
    /// @param[in] text Текст для вывода.
    explicit JavaPrintUnit(std::string text);

    /// @brief Формирует вызов System.out.println().
    /// @param[in] indentLevel Уровень отступа.
    /// @return Текст инструкции печати.
    std::string Render(unsigned int indentLevel) const override;

   private:
    std::string text_;  ///< Текст для печати.
};

}  // namespace codegen::java

#endif  // GUID_2cc13e63_efaa_4a4c_b760_4d72daf66796
