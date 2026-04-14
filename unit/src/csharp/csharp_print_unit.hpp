/// @file
/// @brief Объявление класса для генерации C# print statement.
#ifndef GUID_c78eb6e2_8970_4f73_8480_7f4db7b3165a
#define GUID_c78eb6e2_8970_4f73_8480_7f4db7b3165a

#include <string>
#include <unit.hpp>

namespace codegen::csharp
{

/// @brief Класс для генерации инструкции вывода на печать на C#.
class CSharpPrintUnit : public codegen::CodeUnit
{
   public:
    /// @brief Конструктор генератора инструкции печати на C#.
    /// @param[in] text Текст для вывода.
    explicit CSharpPrintUnit(std::string text);

    /// @brief Формирует вызов Console.WriteLine().
    /// @param[in] indentLevel Уровень отступа.
    /// @return Текст инструкции печати.
    std::string Render(unsigned int indentLevel) const override;

   private:
    std::string text_;  ///< Текст для печати.
};

}  // namespace codegen::csharp

#endif  // GUID_c78eb6e2_8970_4f73_8480_7f4db7b3165a
