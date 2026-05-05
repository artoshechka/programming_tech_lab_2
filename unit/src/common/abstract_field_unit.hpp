/// @file
/// @brief Базовый абстрактный класс для языковых реализаций поля класса.
#ifndef GUID_0ef9f91e_57ea_411b_89a7_97f8d2f70c9d
#define GUID_0ef9f91e_57ea_411b_89a7_97f8d2f70c9d

#include <codegen_types.hpp>
#include <string>
#include <unit.hpp>

namespace codegen::detail
{

/// @brief Общая база для генераторов объявления поля.
class AbstractFieldUnit : public codegen::CodeUnit
{
   public:
    /// @brief Конструктор базового юнита поля.
    /// @param[in] name Имя поля.
    /// @param[in] type Тип поля.
    /// @param[in] flagsValue Флаги модификаторов поля.
    AbstractFieldUnit(std::string name, std::string type, MethodModifier flagsValue);

    /// @brief Формирует строку объявления поля.
    /// @param[in] indentLevel Уровень отступа.
    /// @return Текст объявления поля.
    std::string Render(unsigned int indentLevel) const override;

    /// @brief Виртуальный деструктор.
    ~AbstractFieldUnit() override = default;

   protected:
    /// @brief Формирует языковые модификаторы перед объявлением поля.
    /// @return Префикс модификаторов.
    virtual std::string RenderPrefixModifiers() const = 0;

    /// @brief Формирует языковой суффикс до точки с запятой.
    /// @return Суффикс объявления.
    virtual std::string RenderSuffixModifiers() const;

    /// @brief Возвращает флаги модификаторов поля.
    /// @return Флаги модификаторов.
    MethodModifier GetFieldFlags() const;

   private:
    std::string name_;      ///< Имя поля.
    std::string type_;      ///< Тип поля.
    MethodModifier flags_;  ///< Флаги модификаторов поля.
};

}  // namespace codegen::detail

#endif  // GUID_0ef9f91e_57ea_411b_89a7_97f8d2f70c9d
