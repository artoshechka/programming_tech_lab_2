/// @file
/// @brief Вспомогательный узел для префиксирования вывода модификатором доступа.
#ifndef GUID_d5b1820e_5f9f_4a7e_85ec_1d12f5bf1fb4
#define GUID_d5b1820e_5f9f_4a7e_85ec_1d12f5bf1fb4

#include <memory>
#include <string>
#include <unit.hpp>

namespace codegen::detail
{

/// @brief Обертка над CodeUnit, которая добавляет ключевое слово доступа в начало строки.
class AccessControlledUnit : public codegen::CodeUnit
{
   public:
    /// @brief Конструктор обертки доступа.
    /// @param[in] accessKeyword Ключевое слово доступа.
    /// @param[in] unit Оборачиваемый узел.
    AccessControlledUnit(std::string accessKeyword, std::shared_ptr<codegen::CodeUnit> unit);

    /// @brief Возвращает вывод обернутого узла с префиксом модификатора доступа.
    /// @param[in] indentLevel Уровень отступа.
    /// @return Строка с добавленным ключевым словом доступа.
    std::string Render(unsigned int indentLevel) const override;

   private:
    std::string accessKeyword_;                ///< Ключевое слово доступа для префиксирования.
    std::shared_ptr<codegen::CodeUnit> unit_;  ///< Обернутый узел кода.
};

}  // namespace codegen::detail

#endif  // GUID_d5b1820e_5f9f_4a7e_85ec_1d12f5bf1fb4
