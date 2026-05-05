/// @file
/// @brief Базовый абстрактный класс для языковых реализаций метода.
#ifndef GUID_8b34a2d7_38f8_4df4_8970_4f6f35498268
#define GUID_8b34a2d7_38f8_4df4_8970_4f6f35498268

#include <codegen_types.hpp>
#include <string>
#include <unit.hpp>
#include <vector>

namespace codegen::detail
{

/// @brief Общая база для генераторов метода на разных языках.
class AbstractMethodUnit : public codegen::CodeUnit
{
   public:
    /// @brief Конструктор базового юнита метода.
    /// @param[in] name Имя метода.
    /// @param[in] returnType Тип возвращаемого значения.
    /// @param[in] flagsValue Флаги модификаторов.
    AbstractMethodUnit(std::string name, std::string returnType, MethodModifier flagsValue);

    /// @brief Добавляет оператор в тело метода.
    /// @param[in] unit Вложенный оператор.
    /// @param[in] flagsValue Дополнительные флаги (не используются).
    void Append(const std::shared_ptr<codegen::CodeUnit>& unit, Flags flagsValue) override;

    /// @brief Формирует итоговый код метода.
    /// @param[in] indentLevel Уровень отступа.
    /// @return Текст метода.
    std::string Render(unsigned int indentLevel) const override;

    /// @brief Виртуальный деструктор.
    ~AbstractMethodUnit() override = default;

   protected:
    /// @brief Формирует языковые модификаторы перед типом метода.
    /// @return Текст префикса модификаторов (с завершающим пробелом при необходимости).
    virtual std::string RenderPrefixModifiers() const = 0;

    /// @brief Формирует языковые модификаторы после сигнатуры метода.
    /// @return Суффикс сигнатуры.
    virtual std::string RenderSuffixModifiers() const;

    /// @brief Определяет, является ли метод абстрактным.
    /// @return true, если метод без тела.
    virtual bool IsAbstractMethod() const = 0;

    /// @brief Формирует окончание абстрактного метода.
    /// @return Строка завершения (например, ";\n" или " = 0;\n").
    virtual std::string RenderAbstractTerminator() const = 0;

    /// @brief Возвращает флаги модификаторов метода.
    /// @return Флаги метода.
    MethodModifier GetMethodFlags() const;

   private:
    std::string name_;                                      ///< Имя метода.
    std::string returnType_;                                ///< Тип возвращаемого значения.
    MethodModifier flags_;                                  ///< Флаги модификаторов.
    std::vector<std::shared_ptr<codegen::CodeUnit>> body_;  ///< Тело метода.
};

}  // namespace codegen::detail

#endif  // GUID_8b34a2d7_38f8_4df4_8970_4f6f35498268
