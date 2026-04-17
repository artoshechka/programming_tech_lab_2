/// @file
/// @brief Объявление класса для генерации Java-метода.
#ifndef GUID_f36b12fb_4799_4ec2_8c8d_c664f24efbe9
#define GUID_f36b12fb_4799_4ec2_8c8d_c664f24efbe9

#include <codegen_types.hpp>
#include <src/common/abstract_method_unit.hpp>
#include <string>

namespace codegen::java
{

/// @brief Класс для генерации объявления Java-метода.
class JavaMethodUnit : public codegen::detail::AbstractMethodUnit
{
   public:
    /// @brief Конструктор генератора объявления Java-метода.
    /// @param[in] name Имя метода.
    /// @param[in] returnType Тип возвращаемого значения.
    /// @param[in] flagsValue Флаги модификаторов.
    JavaMethodUnit(std::string name, std::string returnType, Flags flagsValue);

   protected:
    /// @brief Формирует модификаторы Java перед типом метода.
    /// @return Префикс модификаторов.
    std::string RenderPrefixModifiers() const override;

    /// @brief Проверяет, является ли Java-метод абстрактным.
    /// @return true, если метод abstract.
    bool IsAbstractMethod() const override;

    /// @brief Возвращает окончание для abstract Java-метода.
    /// @return Строка ";\n".
    std::string RenderAbstractTerminator() const override;
};

}  // namespace codegen::java

#endif  // GUID_f36b12fb_4799_4ec2_8c8d_c664f24efbe9
