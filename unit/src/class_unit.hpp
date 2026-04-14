/// @file
/// @brief Объявление класса для генерации кода C++ класса.
/// @author Artemenko Anton
#ifndef GUID_5b1d3d42_1db0_4fd1_901f_05aa2fd3f31d
#define GUID_5b1d3d42_1db0_4fd1_901f_05aa2fd3f31d

#include <string>
#include <unit.hpp>
#include <vector>

namespace codegen
{

/// @brief Модификатор доступа секции класса.
enum class AccessModifier { publicAccess, protectedAccess, privateAccess };

/// @brief Класс, который собирает и формирует объявление C++ класса.
class ClassDeclarationUnit : public CodeUnit
{
   public:
    /// @brief Конструктор класса генератора объявления C++ класса.
    /// @param[in] name Имя класса.
    explicit ClassDeclarationUnit(const std::string& name);
    /// @brief Добавляет вложенный элемент в указанную секцию доступа.
    /// @param[in] unit Вложенный узел.
    /// @param[in] accessModifier Целевая секция доступа.
    void Append(const std::shared_ptr<CodeUnit>& unit, AccessModifier accessModifier);
    /// @brief Добавляет вложенный элемент с доступом, заданным через флаги.
    /// @param[in] unit Вложенный узел.
    /// @param[in] flagsValue Флаги, интерпретируемые как AccessModifier.
    void Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue) override;
    /// @brief Формирует текст объявления класса.
    /// @param[in] indentLevel Уровень отступа.
    /// @return Текст объявления класса.
    std::string Render(unsigned int indentLevel = 0) const override;

   private:
    static const std::vector<std::string> accessModifiers_;  ///< Текстовые имена модификаторов доступа.
    std::string name_;                                       ///< Имя генерируемого класса.
    /// @brief Контейнер элементов в одной секции доступа.
    using Fields = std::vector<std::shared_ptr<CodeUnit>>;
    std::vector<Fields> fields_;  ///< Набор секций доступа и их элементов.
};

}  // namespace codegen

#endif