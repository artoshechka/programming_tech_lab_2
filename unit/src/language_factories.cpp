/// @file
/// @brief Определение скрытых реализаций фабрик генерации кода для разных языков.
#include <code_factory.hpp>
#include <src/class_unit.hpp>
#include <src/method_unit.hpp>
#include <src/print_operator_unit.hpp>
#include <vector>

namespace
{

/// @brief Вспомогательный класс для добавления модификаторов доступа перед генерируемым кодом.
/// Обёртывает другой узел и префиксирует его вывод ключевым словом доступа (public, protected, private).
class AccessControlledUnit : public codegen::CodeUnit
{
   public:
    /// @brief Конструктор вспомогательного модуля контроля доступа.
    /// @param[in] accessKeyword Ключевое слово доступа ("public", "protected", "private").
    /// @param[in] unit Узел, к которому нужно добавить модификатор доступа.
    AccessControlledUnit(std::string accessKeyword, std::shared_ptr<codegen::CodeUnit> unit)
        : accessKeyword_(std::move(accessKeyword)), unit_(std::move(unit))
    {
    }

    std::string Render(unsigned int indentLevel) const override
    {
        std::string rendered = unit_->Render(indentLevel);
        const std::string indent = MakeIndent(indentLevel);

        if (rendered.rfind(indent, 0) == 0)
        {
            rendered.insert(indent.size(), accessKeyword_ + " ");
        }
        return rendered;
    }

   private:
    std::string accessKeyword_;                ///< Ключевое слово доступа для префиксирования.
    std::shared_ptr<codegen::CodeUnit> unit_;  ///< Обёрнутый узел кода.
};

/// @brief Класс для генерации объявления Java-класса.
/// Поддерживает секции доступа (public, protected, private) и формирует синтаксически корректное объявление класса на
/// Java.
class JavaClassUnit : public codegen::CodeUnit
{
   public:
    /// @brief Конструктор генератора объявления Java-класса.
    /// @param[in] name Имя создаваемого Java-класса.
    explicit JavaClassUnit(std::string name) : name_(std::move(name))
    {
    }

    void Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue) override
    {
        std::string accessKeyword = "private";
        if (flagsValue == static_cast<Flags>(codegen::AccessModifier::publicAccess))
        {
            accessKeyword = "public";
        }
        if (flagsValue == static_cast<Flags>(codegen::AccessModifier::protectedAccess))
        {
            accessKeyword = "protected";
        }
        members_.push_back(std::make_shared<AccessControlledUnit>(accessKeyword, unit));
    }

    std::string Render(unsigned int indentLevel) const override
    {
        std::string result = MakeIndent(indentLevel) + "class " + name_ + " {\n";
        for (const auto& member : members_)
        {
            result += member->Render(indentLevel + 1);
        }
        result += MakeIndent(indentLevel) + "}\n";
        return result;
    }

   private:
    std::string name_;                                         ///< Имя Java-класса.
    std::vector<std::shared_ptr<codegen::CodeUnit>> members_;  ///< Элементы (методы, поля) класса.
};

/// @brief Класс для генерации объявления Java-метода.
/// Поддерживает модификаторы static и формирует синтаксически корректное объявление методов на Java.
class JavaMethodUnit : public codegen::CodeUnit
{
   public:
    /// @brief Конструктор генератора объявления Java-метода.
    /// @param[in] name Имя создаваемого Java-метода.
    /// @param[in] returnType Тип возвращаемого значения.
    /// @param[in] flagsValue Битовая маска флагов модификаторов (static).
    JavaMethodUnit(std::string name, std::string returnType, Flags flagsValue)
        : name_(std::move(name)), returnType_(std::move(returnType)), flags_(flagsValue)
    {
    }

    void Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue) override
    {
        (void)flagsValue;
        body_.push_back(unit);
    }

    std::string Render(unsigned int indentLevel) const override
    {
        std::string result = MakeIndent(indentLevel);
        if (flags_ & codegen::ToFlags(codegen::MethodModifier::staticModifier))
        {
            result += "static ";
        }
        result += returnType_ + " " + name_ + "() {\n";
        for (const auto& statement : body_)
        {
            result += statement->Render(indentLevel + 1);
        }
        result += MakeIndent(indentLevel) + "}\n";
        return result;
    }

   private:
    std::string name_;                                      ///< Имя Java-метода.
    std::string returnType_;                                ///< Тип возвращаемого значения.
    Flags flags_;                                           ///< Флаги модификаторов метода.
    std::vector<std::shared_ptr<codegen::CodeUnit>> body_;  ///< Инструкции в теле метода.
};

/// @brief Класс для генерации инструкции вывода на печать на Java.
/// Использует System.out.println() для вывода текста.
class JavaPrintUnit : public codegen::CodeUnit
{
   public:
    /// @brief Конструктор генератора инструкции печати на Java.
    /// @param[in] text Текст для вывода на печать.
    explicit JavaPrintUnit(std::string text) : text_(std::move(text))
    {
    }

