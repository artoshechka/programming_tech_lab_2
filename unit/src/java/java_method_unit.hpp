/// @file
/// @brief Объявление класса для генерации Java-метода.
#ifndef GUID_f36b12fb_4799_4ec2_8c8d_c664f24efbe9
#define GUID_f36b12fb_4799_4ec2_8c8d_c664f24efbe9

#include <codegen_types.hpp>
#include <string>
#include <unit.hpp>
#include <vector>

namespace codegen::java
{

/// @brief Класс для генерации объявления Java-метода.
class JavaMethodUnit : public codegen::CodeUnit
{
   public:
    /// @brief Конструктор генератора объявления Java-метода.
    /// @param[in] name Имя метода.
    /// @param[in] returnType Тип возвращаемого значения.
    /// @param[in] flagsValue Флаги модификаторов.
    JavaMethodUnit(std::string name, std::string returnType, Flags flagsValue);

    /// @brief Добавляет выражение/инструкцию в тело метода.
    /// @param[in] unit Вложенный узел.
    /// @param[in] flagsValue Дополнительные флаги (не используются).
    void Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue) override;

    /// @brief Формирует текст Java-метода.
    /// @param[in] indentLevel Уровень отступа.
    /// @return Текст объявления метода.
    std::string Render(unsigned int indentLevel) const override;

   private:
    std::string name_;                                      ///< Имя Java-метода.
    std::string returnType_;                                ///< Тип возвращаемого значения.
    Flags flags_;                                           ///< Флаги модификаторов метода.
    std::vector<std::shared_ptr<codegen::CodeUnit>> body_;  ///< Инструкции в теле метода.
};

}  // namespace codegen::java

#endif  // GUID_f36b12fb_4799_4ec2_8c8d_c664f24efbe9
