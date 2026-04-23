#include <gtest/gtest.h>

#include <array>
#include <code_factory.hpp>
#include <codegen_types.hpp>
#include <memory>
#include <src/cpp/cpp_class_unit.hpp>
#include <src/common/abstract_print_unit.hpp>
#include <src/common/access_controlled_unit.hpp>
#include <stdexcept>
#include <string>
#include <utility>

namespace
{

codegen::CodeUnit::Flags ToAccessFlags(codegen::AccessModifier access)
{
    return access | codegen::AccessModifier::Unknown;
}

bool Contains(const std::string& source, const std::string& fragment)
{
    return source.find(fragment) != std::string::npos;
}

class IndentedLineUnit final : public codegen::CodeUnit
{
   public:
    std::string Render(unsigned int indentLevel) const override
    {
        return MakeIndent(indentLevel) + "payload;\n";
    }
};

class RawLineUnit final : public codegen::CodeUnit
{
   public:
    std::string Render(unsigned int) const override
    {
        return "payload;\n";
    }
};

class ProbePrintUnit final : public codegen::detail::AbstractPrintUnit
{
   public:
    explicit ProbePrintUnit(std::string text) : codegen::detail::AbstractPrintUnit(std::move(text))
    {
    }

    std::string ReadStoredText() const
    {
        return GetPrintText();
    }

   protected:
    std::string RenderPrintExpression(const std::string& text) const override
    {
        return "print(" + text + ")";
    }
};

class ProbeClassUnit final : public codegen::detail::AbstractClassUnit
{
   public:
    ProbeClassUnit(std::string name, Flags flagsValue) : codegen::detail::AbstractClassUnit(std::move(name), flagsValue)
    {
    }

    Flags ReadFlags() const
    {
        return GetClassFlags();
    }

    std::string Render(unsigned int indentLevel = 0) const override
    {
        return MakeIndent(indentLevel) + GetClassName();
    }
};

}  // namespace

// Кейс: CreateFactory возвращает корректные имена для всех поддерживаемых языков.
TEST(CreateFactoryTest, ReturnsExpectedLanguageName)
{
    const auto cppFactory = codegen::CreateFactory(codegen::Language::CppLanguage);
    const auto javaFactory = codegen::CreateFactory(codegen::Language::JavaLanguage);
    const auto csharpFactory = codegen::CreateFactory(codegen::Language::CSharpLanguage);

    ASSERT_NE(cppFactory, nullptr);
    ASSERT_NE(javaFactory, nullptr);
    ASSERT_NE(csharpFactory, nullptr);

    EXPECT_EQ(cppFactory->GetLanguageName(), "C++");
    EXPECT_EQ(javaFactory->GetLanguageName(), "Java");
    EXPECT_EQ(csharpFactory->GetLanguageName(), "C#");
}

// Кейс: CreateFactory выбрасывает исключение при неизвестном значении enum языка.
TEST(CreateFactoryTest, UnknownLanguageThrows)
{
    const auto invalidLanguage = static_cast<codegen::Language>(999);
    EXPECT_THROW(codegen::CreateFactory(invalidLanguage), std::invalid_argument);
}

// Кейс: Побитовые helper-функции и маски модификаторов возвращают ожидаемые значения.
TEST(CodegenTypesTest, BitHelpersPreserveMasksAndFlags)
{
    const auto accessFlags =
        codegen::AccessModifier::ProtectedAccess | codegen::AccessModifier::InternalAccess;
    EXPECT_EQ(codegen::ToAccessModifierMask(accessFlags), codegen::AccessModifier::ProtectedInternalAccess);
    EXPECT_EQ(accessFlags & codegen::AccessModifier::ProtectedAccess,
              static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::ProtectedAccess));
    EXPECT_TRUE(codegen::HasFlag(accessFlags, codegen::AccessModifier::ProtectedAccess));

    const auto methodFlags = codegen::MethodModifier::StaticModifier | codegen::MethodModifier::FinalModifier;
    EXPECT_EQ(codegen::ToMethodModifierMask(methodFlags), codegen::MethodModifier::StaticFinalModifier);
    EXPECT_EQ(methodFlags & codegen::MethodModifier::FinalModifier,
              static_cast<codegen::CodeUnit::Flags>(codegen::MethodModifier::FinalModifier));
    EXPECT_TRUE(codegen::HasFlag(methodFlags, codegen::MethodModifier::StaticModifier));

    const auto classFlags = codegen::ClassModifier::AbstractModifier | codegen::ClassModifier::FinalModifier;
    EXPECT_EQ(codegen::ToClassModifierMask(classFlags), codegen::ClassModifier::AbstractFinalModifier);
    EXPECT_EQ(classFlags & codegen::ClassModifier::AbstractModifier,
              static_cast<codegen::CodeUnit::Flags>(codegen::ClassModifier::AbstractModifier));
    EXPECT_TRUE(codegen::HasFlag(classFlags, codegen::ClassModifier::FinalModifier));
}