    std::string Render(unsigned int indentLevel) const override
    {
        return MakeIndent(indentLevel) + "System.out.println(\"" + text_ + "\");\n";
    }

   private:
    std::string text_;  ///< Текст для печати.
};

/// @brief Класс для генерации объявления C#-класса.
/// Поддерживает секции доступа (public, protected, private) и формирует синтаксически корректное объявление класса на
/// C#.
class CSharpClassUnit : public codegen::CodeUnit
{
   public:
    /// @brief Конструктор генератора объявления C#-класса.
    /// @param[in] name Имя создаваемого C#-класса.
    explicit CSharpClassUnit(std::string name) : name_(std::move(name))
    {
    }

    void Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue) override
    {
        std::string accessKeyword = "private";
        if (flagsValue == static_cast<Flags>(codegen::AccessModifier::publicAccess))
        {
            accessKeyword = "public";
        }
        if (flagsValue == static_cast<Flags>(codegen::AccessModifier::protectedAccess))
        {
            accessKeyword = "protected";
        }
        members_.push_back(std::make_shared<AccessControlledUnit>(accessKeyword, unit));
    }

    std::string Render(unsigned int indentLevel) const override
    {
        std::string result = MakeIndent(indentLevel) + "class " + name_ + " {\n";
        for (const auto& member : members_)
        {
            result += member->Render(indentLevel + 1);
        }
        result += MakeIndent(indentLevel) + "}\n";
        return result;
    }

   private:
    std::string name_;                                         ///< Имя C#-класса.
    std::vector<std::shared_ptr<codegen::CodeUnit>> members_;  ///< Элементы (методы, свойства) класса.
};

/// @brief Класс для генерации объявления C#-метода.
/// Поддерживает модификаторы static, virtual и формирует синтаксически корректное объявление методов на C#.
class CSharpMethodUnit : public codegen::CodeUnit
{
   public:
    /// @brief Конструктор генератора объявления C#-метода.
    /// @param[in] name Имя создаваемого C#-метода.
    /// @param[in] returnType Тип возвращаемого значения.
    /// @param[in] flagsValue Битовая маска флагов модификаторов (static, virtual).
    CSharpMethodUnit(std::string name, std::string returnType, Flags flagsValue)
        : name_(std::move(name)), returnType_(std::move(returnType)), flags_(flagsValue)
    {
    }

    void Append(const std::shared_ptr<CodeUnit>& unit, Flags flagsValue) override
    {
        (void)flagsValue;
        body_.push_back(unit);
    }

    std::string Render(unsigned int indentLevel) const override
    {
        std::string result = MakeIndent(indentLevel);
        if (flags_ & codegen::ToFlags(codegen::MethodModifier::staticModifier))
        {
            result += "static ";
        } else if (flags_ & codegen::ToFlags(codegen::MethodModifier::virtualModifier))
        {
            result += "virtual ";
        }
        result += returnType_ + " " + name_ + "() {\n";
        for (const auto& statement : body_)
        {
            result += statement->Render(indentLevel + 1);
        }
        result += MakeIndent(indentLevel) + "}\n";
        return result;
    }

   private:
    std::string name_;                                      ///< Имя C#-метода.
    std::string returnType_;                                ///< Тип возвращаемого значения.
    Flags flags_;                                           ///< Флаги модификаторов метода.
    std::vector<std::shared_ptr<codegen::CodeUnit>> body_;  ///< Инструкции в теле метода.
};

/// @brief Класс для генерации инструкции вывода на печать на C#.
/// Использует Console.WriteLine() для вывода текста.
class CSharpPrintUnit : public codegen::CodeUnit
{
   public:
    /// @brief Конструктор генератора инструкции печати на C#.
    /// @param[in] text Текст для вывода на печать.
    explicit CSharpPrintUnit(std::string text) : text_(std::move(text))
    {
    }

    std::string Render(unsigned int indentLevel) const override
    {
        return MakeIndent(indentLevel) + "Console.WriteLine(\"" + text_ + "\");\n";
    }

   private:
    std::string text_;  ///< Текст для печати.
};

/// @brief Конкретная реализация фабрики для генерации кода на C++.
/// Скрытая реализация, доступна только через публичный интерфейс ICodeFactory и глобальную функцию CreateFactory().
class CppCodeFactory final : public codegen::ICodeFactory
{
   public:
    /// @brief Создаёт узел объявления C++-класса.
    /// @param[in] name Имя создаваемого класса.
    /// @return Указатель на объект ClassDeclarationUnit.
    std::shared_ptr<codegen::CodeUnit> CreateClass(const std::string& name) const override
    {
        return std::make_shared<codegen::ClassDeclarationUnit>(name);
    }

    /// @brief Создаёт узел объявления C++-метода.
    /// @param[in] name Имя создаваемого метода.
    /// @param[in] returnType Тип возвращаемого значения.
    /// @param[in] flagsValue Флаги модификаторов (static, virtual, const).
    /// @return Указатель на объект MethodDeclarationUnit.
    std::shared_ptr<codegen::CodeUnit> CreateMethod(const std::string& name, const std::string& returnType,
                                                    codegen::CodeUnit::Flags flagsValue) const override
    {
        return std::make_shared<codegen::MethodDeclarationUnit>(name, returnType, flagsValue);
    }

