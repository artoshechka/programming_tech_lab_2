/// @file
/// @brief Базовый абстрактный класс для языковых реализаций оператора печати.
#ifndef GUID_428e9211_70a4_45fd_86ce_c60a1e2e0a0f
#define GUID_428e9211_70a4_45fd_86ce_c60a1e2e0a0f

#include <string>
#include <unit.hpp>

namespace codegen::detail
{

/// @brief Общая база для генераторов инструкции печати.
class AbstractPrintUnit : public codegen::CodeUnit
{
   public:
    /// @brief Конструктор базового юнита печати.
    /// @param[in] text Текст для вывода.
    explicit AbstractPrintUnit(std::string text);

    /// @brief Формирует строку инструкции печати.
    /// @param[in] indentLevel Уровень отступа.
    /// @return Текст инструкции вывода.
    std::string Render(unsigned int indentLevel) const override;

    /// @brief Виртуальный деструктор.
    ~AbstractPrintUnit() override = default;

   protected:
    /// @brief Формирует языковую инструкцию печати без отступа и перевода строки.
    /// @param[in] text Текст для вывода.
    /// @return Языковая инструкция печати.
    virtual std::string RenderPrintExpression(const std::string& text) const = 0;

    /// @brief Возвращает сохраненный текст печати.
    /// @return Текст печати.
    const std::string& GetPrintText() const;

   private:
    std::string text_;  ///< Текст для вывода.
};

}  // namespace codegen::detail

#endif  // GUID_428e9211_70a4_45fd_86ce_c60a1e2e0a0f
