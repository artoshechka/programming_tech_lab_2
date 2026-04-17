/// @file
/// @brief Объявления фабрики генерации кода на Java.
#ifndef GUID_4be8dce5_66f5_4ca8_90b2_63f5eaa8ece2
#define GUID_4be8dce5_66f5_4ca8_90b2_63f5eaa8ece2

#include <code_factory.hpp>

namespace codegen::java
{

class JavaCodeFactory final : public codegen::ICodeFactory
{
   public:
    /// @brief Создает узел объявления Java-класса.
    /// @param[in] name Имя класса.
    /// @param[in] flagsValue Флаги модификаторов класса (final, abstract и т.д.).
    /// @return Указатель на узел класса.
    std::shared_ptr<codegen::CodeUnit> CreateClass(const std::string& name,
                                                   codegen::CodeUnit::Flags flagsValue) const override;

    /// @brief Создает узел объявления Java-метода.
    /// @param[in] name Имя метода.
    /// @param[in] returnType Тип возвращаемого значения.
    /// @param[in] flagsValue Флаги модификаторов.
    /// @return Указатель на узел метода.
    std::shared_ptr<codegen::CodeUnit> CreateMethod(const std::string& name, const std::string& returnType,
                                                    codegen::CodeUnit::Flags flagsValue) const override;

    /// @brief Создает узел объявления поля Java.
    /// @param[in] name Имя поля.
    /// @param[in] type Тип поля.
    /// @param[in] flagsValue Флаги модификаторов (static, final).
    /// @return Указатель на узел поля.
    std::shared_ptr<codegen::CodeUnit> CreateField(const std::string& name, const std::string& type,
                                                   codegen::CodeUnit::Flags flagsValue) const override;

    /// @brief Создает инструкцию печати Java.
    /// @param[in] text Текст для вывода.
    /// @return Указатель на узел печати.
    std::shared_ptr<codegen::CodeUnit> CreatePrintStatement(const std::string& text) const override;

    /// @brief Возвращает название языка.
    /// @return Строка "Java".
    std::string GetLanguageName() const override;
};

}  // namespace codegen::java

#endif  // GUID_4be8dce5_66f5_4ca8_90b2_63f5eaa8ece2