// Кейс: Листовые юниты отклоняют Append() и выбрасывают ожидаемый runtime_error.
TEST(BaseCodeUnitTest, AppendOnLeafFieldThrowsNotSupported)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CppLanguage);
    const auto field = factory->CreateField("value_", "int", 0);

    EXPECT_THROW(field->Append(factory->CreatePrintStatement("x"), 0), std::runtime_error);
}

// Кейс: Базовая перегрузка Append(accessModifier) на листовом юните также выбрасывает Not supported.
TEST(BaseCodeUnitTest, AppendOnLeafFieldWithAccessModifierThrowsNotSupported)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CppLanguage);
    const auto field = factory->CreateField("value_", "int", 0);

    EXPECT_THROW(field->Append(factory->CreatePrintStatement("x"), codegen::AccessModifier::PublicAccess),
                 std::runtime_error);
}

// Кейс: Класс с пустым именем создается, но ошибка фиксируется при Render().
TEST(ExceptionHandlingTest, CreateClassTracksEmptyNameOnRender)
{
    const std::array<codegen::Language, 3> languages = {
        codegen::Language::CppLanguage,
        codegen::Language::JavaLanguage,
        codegen::Language::CSharpLanguage,
    };

    for (const auto language : languages)
    {
        const auto factory = codegen::CreateFactory(language);
        ASSERT_NE(factory, nullptr);

        std::shared_ptr<codegen::CodeUnit> classUnit;
        EXPECT_NO_THROW(classUnit = factory->CreateClass("", 0));
        ASSERT_NE(classUnit, nullptr);
        EXPECT_THROW(classUnit->Render(), std::runtime_error);
    }
}

// Кейс: Метод с пустым именем/типом создается, но ошибка фиксируется при Render().
TEST(ExceptionHandlingTest, CreateMethodTracksEmptyNameOrTypeOnRender)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CppLanguage);

    ASSERT_NE(factory, nullptr);

    std::shared_ptr<codegen::CodeUnit> emptyNameMethod;
    EXPECT_NO_THROW(emptyNameMethod = factory->CreateMethod("", "void", 0));
    ASSERT_NE(emptyNameMethod, nullptr);
    EXPECT_THROW(emptyNameMethod->Render(), std::runtime_error);

    std::shared_ptr<codegen::CodeUnit> emptyTypeMethod;
    EXPECT_NO_THROW(emptyTypeMethod = factory->CreateMethod("Run", "", 0));
    ASSERT_NE(emptyTypeMethod, nullptr);
    EXPECT_THROW(emptyTypeMethod->Render(), std::runtime_error);
}

// Кейс: Поле с пустым именем/типом создается, но ошибка фиксируется при Render().
TEST(ExceptionHandlingTest, CreateFieldTracksEmptyNameOrTypeOnRender)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CppLanguage);

    ASSERT_NE(factory, nullptr);

    std::shared_ptr<codegen::CodeUnit> emptyNameField;
    EXPECT_NO_THROW(emptyNameField = factory->CreateField("", "int", 0));
    ASSERT_NE(emptyNameField, nullptr);
    EXPECT_THROW(emptyNameField->Render(), std::runtime_error);

    std::shared_ptr<codegen::CodeUnit> emptyTypeField;
    EXPECT_NO_THROW(emptyTypeField = factory->CreateField("value_", "", 0));
    ASSERT_NE(emptyTypeField, nullptr);
    EXPECT_THROW(emptyTypeField->Render(), std::runtime_error);
}

// Кейс: Print-юнит с пустым текстом создается, но ошибка фиксируется при Render().
TEST(ExceptionHandlingTest, CreatePrintTracksEmptyTextOnRender)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CppLanguage);

    ASSERT_NE(factory, nullptr);

    std::shared_ptr<codegen::CodeUnit> printUnit;
    EXPECT_NO_THROW(printUnit = factory->CreatePrintStatement(""));
    ASSERT_NE(printUnit, nullptr);
    EXPECT_THROW(printUnit->Render(), std::runtime_error);
}

// Кейс: Append(nullptr) в класс выбрасывает invalid_argument для всех языков.
TEST(ExceptionHandlingTest, ClassAppendThrowsForNullMember)
{
    const std::array<codegen::Language, 3> languages = {
        codegen::Language::CppLanguage,
        codegen::Language::JavaLanguage,
        codegen::Language::CSharpLanguage,
    };

    for (const auto language : languages)
    {
        const auto factory = codegen::CreateFactory(language);
        ASSERT_NE(factory, nullptr);

        const auto classUnit = factory->CreateClass("Container", 0);
        ASSERT_NE(classUnit, nullptr);
        EXPECT_THROW(classUnit->Append(nullptr, 0), std::invalid_argument);
    }
}

// Кейс: Append(nullptr) в тело метода выбрасывает invalid_argument.
TEST(ExceptionHandlingTest, MethodAppendThrowsForNullStatement)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CppLanguage);
    ASSERT_NE(factory, nullptr);

    const auto method = factory->CreateMethod("Run", "void", 0);
    ASSERT_NE(method, nullptr);
    EXPECT_THROW(method->Append(nullptr, 0), std::invalid_argument);
}

