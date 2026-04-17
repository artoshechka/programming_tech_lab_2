/// @file
/// @brief Объявления фабрики генерации кода на C#.
#ifndef GUID_7bdbb715_3ab5_4024_80b1_16348f070228
#define GUID_7bdbb715_3ab5_4024_80b1_16348f070228

#include <code_factory.hpp>

namespace codegen::csharp
{

class CSharpCodeFactory final : public codegen::ICodeFactory
{
   public:
    /// @brief Создает узел объявления C#-класса.
    /// @param[in] name Имя класса.
    /// @param[in] flagsValue Флаги модификаторов класса.
    /// @return Указатель на узел класса.
    std::shared_ptr<codegen::CodeUnit> CreateClass(const std::string& name,
                                                   codegen::CodeUnit::Flags flagsValue) const override;

    /// @brief Создает узел объявления C#-метода.
    /// @param[in] name Имя метода.
    /// @param[in] returnType Тип возвращаемого значения.
    /// @param[in] flagsValue Флаги модификаторов.
    /// @return Указатель на узел метода.
    std::shared_ptr<codegen::CodeUnit> CreateMethod(const std::string& name, const std::string& returnType,
                                                    codegen::CodeUnit::Flags flagsValue) const override;

    /// @brief Создает узел объявления поля C#.
    /// @param[in] name Имя поля.
    /// @param[in] type Тип поля.
    /// @param[in] flagsValue Флаги модификаторов (static, readonly).
    /// @return Указатель на узел поля.
    std::shared_ptr<codegen::CodeUnit> CreateField(const std::string& name, const std::string& type,
                                                   codegen::CodeUnit::Flags flagsValue) const override;

    /// @brief Создает инструкцию печати C#.
    /// @param[in] text Текст для вывода.
    /// @return Указатель на узел печати.
    std::shared_ptr<codegen::CodeUnit> CreatePrintStatement(const std::string& text) const override;

    /// @brief Возвращает название языка.
    /// @return Строка "C#".
    std::string GetLanguageName() const override;
};

}  // namespace codegen::csharp

#endif  // GUID_7bdbb715_3ab5_4024_80b1_16348f070228
