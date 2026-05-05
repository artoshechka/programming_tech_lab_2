/// @file
/// @brief Объявление класса для генерации кода метода.
#ifndef GUID_55f5a170_8f2a_4112_842f_c0f6af77ff6e
#define GUID_55f5a170_8f2a_4112_842f_c0f6af77ff6e

#include <codegen_types.hpp>
#include <src/common/abstract_method_unit.hpp>
#include <string>

namespace codegen
{

/// @brief Класс, который формирует объявление метода C++ класса.
class CppMethodUnit : public codegen::detail::AbstractMethodUnit
{
   public:
    /// @brief Конструктор генератора объявления C++ метода.
    /// @param[in] name Имя метода.
    /// @param[in] returnType Тип возвращаемого значения.
    /// @param[in] flagsValue Флаги модификаторов метода.
    CppMethodUnit(const std::string& name, const std::string& returnType, MethodModifier flagsValue);

   protected:
    /// @brief Формирует модификаторы C++ перед типом метода.
    /// @return Префикс модификаторов.
    std::string RenderPrefixModifiers() const override;

    /// @brief Формирует модификаторы C++ после сигнатуры метода.
    /// @return Суффикс модификаторов.
    std::string RenderSuffixModifiers() const override;

    /// @brief Проверяет, является ли метод абстрактным (pure virtual).
    /// @return true, если метод abstract.
    bool IsAbstractMethod() const override;

    /// @brief Возвращает окончание для pure virtual метода.
    /// @return Строка " = 0;\n".
    std::string RenderAbstractTerminator() const override;
};

}  // namespace codegen

#endif  // GUID_55f5a170_8f2a_4112_842f_c0f6af77ff6e