// Кейс: C++ перегрузка Append(accessModifier) проверяет null до преобразования модификатора.
TEST(ExceptionHandlingTest, CppClassAppendAccessOverloadThrowsForNullMember)
{
    auto classUnit = std::static_pointer_cast<codegen::CppClassUnit>(
        codegen::CreateFactory(codegen::Language::CppLanguage)->CreateClass("Container", 0));

    ASSERT_NE(classUnit, nullptr);
    EXPECT_THROW(classUnit->Append(nullptr, codegen::AccessModifier::PublicAccess), std::invalid_argument);
}

// Кейс: AccessControlledUnit создается всегда, но некорректные аргументы выявляются при Render().
TEST(ExceptionHandlingTest, AccessControlledUnitThrowsForInvalidArguments)
{
    const codegen::detail::AccessControlledUnit emptyKeywordUnit("", std::make_shared<RawLineUnit>());
    EXPECT_THROW(emptyKeywordUnit.Render(0), std::runtime_error);

    const codegen::detail::AccessControlledUnit nullWrappedUnit("public", std::shared_ptr<codegen::CodeUnit>());
    EXPECT_THROW(nullWrappedUnit.Render(0), std::runtime_error);
}

// Кейс: Базовый класс объявления класса отдает сохраненные флаги и имя без ошибок.
TEST(AbstractClassUnitTest, ExposesStoredFlagsAndName)
{
    const ProbeClassUnit unit("Named", static_cast<codegen::CodeUnit::Flags>(codegen::ClassModifier::FinalModifier));

    EXPECT_EQ(unit.ReadFlags(), static_cast<codegen::CodeUnit::Flags>(codegen::ClassModifier::FinalModifier));
    EXPECT_EQ(unit.Render(2), "  Named");
}

// Кейс: Абстрактный print-юнит хранит текст и рендерит его через реализацию потомка.
TEST(AbstractPrintUnitTest, ExposesStoredTextAndRenderExpression)
{
    const ProbePrintUnit unit("payload");

    EXPECT_EQ(unit.ReadStoredText(), "payload");
    EXPECT_EQ(unit.Render(2), "  print(payload)\n");
}

// Кейс: Обертка доступа добавляет ключевое слово, когда строка начинается с нужного отступа.
TEST(AccessControlledUnitTest, AddsKeywordWhenRenderedLineStartsWithIndent)
{
    const auto wrapped = codegen::detail::AccessControlledUnit("public", std::make_shared<IndentedLineUnit>());

    EXPECT_EQ(wrapped.Render(2), "  public payload;\n");
}

// Кейс: Обертка доступа не изменяет строку, если префикс отступа отсутствует.
TEST(AccessControlledUnitTest, KeepsRenderedLineWithoutIndentPrefix)
{
    const auto wrapped = codegen::detail::AccessControlledUnit("public", std::make_shared<RawLineUnit>());

    EXPECT_EQ(wrapped.Render(3), "payload;\n");
}

// Кейс: C++ класс выбрасывает исключение для неподдерживаемого модификатора доступа.
TEST(CppRenderTest, ClassThrowsForUnsupportedAccess)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CppLanguage);
    const auto classUnit = factory->CreateClass("FallbackDemo", 0);

    EXPECT_THROW(
        classUnit->Append(factory->CreateField("value_", "int", 0), ToAccessFlags(codegen::AccessModifier::FileAccess)),
        std::invalid_argument);
}

// Кейс: C++ метод отдает приоритет static над virtual и рендерит суффиксы final/const.
TEST(CppRenderTest, MethodPrefersStaticOverVirtualAndRendersSuffixes)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CppLanguage);
    const auto methodFlags = codegen::MethodModifier::StaticModifier | codegen::MethodModifier::VirtualModifier |
                             codegen::MethodModifier::FinalModifier | codegen::MethodModifier::ConstModifier;
    const auto methodUnit = factory->CreateMethod("Compute", "int", methodFlags);
    methodUnit->Append(factory->CreatePrintStatement("run"), 0);

    const std::string rendered = methodUnit->Render(1);

    EXPECT_TRUE(Contains(rendered, "static int Compute() final const {"));
    EXPECT_FALSE(Contains(rendered, "virtual int Compute()"));
    EXPECT_TRUE(Contains(rendered, "printf( \"run\" );"));
}

// Кейс: C++ абстрактный метод рендерится как pure virtual без тела.
TEST(CppRenderTest, AbstractMethodUsesPureVirtualTerminator)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CppLanguage);
    const auto methodFlags = codegen::MethodModifier::VirtualModifier | codegen::MethodModifier::AbstractModifier;
    const auto methodUnit = factory->CreateMethod("Process", "void", methodFlags);

    const std::string rendered = methodUnit->Render(1);

    EXPECT_TRUE(Contains(rendered, "virtual void Process() = 0;"));
    EXPECT_FALSE(Contains(rendered, "{"));
}

