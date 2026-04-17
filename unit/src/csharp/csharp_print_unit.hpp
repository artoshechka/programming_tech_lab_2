/// @file
/// @brief Объявление класса для генерации C# print statement.
#ifndef GUID_c78eb6e2_8970_4f73_8480_7f4db7b3165a
#define GUID_c78eb6e2_8970_4f73_8480_7f4db7b3165a

#include <src/common/abstract_print_unit.hpp>
#include <string>

namespace codegen::csharp
{

/// @brief Класс для генерации инструкции вывода на печать на C#.
class CSharpPrintUnit : public codegen::detail::AbstractPrintUnit
{
   public:
    /// @brief Конструктор генератора инструкции печати на C#.
    /// @param[in] text Текст для вывода.
    explicit CSharpPrintUnit(std::string text);

   protected:
    /// @brief Формирует C#-инструкцию печати без отступа.
    /// @param[in] text Текст для печати.
    /// @return Выражение печати.
    std::string RenderPrintExpression(const std::string& text) const override;
};

}  // namespace codegen::csharp

#endif  // GUID_c78eb6e2_8970_4f73_8480_7f4db7b3165a
