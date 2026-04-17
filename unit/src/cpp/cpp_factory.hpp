/// @file
/// @brief Объявления для фабрики генерации кода на C++.
#ifndef GUID_784d13c2_0727_42ed_8a99_a5e1d7987372
#define GUID_784d13c2_0727_42ed_8a99_a5e1d7987372

#include <code_factory.hpp>

namespace codegen::cpp
{

/// @brief Конкретная реализация фабрики для генерации кода на C++.
/// Создаёт узлы синтаксиса C++: классы, методы, операторы печати.
class CppCodeFactory final : public codegen::ICodeFactory
{
   public:
    /// @brief Создаёт узел объявления C++-класса.
    /// @param[in] name Имя создаваемого класса.
    /// @param[in] flagsValue Флаги модификаторов класса (в C++-реализации не используются).
    /// @return Указатель на объект CppClassUnit.
    std::shared_ptr<codegen::CodeUnit> CreateClass(const std::string& name,
                                                   codegen::CodeUnit::Flags flagsValue) const override;

    /// @brief Создаёт узел объявления C++-метода.
    /// @param[in] name Имя создаваемого метода.
    /// @param[in] returnType Тип возвращаемого значения.
    /// @param[in] flagsValue Флаги модификаторов (static, virtual, const).
    /// @return Указатель на объект CppMethodUnit.
    std::shared_ptr<codegen::CodeUnit> CreateMethod(const std::string& name, const std::string& returnType,
                                                    codegen::CodeUnit::Flags flagsValue) const override;

    /// @brief Создаёт узел объявления C++-поля.
    /// @param[in] name Имя поля.
    /// @param[in] type Тип поля.
    /// @param[in] flagsValue Флаги модификаторов (static, const).
    /// @return Указатель на объект CppFieldUnit.
    std::shared_ptr<codegen::CodeUnit> CreateField(const std::string& name, const std::string& type,
                                                   codegen::CodeUnit::Flags flagsValue) const override;

    /// @brief Создаёт узел инструкции printf на C++.
    /// @param[in] text Текст для печати.
    /// @return Указатель на объект CppPrintUnit.
    std::shared_ptr<codegen::CodeUnit> CreatePrintStatement(const std::string& text) const override;

    /// @brief Возвращает название языка программирования.
    /// @return Строка "C++".
    std::string GetLanguageName() const override;
};

}  // namespace codegen::cpp

#endif  // GUID_784d13c2_0727_42ed_8a99_a5e1d7987372