// Кейс: C++ метод без модификаторов рендерится как обычный метод с телом.
TEST(CppRenderTest, PlainMethodRendersWithoutExtraModifiers)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CppLanguage);
    const auto methodUnit = factory->CreateMethod("Run", "void", 0);
    methodUnit->Append(factory->CreatePrintStatement("ok"), 0);

    const std::string rendered = methodUnit->Render(1);

    EXPECT_TRUE(Contains(rendered, "void Run() {"));
    EXPECT_FALSE(Contains(rendered, "static void Run()"));
    EXPECT_FALSE(Contains(rendered, "virtual void Run()"));
}

// Кейс: C++ методы покрывают отдельные варианты static/final/const.
TEST(CppRenderTest, MethodsRenderStandaloneStaticFinalAndConstModifiers)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CppLanguage);
    const auto staticMethod = factory->CreateMethod("StaticRun", "void", codegen::MethodModifier::StaticModifier);
    const auto finalMethod = factory->CreateMethod("Finalize", "void", codegen::MethodModifier::FinalModifier);
    const auto constMethod = factory->CreateMethod("Inspect", "int", codegen::MethodModifier::ConstModifier);

    EXPECT_TRUE(Contains(staticMethod->Render(1), "static void StaticRun() {"));
    EXPECT_TRUE(Contains(finalMethod->Render(1), "void Finalize() final {"));
    EXPECT_TRUE(Contains(constMethod->Render(1), "int Inspect() const {"));
}

// Кейс: C++ юниты поля и печати рендерят ожидаемый синтаксис языка.
TEST(CppRenderTest, FieldAndPrintStatementsRenderExpectedSyntax)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CppLanguage);
    const auto fieldFlags = codegen::MethodModifier::StaticModifier | codegen::MethodModifier::ConstModifier;
    const auto fieldUnit = factory->CreateField("cache_", "int", fieldFlags);
    const auto printUnit = factory->CreatePrintStatement("Hello");

    EXPECT_EQ(fieldUnit->Render(1), " static const int cache_;\n");
    EXPECT_EQ(printUnit->Render(1), " printf( \"Hello\" );\n");
}

// Кейс: C++ поле поддерживает одиночный const без static.
TEST(CppRenderTest, FieldSupportsStandaloneConstModifier)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CppLanguage);
    const auto fieldUnit = factory->CreateField("limit_", "int", codegen::MethodModifier::ConstModifier);

    EXPECT_EQ(fieldUnit->Render(1), " const int limit_;\n");
}

// Кейс: C++ поле поддерживает одиночный static без const.
TEST(CppRenderTest, FieldSupportsStandaloneStaticModifier)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CppLanguage);
    const auto fieldUnit = factory->CreateField("shared_", "int", codegen::MethodModifier::StaticModifier);

    EXPECT_EQ(fieldUnit->Render(1), " static int shared_;\n");
}

// Кейс: C++ поле выбрасывает исключение для неподдерживаемого модификатора.
TEST(CppRenderTest, FieldThrowsForUnsupportedModifier)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CppLanguage);
    const auto fieldUnit = factory->CreateField("value_", "int", codegen::MethodModifier::VirtualModifier);

    EXPECT_THROW(fieldUnit->Render(1), std::invalid_argument);
}

// Кейс: Рендер C++ класса включает модификатор final и pure virtual метод.
TEST(CodeRenderTest, CppRendersFinalClassWithPureVirtualMethod)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CppLanguage);
    const auto classUnit = factory->CreateClass("Worker", codegen::ClassModifier::FinalModifier);

    const auto methodFlags = codegen::MethodModifier::VirtualModifier | codegen::MethodModifier::AbstractModifier;
    classUnit->Append(factory->CreateMethod("Process", "void", methodFlags),
                      ToAccessFlags(codegen::AccessModifier::PublicAccess));

    const std::string rendered = classUnit->Render();

    EXPECT_TRUE(Contains(rendered, "class Worker final"));
    EXPECT_TRUE(Contains(rendered, "public:"));
    EXPECT_TRUE(Contains(rendered, "virtual void Process() = 0;"));
}

// Кейс: C++ конкретная перегрузка Append(accessModifier) раскладывает членов по секциям доступа.
TEST(CodeRenderTest, CppConcreteAppendOverloadRendersProtectedAndPrivateSections)
{
    auto classUnit = std::static_pointer_cast<codegen::CppClassUnit>(
        codegen::CreateFactory(codegen::Language::CppLanguage)->CreateClass("Sections", 0));

    classUnit->Append(std::make_shared<RawLineUnit>(), codegen::AccessModifier::PublicAccess);
    classUnit->Append(std::make_shared<RawLineUnit>(), codegen::AccessModifier::ProtectedAccess);
    classUnit->Append(std::make_shared<RawLineUnit>(), codegen::AccessModifier::PrivateAccess);

    const std::string rendered = classUnit->Render();

    EXPECT_TRUE(Contains(rendered, "public:\npayload;\n"));
    EXPECT_TRUE(Contains(rendered, "protected:\npayload;\n"));
    EXPECT_TRUE(Contains(rendered, "private:\npayload;\n"));
}

