/// @file
/// @brief Объявление класса для генерации Java print statement.
#ifndef GUID_2cc13e63_efaa_4a4c_b760_4d72daf66796
#define GUID_2cc13e63_efaa_4a4c_b760_4d72daf66796

#include <src/common/abstract_print_unit.hpp>
#include <string>

namespace codegen::java
{

/// @brief Класс для генерации инструкции вывода на печать на Java.
class JavaPrintUnit : public codegen::detail::AbstractPrintUnit
{
   public:
    /// @brief Конструктор генератора инструкции печати на Java.
    /// @param[in] text Текст для вывода.
    explicit JavaPrintUnit(std::string text);

   protected:
    /// @brief Формирует Java-инструкцию печати без отступа.
    /// @param[in] text Текст для печати.
    /// @return Выражение печати.
    std::string RenderPrintExpression(const std::string& text) const override;
};

}  // namespace codegen::java

#endif  // GUID_2cc13e63_efaa_4a4c_b760_4d72daf66796
