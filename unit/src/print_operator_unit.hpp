/// @file
/// @brief Объявление класса для генерации оператора печати.
/// @author Artemenko Anton
#ifndef GUID_8f9f34c0_9d5f_4874_bf16_e50baf617204
#define GUID_8f9f34c0_9d5f_4874_bf16_e50baf617204

#include <string>
#include <unit.hpp>

namespace codegen
{

/// @brief Класс, который формирует оператор печати.
class PrintStatementUnit : public CodeUnit
{
   public:
    /// @brief Конструктор класса генератора оператора печати.
    /// @param[in] text Текст для печати.
    explicit PrintStatementUnit(const std::string& text);
    /// @brief Формирует текст инструкции печати.
    /// @param[in] indentLevel Уровень отступа.
    /// @return Текст инструкции печати.
    std::string Render(unsigned int indentLevel = 0) const override;

   private:
    std::string text_;  ///< Печатаемый текст.
};

}  // namespace codegen

#endif