// Кейс: Java класс рендерит комбинированные модификаторы и выбрасывает исключение для неподдерживаемого доступа.
TEST(JavaRenderTest, ClassRendersModifiersAndThrowsForUnsupportedAccess)
{
    const auto factory = codegen::CreateFactory(codegen::Language::JavaLanguage);
    const auto classFlags = codegen::ClassModifier::AbstractModifier | codegen::ClassModifier::FinalModifier;
    const auto classUnit = factory->CreateClass("JavaDemo", classFlags);

    EXPECT_THROW(classUnit->Append(factory->CreateMethod("Run", "void", 0),
                                   ToAccessFlags(codegen::AccessModifier::InternalAccess)),
                 std::invalid_argument);

    const std::string rendered = classUnit->Render();

    EXPECT_TRUE(Contains(rendered, "abstract final class JavaDemo"));
}

// Кейс: Java класс без флагов рендерит обычный заголовок без abstract/final.
TEST(JavaRenderTest, ClassWithoutModifiersRendersPlainHeader)
{
    const auto factory = codegen::CreateFactory(codegen::Language::JavaLanguage);
    const auto classUnit = factory->CreateClass("PlainJava", 0);

    const std::string rendered = classUnit->Render();

    EXPECT_TRUE(Contains(rendered, "class PlainJava {"));
    EXPECT_FALSE(Contains(rendered, "abstract class PlainJava"));
    EXPECT_FALSE(Contains(rendered, "final class PlainJava"));
}

// Кейс: Java класс с final рендерит только final-префикс.
TEST(JavaRenderTest, ClassWithFinalModifierRendersFinalHeader)
{
    const auto factory = codegen::CreateFactory(codegen::Language::JavaLanguage);
    const auto classUnit = factory->CreateClass("FinalJava", codegen::ClassModifier::FinalModifier);

    const std::string rendered = classUnit->Render();

    EXPECT_TRUE(Contains(rendered, "final class FinalJava {"));
    EXPECT_FALSE(Contains(rendered, "abstract final class FinalJava"));
}

// Кейс: Java класс поддерживает private-члены через access wrapper.
TEST(JavaRenderTest, ClassRendersPrivateMember)
{
    const auto factory = codegen::CreateFactory(codegen::Language::JavaLanguage);
    const auto classUnit = factory->CreateClass("Secrets", 0);

    classUnit->Append(factory->CreateField("token", "String", 0), ToAccessFlags(codegen::AccessModifier::PrivateAccess));

    EXPECT_TRUE(Contains(classUnit->Render(), "private String token;"));
}

// Кейс: Java абстрактный метод завершается точкой с запятой и не содержит тела.
TEST(JavaRenderTest, AbstractMethodUsesSemicolonWithoutBody)
{
    const auto factory = codegen::CreateFactory(codegen::Language::JavaLanguage);
    const auto methodFlags = codegen::MethodModifier::StaticModifier | codegen::MethodModifier::FinalModifier |
                             codegen::MethodModifier::AbstractModifier;
    const auto methodUnit = factory->CreateMethod("Process", "void", methodFlags);

    const std::string rendered = methodUnit->Render(1);

    EXPECT_TRUE(Contains(rendered, "static final abstract void Process();"));
    EXPECT_FALSE(Contains(rendered, "{"));
}

// Кейс: Java конкретный метод рендерит тело и инструкцию печати.
TEST(JavaRenderTest, ConcreteMethodRendersBody)
{
    const auto factory = codegen::CreateFactory(codegen::Language::JavaLanguage);
    const auto methodUnit = factory->CreateMethod("Print", "void", 0);
    methodUnit->Append(factory->CreatePrintStatement("ok"), 0);

    const std::string rendered = methodUnit->Render(1);

    EXPECT_TRUE(Contains(rendered, "void Print() {"));
    EXPECT_TRUE(Contains(rendered, "System.out.println(\"ok\");"));
}

// Кейс: Java методы покрывают отдельные варианты static/final без abstract.
TEST(JavaRenderTest, MethodsRenderStandaloneStaticAndFinalModifiers)
{
    const auto factory = codegen::CreateFactory(codegen::Language::JavaLanguage);
    const auto staticMethod = factory->CreateMethod("RunStatic", "void", codegen::MethodModifier::StaticModifier);
    const auto finalMethod = factory->CreateMethod("RunFinal", "void", codegen::MethodModifier::FinalModifier);

    EXPECT_TRUE(Contains(staticMethod->Render(1), "static void RunStatic() {"));
    EXPECT_TRUE(Contains(finalMethod->Render(1), "final void RunFinal() {"));
}

// Кейс: Java методы поддерживают комбинации static abstract и final abstract.
TEST(JavaRenderTest, MethodsRenderStaticAbstractAndFinalAbstractModifiers)
{
    const auto factory = codegen::CreateFactory(codegen::Language::JavaLanguage);
    const auto staticAbstractMethod =
        factory->CreateMethod("Compose", "void", codegen::MethodModifier::StaticAbstractModifier);
    const auto finalAbstractMethod =
        factory->CreateMethod("Plan", "void", codegen::MethodModifier::FinalAbstractModifier);

    EXPECT_TRUE(Contains(staticAbstractMethod->Render(1), "static abstract void Compose();"));
    EXPECT_TRUE(Contains(finalAbstractMethod->Render(1), "final abstract void Plan();"));
}