    /// @brief Создаёт узел инструкции printf на C++.
    /// @param[in] text Текст для печати.
    /// @return Указатель на объект PrintStatementUnit.
    std::shared_ptr<codegen::CodeUnit> CreatePrintStatement(const std::string& text) const override
    {
        return std::make_shared<codegen::PrintStatementUnit>(text);
    }

    /// @brief Возвращает название языка программирования.
    /// @return Строка "C++".
    std::string GetLanguageName() const override
    {
        return "C++";
    }
};

/// @brief Конкретная реализация фабрики для генерации кода на Java.
/// Скрытая реализация, доступна только через публичный интерфейс ICodeFactory и глобальную функцию CreateFactory().
class JavaCodeFactory final : public codegen::ICodeFactory
{
   public:
    /// @brief Создаёт узел объявления Java-класса.
    /// @param[in] name Имя создаваемого класса.
    /// @return Указатель на объект JavaClassUnit.
    std::shared_ptr<codegen::CodeUnit> CreateClass(const std::string& name) const override
    {
        return std::make_shared<JavaClassUnit>(name);
    }

    /// @brief Создаёт узел объявления Java-метода.
    /// @param[in] name Имя создаваемого метода.
    /// @param[in] returnType Тип возвращаемого значения.
    /// @param[in] flagsValue Флаги модификаторов (static).
    /// @return Указатель на объект JavaMethodUnit.
    std::shared_ptr<codegen::CodeUnit> CreateMethod(const std::string& name, const std::string& returnType,
                                                    codegen::CodeUnit::Flags flagsValue) const override
    {
        return std::make_shared<JavaMethodUnit>(name, returnType, flagsValue);
    }

    /// @brief Создаёт узел инструкции System.out.println() на Java.
    /// @param[in] text Текст для печати.
    /// @return Указатель на объект JavaPrintUnit.
    std::shared_ptr<codegen::CodeUnit> CreatePrintStatement(const std::string& text) const override
    {
        return std::make_shared<JavaPrintUnit>(text);
    }

    /// @brief Возвращает название языка программирования.
    /// @return Строка "Java".
    std::string GetLanguageName() const override
    {
        return "Java";
    }
};

/// @brief Конкретная реализация фабрики для генерации кода на C#.
/// Скрытая реализация, доступна только через публичный интерфейс ICodeFactory и глобальную функцию CreateFactory().
class CSharpCodeFactory final : public codegen::ICodeFactory
{
   public:
    /// @brief Создаёт узел объявления C#-класса.
    /// @param[in] name Имя создаваемого класса.
    /// @return Указатель на объект CSharpClassUnit.
    std::shared_ptr<codegen::CodeUnit> CreateClass(const std::string& name) const override
    {
        return std::make_shared<CSharpClassUnit>(name);
    }

    /// @brief Создаёт узел объявления C#-метода.
    /// @param[in] name Имя создаваемого метода.
    /// @param[in] returnType Тип возвращаемого значения.
    /// @param[in] flagsValue Флаги модификаторов (static, virtual).
    /// @return Указатель на объект CSharpMethodUnit.
    std::shared_ptr<codegen::CodeUnit> CreateMethod(const std::string& name, const std::string& returnType,
                                                    codegen::CodeUnit::Flags flagsValue) const override
    {
        return std::make_shared<CSharpMethodUnit>(name, returnType, flagsValue);
    }

    /// @brief Создаёт узел инструкции Console.WriteLine() на C#.
    /// @param[in] text Текст для печати.
    /// @return Указатель на объект CSharpPrintUnit.
    std::shared_ptr<codegen::CodeUnit> CreatePrintStatement(const std::string& text) const override
    {
        return std::make_shared<CSharpPrintUnit>(text);
    }

    /// @brief Возвращает название языка программирования.
    /// @return Строка "C#".
    std::string GetLanguageName() const override
    {
        return "C#";
    }
};

}  // namespace

namespace codegen
{

/// @brief Фабрика для создания языковой фабрики по выбранному языку.
/// Является точкой входа для получения конкретной реализации ICodeFactory.
/// @param[in] language Целевой язык программирования.
/// @return Указатель на создаваемую фабрику для выбранного языка.
/// @note По умолчанию возвращает C++-фабрику при некорректном значении параметра.
std::shared_ptr<ICodeFactory> CreateFactory(Language language)
{
    switch (language)
    {
        case Language::cppLanguage:
            return std::make_shared<CppCodeFactory>();
        case Language::javaLanguage:
            return std::make_shared<JavaCodeFactory>();
        case Language::csharpLanguage:
            return std::make_shared<CSharpCodeFactory>();
        default:
            return std::make_shared<CppCodeFactory>();
    }
}

}  // namespace codegen
