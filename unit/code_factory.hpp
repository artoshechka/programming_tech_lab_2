/// @file
/// @brief Публичные абстракции фабрики генерации кода.
#ifndef GUID_c1bba86f_6f85_4559_9606_2200f04ac245
#define GUID_c1bba86f_6f85_4559_9606_2200f04ac245

#include <codegen_types.hpp>
#include <memory>
#include <string>
#include <unit.hpp>

namespace codegen
{

/// @brief Поддерживаемые фабрикой языки.
enum class Language : unsigned int {
    Unknown = 0,    ///< Неизвестный язык.
    CppLanguage,    ///< C++ язык программирования.
    JavaLanguage,   ///< Java язык программирования.
    CSharpLanguage  ///< C# язык программирования.
};

/// @brief Интерфейс абстрактной фабрики элементов кода.
/// Определяет контракт для создания элементов генерации кода на различных языках программирования.
class ICodeFactory
{
   public:
    /// @brief Виртуальный деструктор для полиморфного удаления.
    virtual ~ICodeFactory() = default;

    /// @brief Создаёт узел объявления класса.
    /// @param[in] name Имя создаваемого класса.
    /// @param[in] flagsValue Битовая маска флагов модификаторов класса.
    /// @return Указатель на созданный узел объявления класса.
    virtual std::shared_ptr<CodeUnit> CreateClass(const std::string& name, CodeUnit::Flags flagsValue = 0) const = 0;

    /// @brief Создаёт узел объявления метода.
    /// @param[in] name Имя создаваемого метода.
    /// @param[in] returnType Тип возвращаемого значения метода.
    /// @param[in] flagsValue Битовая маска флагов модификаторов метода (static, virtual, const).
    /// @return Указатель на созданный узел объявления метода.
    virtual std::shared_ptr<CodeUnit> CreateMethod(const std::string& name, const std::string& returnType,
                                                   CodeUnit::Flags flagsValue) const = 0;

    /// @brief Создаёт узел объявления поля.
    /// @param[in] name Имя поля.
    /// @param[in] type Тип поля.
    /// @param[in] flagsValue Битовая маска модификаторов поля (static, const/final).
    /// @return Указатель на созданный узел объявления поля.
    virtual std::shared_ptr<CodeUnit> CreateField(const std::string& name, const std::string& type,
                                                  CodeUnit::Flags flagsValue) const = 0;

    /// @brief Создаёт узел инструкции вывода на печать.
    /// @param[in] text Текст для печати.
    /// @return Указатель на созданный узел инструкции печати.
    virtual std::shared_ptr<CodeUnit> CreatePrintStatement(const std::string& text) const = 0;

    /// @brief Возвращает название языка программирования.
    /// @return Строка с названием целевого языка ("C++", "Java", "C#" и т.д.).
    virtual std::string GetLanguageName() const = 0;
};

/// @brief Глобальная фабрика, создающая языковую фабрику по выбранному языку.
/// @param[in] language Выбранный язык программирования (Language::CppLanguage, JavaLanguage, CSharpLanguage).
/// @return Указатель на создаваемую конкретную фабрику для выбранного языка.
/// @throw std::invalid_argument Если передан неизвестный язык.
std::shared_ptr<ICodeFactory> CreateFactory(Language language);

}  // namespace codegen

#endif