// Кейс: Java юниты поля и печати рендерят ожидаемый синтаксис языка.
TEST(JavaRenderTest, FieldAndPrintStatementsRenderExpectedSyntax)
{
    const auto factory = codegen::CreateFactory(codegen::Language::JavaLanguage);
    const auto fieldFlags = codegen::MethodModifier::StaticModifier | codegen::MethodModifier::FinalModifier;
    const auto fieldUnit = factory->CreateField("VERSION", "String", fieldFlags);
    const auto printUnit = factory->CreatePrintStatement("ok");

    EXPECT_EQ(fieldUnit->Render(1), " static final String VERSION;\n");
    EXPECT_EQ(printUnit->Render(1), " System.out.println(\"ok\");\n");
}

// Кейс: Java поле покрывает одиночные static и final модификаторы.
TEST(JavaRenderTest, FieldSupportsStandaloneStaticAndFinalModifiers)
{
    const auto factory = codegen::CreateFactory(codegen::Language::JavaLanguage);
    const auto staticField = factory->CreateField("counter", "int", codegen::MethodModifier::StaticModifier);
    const auto finalField = factory->CreateField("name", "String", codegen::MethodModifier::FinalModifier);

    EXPECT_EQ(staticField->Render(1), " static int counter;\n");
    EXPECT_EQ(finalField->Render(1), " final String name;\n");
}

// Кейс: Java поле без флагов рендерится как объявление без модификаторов.
TEST(JavaRenderTest, FieldWithoutModifiersRendersPlainDeclaration)
{
    const auto factory = codegen::CreateFactory(codegen::Language::JavaLanguage);
    const auto fieldUnit = factory->CreateField("value", "int", 0);

    EXPECT_EQ(fieldUnit->Render(1), " int value;\n");
}

// Кейс: Рендер Java класса включает абстрактный метод и static final поле.
TEST(CodeRenderTest, JavaRendersAbstractClassAndStaticFinalField)
{
    const auto factory = codegen::CreateFactory(codegen::Language::JavaLanguage);
    const auto classUnit = factory->CreateClass("MathUtils", codegen::ClassModifier::AbstractModifier);

    const auto fieldFlags = codegen::MethodModifier::StaticModifier | codegen::MethodModifier::FinalModifier;
    classUnit->Append(factory->CreateField("VERSION", "String", fieldFlags),
                      ToAccessFlags(codegen::AccessModifier::PublicAccess));

    const auto methodUnit = factory->CreateMethod("Execute", "void", codegen::MethodModifier::AbstractModifier);
    classUnit->Append(methodUnit, ToAccessFlags(codegen::AccessModifier::ProtectedAccess));

    const std::string rendered = classUnit->Render();

    EXPECT_TRUE(Contains(rendered, "abstract class MathUtils"));
    EXPECT_TRUE(Contains(rendered, "public static final String VERSION;"));
    EXPECT_TRUE(Contains(rendered, "protected abstract void Execute();"));
}

// Кейс: Java поле выбрасывает исключение для неподдерживаемого модификатора.
TEST(JavaRenderTest, FieldThrowsForUnsupportedModifier)
{
    const auto factory = codegen::CreateFactory(codegen::Language::JavaLanguage);
    const auto fieldUnit = factory->CreateField("broken", "int", codegen::MethodModifier::AbstractModifier);

    EXPECT_THROW(fieldUnit->Render(1), std::invalid_argument);
}

// Кейс: Java метод выбрасывает исключение для неподдерживаемого набора модификаторов.
TEST(JavaRenderTest, MethodThrowsForUnsupportedModifier)
{
    const auto factory = codegen::CreateFactory(codegen::Language::JavaLanguage);
    const auto methodUnit = factory->CreateMethod("Broken", "void", codegen::MethodModifier::ConstModifier);

    EXPECT_THROW(methodUnit->Render(1), std::invalid_argument);
}

// Кейс: Java класс выбрасывает исключение для неподдерживаемого модификатора класса.
TEST(JavaRenderTest, ClassThrowsForUnsupportedModifier)
{
    const auto factory = codegen::CreateFactory(codegen::Language::JavaLanguage);
    const auto classUnit = factory->CreateClass("BrokenClass", static_cast<codegen::CodeUnit::Flags>(999));

    EXPECT_THROW(classUnit->Render(), std::invalid_argument);
}

// Кейс: Для C# класса корректно рендерятся префиксы всех поддерживаемых модификаторов доступа.
TEST(CSharpRenderTest, ClassAccessKeywordsAreRendered)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CSharpLanguage);
    struct AccessCase
    {
        codegen::AccessModifier access;
        const char* keyword;
    };

    const std::array<AccessCase, 7> accessCases = {
        AccessCase{codegen::AccessModifier::PublicAccess, "public"},
        AccessCase{codegen::AccessModifier::ProtectedAccess, "protected"},
        AccessCase{codegen::AccessModifier::PrivateAccess, "private"},
        AccessCase{codegen::AccessModifier::PrivateProtectedAccess, "private protected"},
        AccessCase{codegen::AccessModifier::InternalAccess, "internal"},
        AccessCase{codegen::AccessModifier::ProtectedInternalAccess, "protected internal"},
        AccessCase{codegen::AccessModifier::FileAccess, "file"},
    };

    for (const auto& accessCase : accessCases)
    {
        SCOPED_TRACE(accessCase.keyword);

        const auto classUnit = factory->CreateClass("AccessDemo", ToAccessFlags(accessCase.access));
        const std::string rendered = classUnit->Render();

        EXPECT_TRUE(Contains(rendered, std::string(accessCase.keyword) + " class AccessDemo")) << rendered;
    }
}

