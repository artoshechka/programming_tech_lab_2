/// @file
/// @brief Базовый абстрактный класс для языковых реализаций объявления класса.
#ifndef GUID_b3e2c1d4_5aa6_4e81_bce8_3f5e0d846f8a
#define GUID_b3e2c1d4_5aa6_4e81_bce8_3f5e0d846f8a

#include <string>
#include <unit.hpp>

namespace codegen::detail
{

/// @brief Общая база для языковых юнитов объявления класса.
class AbstractClassUnit : public codegen::CodeUnit
{
   public:
    /// @brief Конструктор базового юнита класса.
    /// @param[in] name Имя класса.
    /// @param[in] classFlagsValue Флаги модификаторов класса.
    AbstractClassUnit(std::string name, Flags classFlagsValue);

    /// @brief Виртуальный деструктор.
    ~AbstractClassUnit() override = default;

   protected:
    /// @brief Возвращает имя класса.
    /// @return Имя класса.
    const std::string& GetClassName() const;

    /// @brief Возвращает флаги модификаторов класса.
    /// @return Флаги модификаторов.
    Flags GetClassFlags() const;

   private:
    std::string className_;  ///< Имя класса.
    Flags classFlags_;       ///< Флаги модификаторов класса.
};

}  // namespace codegen::detail

#endif  // GUID_b3e2c1d4_5aa6_4e81_bce8_3f5e0d846f8a
