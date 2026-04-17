/// @file
/// @brief Объявление класса для генерации C#-метода.
#ifndef GUID_95f18fd5_30c3_4295_8698_60bf56494d12
#define GUID_95f18fd5_30c3_4295_8698_60bf56494d12

#include <codegen_types.hpp>
#include <src/common/abstract_method_unit.hpp>
#include <string>

namespace codegen::csharp
{

/// @brief Класс для генерации объявления C#-метода.
class CSharpMethodUnit : public codegen::detail::AbstractMethodUnit
{
   public:
    /// @brief Конструктор генератора объявления C#-метода.
    /// @param[in] name Имя метода.
    /// @param[in] returnType Тип возвращаемого значения.
    /// @param[in] flagsValue Флаги модификаторов.
    CSharpMethodUnit(std::string name, std::string returnType, Flags flagsValue);

   protected:
    /// @brief Формирует модификаторы C# перед типом метода.
    /// @return Префикс модификаторов.
    std::string RenderPrefixModifiers() const override;

    /// @brief Проверяет, является ли C#-метод абстрактным.
    /// @return true, если метод abstract.
    bool IsAbstractMethod() const override;

    /// @brief Возвращает окончание для abstract C#-метода.
    /// @return Строка ";\n".
    std::string RenderAbstractTerminator() const override;
};

}  // namespace codegen::csharp

#endif  // GUID_95f18fd5_30c3_4295_8698_60bf56494d12