// Кейс: Для членов C# поддержаны все варианты доступа, а невалидный доступ вызывает исключение.
TEST(CSharpRenderTest, MemberAccessKeywordsIncludeAllSupportedVariants)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CSharpLanguage);
    const auto classUnit = factory->CreateClass("Service", 0);

    classUnit->Append(factory->CreateMethod("PublicMethod", "void", 0),
                      ToAccessFlags(codegen::AccessModifier::PublicAccess));
    classUnit->Append(factory->CreateMethod("ProtectedMethod", "void", 0),
                      ToAccessFlags(codegen::AccessModifier::ProtectedAccess));
    classUnit->Append(factory->CreateMethod("PrivateMethod", "void", 0),
                      ToAccessFlags(codegen::AccessModifier::PrivateAccess));
    classUnit->Append(factory->CreateMethod("PrivateProtectedMethod", "void", 0),
                      ToAccessFlags(codegen::AccessModifier::PrivateProtectedAccess));
    classUnit->Append(factory->CreateMethod("InternalMethod", "void", 0),
                      ToAccessFlags(codegen::AccessModifier::InternalAccess));
    classUnit->Append(factory->CreateMethod("ProtectedInternalMethod", "void", 0),
                      ToAccessFlags(codegen::AccessModifier::ProtectedInternalAccess));
    EXPECT_THROW(classUnit->Append(factory->CreateMethod("FallbackMethod", "void", 0),
                                   static_cast<codegen::CodeUnit::Flags>(999)),
                 std::invalid_argument);

    const std::string rendered = classUnit->Render();

    EXPECT_TRUE(Contains(rendered, "public void PublicMethod()"));
    EXPECT_TRUE(Contains(rendered, "protected void ProtectedMethod()"));
    EXPECT_TRUE(Contains(rendered, "private void PrivateMethod()"));
    EXPECT_TRUE(Contains(rendered, "private protected void PrivateProtectedMethod()"));
    EXPECT_TRUE(Contains(rendered, "internal void InternalMethod()"));
    EXPECT_TRUE(Contains(rendered, "protected internal void ProtectedInternalMethod()"));
}

// Кейс: C# метод рендерит префикс virtual, когда флаг static отсутствует.
TEST(CSharpRenderTest, MethodRendersVirtualWhenStaticAbsent)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CSharpLanguage);
    const auto methodUnit = factory->CreateMethod("VirtualOp", "void", codegen::MethodModifier::VirtualModifier);

    const std::string rendered = methodUnit->Render(1);

    EXPECT_TRUE(Contains(rendered, "virtual void VirtualOp() {"));
    EXPECT_FALSE(Contains(rendered, "static void VirtualOp()"));
}

// Кейс: C# метод отдает приоритет static над virtual и добавляет ключевое слово sealed.
TEST(CSharpRenderTest, MethodPrefersStaticOverVirtualAndSupportsSealed)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CSharpLanguage);
    const auto methodFlags = codegen::MethodModifier::StaticModifier | codegen::MethodModifier::VirtualModifier |
                             codegen::MethodModifier::FinalModifier;
    const auto methodUnit = factory->CreateMethod("Compute", "void", methodFlags);
    methodUnit->Append(factory->CreatePrintStatement("run"), 0);

    const std::string rendered = methodUnit->Render(1);

    EXPECT_TRUE(Contains(rendered, "static sealed void Compute() {"));
    EXPECT_FALSE(Contains(rendered, "virtual void Compute()"));
    EXPECT_TRUE(Contains(rendered, "Console.WriteLine(\"run\");"));
}

// Кейс: C# метод поддерживает комбинацию abstract sealed без static/virtual.
TEST(CSharpRenderTest, MethodSupportsAbstractSealedCombination)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CSharpLanguage);
    const auto methodUnit = factory->CreateMethod("Compose", "void", codegen::MethodModifier::FinalAbstractModifier);

    EXPECT_TRUE(Contains(methodUnit->Render(1), "abstract sealed void Compose();"));
}

// Кейс: C# абстрактный метод завершается точкой с запятой и не содержит тела.
TEST(CSharpRenderTest, AbstractMethodUsesSemicolonWithoutBody)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CSharpLanguage);
    const auto methodUnit = factory->CreateMethod("Execute", "void", codegen::MethodModifier::AbstractModifier);

    const std::string rendered = methodUnit->Render(1);

    EXPECT_TRUE(Contains(rendered, "abstract void Execute();"));
    EXPECT_FALSE(Contains(rendered, "{"));
}

// Кейс: C# юниты поля и печати рендерят ожидаемый синтаксис языка.
TEST(CSharpRenderTest, FieldAndPrintStatementsRenderExpectedSyntax)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CSharpLanguage);
    const auto fieldFlags = codegen::MethodModifier::StaticModifier | codegen::MethodModifier::FinalModifier;
    const auto fieldUnit = factory->CreateField("Version_", "string", fieldFlags);
    const auto printUnit = factory->CreatePrintStatement("ok");

    EXPECT_EQ(fieldUnit->Render(1), " static readonly string Version_;\n");
    EXPECT_EQ(printUnit->Render(1), " Console.WriteLine(\"ok\");\n");
}

// Кейс: C# поле поддерживает readonly без static.
TEST(CSharpRenderTest, FieldSupportsStandaloneReadonlyModifier)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CSharpLanguage);
    const auto fieldUnit = factory->CreateField("value_", "int", codegen::MethodModifier::FinalModifier);

    EXPECT_EQ(fieldUnit->Render(1), " readonly int value_;\n");
}

// Кейс: C# поле поддерживает одиночный static без readonly.
TEST(CSharpRenderTest, FieldSupportsStandaloneStaticModifier)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CSharpLanguage);
    const auto fieldUnit = factory->CreateField("cache_", "int", codegen::MethodModifier::StaticModifier);

    EXPECT_EQ(fieldUnit->Render(1), " static int cache_;\n");
}

// Кейс: C# поле без флагов рендерится как объявление без модификаторов.
TEST(CSharpRenderTest, FieldWithoutModifiersRendersPlainDeclaration)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CSharpLanguage);
    const auto fieldUnit = factory->CreateField("count_", "int", 0);

    EXPECT_EQ(fieldUnit->Render(1), " int count_;\n");
}

// Кейс: Модификаторы C# класса корректно рендерят варианты abstract и sealed.
TEST(CSharpRenderTest, ClassModifiersRenderAbstractAndSealed)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CSharpLanguage);
    const auto abstractClass = factory->CreateClass("AbstractType", codegen::ClassModifier::AbstractModifier);
    const auto sealedClass = factory->CreateClass("SealedType", codegen::ClassModifier::FinalModifier);

    EXPECT_TRUE(Contains(abstractClass->Render(), "abstract class AbstractType"));
    EXPECT_TRUE(Contains(sealedClass->Render(), "sealed class SealedType"));
}

// Кейс: C# класс поддерживает комбинацию abstract sealed.
TEST(CSharpRenderTest, ClassSupportsAbstractSealedCombination)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CSharpLanguage);
    const auto classFlags = codegen::ClassModifier::AbstractModifier | codegen::ClassModifier::FinalModifier;
    const auto classUnit = factory->CreateClass("Utility", classFlags);

    EXPECT_TRUE(Contains(classUnit->Render(), "abstract sealed class Utility"));
}

// Кейс: C# член класса с Unknown-доступом отклоняется.
TEST(CSharpRenderTest, MemberAccessThrowsForUnknownModifier)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CSharpLanguage);
    const auto classUnit = factory->CreateClass("BrokenAccess", 0);

    EXPECT_THROW(classUnit->Append(factory->CreateMethod("Ghost", "void", 0), 0), std::invalid_argument);
}

// Кейс: C# класс выбрасывает исключение для несовместимой комбинации публичного и приватного доступа.
TEST(CSharpRenderTest, ClassThrowsForUnsupportedAccessCombination)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CSharpLanguage);
    const auto invalidAccessFlags = codegen::AccessModifier::PublicAccess | codegen::AccessModifier::PrivateAccess;
    const auto classUnit = factory->CreateClass("BrokenClass", invalidAccessFlags);

    EXPECT_THROW(classUnit->Render(), std::invalid_argument);
}

// Кейс: Рендер C# класса включает sealed класс, internal поле и protected internal метод.
TEST(CodeRenderTest, CSharpRendersFileScopedSealedClass)
{
    const auto factory = codegen::CreateFactory(codegen::Language::CSharpLanguage);
    const auto classFlags = codegen::ClassModifier::FinalModifier;
    const auto classUnit = factory->CreateClass("Service", classFlags);

    const auto fieldFlags = codegen::MethodModifier::StaticModifier | codegen::MethodModifier::FinalModifier;
    classUnit->Append(factory->CreateField("Version_", "string", fieldFlags),
                      ToAccessFlags(codegen::AccessModifier::InternalAccess));

    const auto methodUnit = factory->CreateMethod("Log", "void", codegen::MethodModifier::StaticModifier);
    methodUnit->Append(factory->CreatePrintStatement("ok"), 0);
    classUnit->Append(methodUnit, ToAccessFlags(codegen::AccessModifier::ProtectedInternalAccess));

    const std::string rendered = classUnit->Render();

    EXPECT_TRUE(Contains(rendered, "sealed class Service"));
    EXPECT_TRUE(Contains(rendered, "internal static readonly string Version_;"));
    EXPECT_TRUE(Contains(rendered, "protected internal static void Log()"));
    EXPECT_TRUE(Contains(rendered, "Console.WriteLine(\"ok\");"));
}
