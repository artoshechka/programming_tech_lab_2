#include <gtest/gtest.h>

#include <array>
#include <code_factory.hpp>
#include <codegen_types.hpp>
#include <memory>
#include <src/common/abstract_print_unit.hpp>
#include <src/common/access_controlled_unit.hpp>
#include <stdexcept>
#include <string>
#include <utility>

namespace
{

codegen::CodeUnit::Flags ToAccessFlags(codegen::AccessModifier access)
{
    return static_cast<codegen::CodeUnit::Flags>(access);
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

}  // namespace

// Кейс: CreateFactory возвращает корректные имена для всех поддерживаемых языков.
TEST(CreateFactoryTest, ReturnsExpectedLanguageName)
{
    const auto cppFactory = codegen::CreateFactory(codegen::Language::cppLanguage);
    const auto javaFactory = codegen::CreateFactory(codegen::Language::javaLanguage);
    const auto csharpFactory = codegen::CreateFactory(codegen::Language::csharpLanguage);

    ASSERT_NE(cppFactory, nullptr);
    ASSERT_NE(javaFactory, nullptr);
    ASSERT_NE(csharpFactory, nullptr);

    EXPECT_EQ(cppFactory->GetLanguageName(), "C++");
    EXPECT_EQ(javaFactory->GetLanguageName(), "Java");
    EXPECT_EQ(csharpFactory->GetLanguageName(), "C#");
}

// Кейс: CreateFactory переключается на C++ при неизвестном значении enum языка.
TEST(CreateFactoryTest, UnknownLanguageFallsBackToCpp)
{
    const auto invalidLanguage = static_cast<codegen::Language>(999);
    const auto factory = codegen::CreateFactory(invalidLanguage);

    ASSERT_NE(factory, nullptr);
    EXPECT_EQ(factory->GetLanguageName(), "C++");
}

// Кейс: Листовые юниты отклоняют Append() и выбрасывают ожидаемый runtime_error.
TEST(BaseCodeUnitTest, AppendOnLeafFieldThrowsNotSupported)
{
    const auto factory = codegen::CreateFactory(codegen::Language::cppLanguage);
    const auto field = factory->CreateField("value_", "int", 0);

    EXPECT_THROW(field->Append(factory->CreatePrintStatement("x"), 0), std::runtime_error);
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

// Кейс: C++ класс использует секцию private по умолчанию для неподдерживаемого модификатора доступа.
TEST(CppRenderTest, ClassFallsBackToPrivateSectionForUnsupportedAccess)
{
    const auto factory = codegen::CreateFactory(codegen::Language::cppLanguage);
    const auto classUnit = factory->CreateClass("FallbackDemo", 0);

    classUnit->Append(factory->CreateField("value_", "int", 0), ToAccessFlags(codegen::AccessModifier::fileAccess));

    const std::string rendered = classUnit->Render();

    EXPECT_TRUE(Contains(rendered, "private:"));
    EXPECT_TRUE(Contains(rendered, "int value_;"));
}

// Кейс: C++ метод отдает приоритет static над virtual и рендерит суффиксы final/const.
TEST(CppRenderTest, MethodPrefersStaticOverVirtualAndRendersSuffixes)
{
    const auto factory = codegen::CreateFactory(codegen::Language::cppLanguage);
    const auto methodFlags = codegen::ToFlags(codegen::MethodModifier::staticModifier) |
                             codegen::ToFlags(codegen::MethodModifier::virtualModifier) |
                             codegen::ToFlags(codegen::MethodModifier::finalModifier) |
                             codegen::ToFlags(codegen::MethodModifier::constModifier);
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
    const auto factory = codegen::CreateFactory(codegen::Language::cppLanguage);
    const auto methodFlags = codegen::ToFlags(codegen::MethodModifier::virtualModifier) |
                             codegen::ToFlags(codegen::MethodModifier::abstractModifier);
    const auto methodUnit = factory->CreateMethod("Process", "void", methodFlags);

    const std::string rendered = methodUnit->Render(1);

    EXPECT_TRUE(Contains(rendered, "virtual void Process() = 0;"));
    EXPECT_FALSE(Contains(rendered, "{"));
}

// Кейс: C++ юниты поля и печати рендерят ожидаемый синтаксис языка.
TEST(CppRenderTest, FieldAndPrintStatementsRenderExpectedSyntax)
{
    const auto factory = codegen::CreateFactory(codegen::Language::cppLanguage);
    const auto fieldFlags = codegen::ToFlags(codegen::MethodModifier::staticModifier) |
                            codegen::ToFlags(codegen::MethodModifier::constModifier);
    const auto fieldUnit = factory->CreateField("cache_", "int", fieldFlags);
    const auto printUnit = factory->CreatePrintStatement("Hello");

    EXPECT_EQ(fieldUnit->Render(1), " static const int cache_;\n");
    EXPECT_EQ(printUnit->Render(1), " printf( \"Hello\" );\n");
}

// Кейс: Рендер C++ класса включает модификатор final и pure virtual метод.
TEST(CodeRenderTest, CppRendersFinalClassWithPureVirtualMethod)
{
    const auto factory = codegen::CreateFactory(codegen::Language::cppLanguage);
    const auto classUnit = factory->CreateClass("Worker", codegen::ToFlags(codegen::ClassModifier::finalModifier));

    const auto methodFlags = codegen::ToFlags(codegen::MethodModifier::virtualModifier) |
                             codegen::ToFlags(codegen::MethodModifier::abstractModifier);
    classUnit->Append(factory->CreateMethod("Process", "void", methodFlags),
                      ToAccessFlags(codegen::AccessModifier::publicAccess));

    const std::string rendered = classUnit->Render();

    EXPECT_TRUE(Contains(rendered, "class Worker final"));
    EXPECT_TRUE(Contains(rendered, "public:"));
    EXPECT_TRUE(Contains(rendered, "virtual void Process() = 0;"));
}

// Кейс: Java класс рендерит комбинированные модификаторы и использует private для неподдерживаемого доступа.
TEST(JavaRenderTest, ClassRendersModifiersAndDefaultsUnsupportedAccessToPrivate)
{
    const auto factory = codegen::CreateFactory(codegen::Language::javaLanguage);
    const auto classFlags = codegen::ToFlags(codegen::ClassModifier::abstractModifier) |
                            codegen::ToFlags(codegen::ClassModifier::finalModifier);
    const auto classUnit = factory->CreateClass("JavaDemo", classFlags);
    classUnit->Append(factory->CreateMethod("Run", "void", 0), ToAccessFlags(codegen::AccessModifier::internalAccess));

    const std::string rendered = classUnit->Render();

    EXPECT_TRUE(Contains(rendered, "abstract final class JavaDemo"));
    EXPECT_TRUE(Contains(rendered, "private void Run()"));
}

// Кейс: Java класс без флагов рендерит обычный заголовок без abstract/final.
TEST(JavaRenderTest, ClassWithoutModifiersRendersPlainHeader)
{
    const auto factory = codegen::CreateFactory(codegen::Language::javaLanguage);
    const auto classUnit = factory->CreateClass("PlainJava", 0);

    const std::string rendered = classUnit->Render();

    EXPECT_TRUE(Contains(rendered, "class PlainJava {"));
    EXPECT_FALSE(Contains(rendered, "abstract class PlainJava"));
    EXPECT_FALSE(Contains(rendered, "final class PlainJava"));
}

// Кейс: Java абстрактный метод завершается точкой с запятой и не содержит тела.
TEST(JavaRenderTest, AbstractMethodUsesSemicolonWithoutBody)
{
    const auto factory = codegen::CreateFactory(codegen::Language::javaLanguage);
    const auto methodFlags = codegen::ToFlags(codegen::MethodModifier::staticModifier) |
                             codegen::ToFlags(codegen::MethodModifier::finalModifier) |
                             codegen::ToFlags(codegen::MethodModifier::abstractModifier);
    const auto methodUnit = factory->CreateMethod("Process", "void", methodFlags);

    const std::string rendered = methodUnit->Render(1);

    EXPECT_TRUE(Contains(rendered, "static final abstract void Process();"));
    EXPECT_FALSE(Contains(rendered, "{"));
}

// Кейс: Java конкретный метод рендерит тело и инструкцию печати.
TEST(JavaRenderTest, ConcreteMethodRendersBody)
{
    const auto factory = codegen::CreateFactory(codegen::Language::javaLanguage);
    const auto methodUnit = factory->CreateMethod("Print", "void", 0);
    methodUnit->Append(factory->CreatePrintStatement("ok"), 0);

    const std::string rendered = methodUnit->Render(1);

    EXPECT_TRUE(Contains(rendered, "void Print() {"));
    EXPECT_TRUE(Contains(rendered, "System.out.println(\"ok\");"));
}

// Кейс: Java юниты поля и печати рендерят ожидаемый синтаксис языка.
TEST(JavaRenderTest, FieldAndPrintStatementsRenderExpectedSyntax)
{
    const auto factory = codegen::CreateFactory(codegen::Language::javaLanguage);
    const auto fieldFlags = codegen::ToFlags(codegen::MethodModifier::staticModifier) |
                            codegen::ToFlags(codegen::MethodModifier::finalModifier);
    const auto fieldUnit = factory->CreateField("VERSION", "String", fieldFlags);
    const auto printUnit = factory->CreatePrintStatement("ok");

    EXPECT_EQ(fieldUnit->Render(1), " static final String VERSION;\n");
    EXPECT_EQ(printUnit->Render(1), " System.out.println(\"ok\");\n");
}

// Кейс: Java поле без флагов рендерится как объявление без модификаторов.
TEST(JavaRenderTest, FieldWithoutModifiersRendersPlainDeclaration)
{
    const auto factory = codegen::CreateFactory(codegen::Language::javaLanguage);
    const auto fieldUnit = factory->CreateField("value", "int", 0);

    EXPECT_EQ(fieldUnit->Render(1), " int value;\n");
}

// Кейс: Рендер Java класса включает абстрактный метод и static final поле.
TEST(CodeRenderTest, JavaRendersAbstractClassAndStaticFinalField)
{
    const auto factory = codegen::CreateFactory(codegen::Language::javaLanguage);
    const auto classUnit =
        factory->CreateClass("MathUtils", codegen::ToFlags(codegen::ClassModifier::abstractModifier));

    const auto fieldFlags = codegen::ToFlags(codegen::MethodModifier::staticModifier) |
                            codegen::ToFlags(codegen::MethodModifier::finalModifier);
    classUnit->Append(factory->CreateField("VERSION", "String", fieldFlags),
                      ToAccessFlags(codegen::AccessModifier::publicAccess));

    const auto methodUnit =
        factory->CreateMethod("Execute", "void", codegen::ToFlags(codegen::MethodModifier::abstractModifier));
    classUnit->Append(methodUnit, ToAccessFlags(codegen::AccessModifier::protectedAccess));

    const std::string rendered = classUnit->Render();

    EXPECT_TRUE(Contains(rendered, "abstract class MathUtils"));
    EXPECT_TRUE(Contains(rendered, "public static final String VERSION;"));
    EXPECT_TRUE(Contains(rendered, "protected abstract void Execute();"));
}

// Кейс: Для C# класса корректно рендерятся префиксы всех поддерживаемых модификаторов доступа.
TEST(CSharpRenderTest, ClassAccessKeywordsAreRendered)
{
    const auto factory = codegen::CreateFactory(codegen::Language::csharpLanguage);
    struct AccessCase
    {
        codegen::AccessModifier access;
        const char* keyword;
    };

    const std::array<AccessCase, 7> accessCases = {
        AccessCase{codegen::AccessModifier::publicAccess, "public"},
        AccessCase{codegen::AccessModifier::protectedAccess, "protected"},
        AccessCase{codegen::AccessModifier::privateAccess, "private"},
        AccessCase{codegen::AccessModifier::privateProtectedAccess, "private protected"},
        AccessCase{codegen::AccessModifier::internalAccess, "internal"},
        AccessCase{codegen::AccessModifier::protectedInternalAccess, "protected internal"},
        AccessCase{codegen::AccessModifier::fileAccess, "file"},
    };

    for (const auto& accessCase : accessCases)
    {
        SCOPED_TRACE(accessCase.keyword);

        const auto classUnit = factory->CreateClass("AccessDemo", ToAccessFlags(accessCase.access));
        const std::string rendered = classUnit->Render();

        EXPECT_TRUE(Contains(rendered, std::string(accessCase.keyword) + " class AccessDemo")) << rendered;
    }
}

// Кейс: Для членов C# поддержаны все варианты доступа и fallback в private.
TEST(CSharpRenderTest, MemberAccessKeywordsIncludeAllSupportedVariants)
{
    const auto factory = codegen::CreateFactory(codegen::Language::csharpLanguage);
    const auto classUnit = factory->CreateClass("Service", 0);

    classUnit->Append(factory->CreateMethod("PublicMethod", "void", 0),
                      ToAccessFlags(codegen::AccessModifier::publicAccess));
    classUnit->Append(factory->CreateMethod("ProtectedMethod", "void", 0),
                      ToAccessFlags(codegen::AccessModifier::protectedAccess));
    classUnit->Append(factory->CreateMethod("PrivateMethod", "void", 0),
                      ToAccessFlags(codegen::AccessModifier::privateAccess));
    classUnit->Append(factory->CreateMethod("PrivateProtectedMethod", "void", 0),
                      ToAccessFlags(codegen::AccessModifier::privateProtectedAccess));
    classUnit->Append(factory->CreateMethod("InternalMethod", "void", 0),
                      ToAccessFlags(codegen::AccessModifier::internalAccess));
    classUnit->Append(factory->CreateMethod("ProtectedInternalMethod", "void", 0),
                      ToAccessFlags(codegen::AccessModifier::protectedInternalAccess));
    classUnit->Append(factory->CreateMethod("FallbackMethod", "void", 0), static_cast<codegen::CodeUnit::Flags>(999));

    const std::string rendered = classUnit->Render();

    EXPECT_TRUE(Contains(rendered, "public void PublicMethod()"));
    EXPECT_TRUE(Contains(rendered, "protected void ProtectedMethod()"));
    EXPECT_TRUE(Contains(rendered, "private void PrivateMethod()"));
    EXPECT_TRUE(Contains(rendered, "private protected void PrivateProtectedMethod()"));
    EXPECT_TRUE(Contains(rendered, "internal void InternalMethod()"));
    EXPECT_TRUE(Contains(rendered, "protected internal void ProtectedInternalMethod()"));
    EXPECT_TRUE(Contains(rendered, "private void FallbackMethod()"));
}

// Кейс: C# метод рендерит префикс virtual, когда флаг static отсутствует.
TEST(CSharpRenderTest, MethodRendersVirtualWhenStaticAbsent)
{
    const auto factory = codegen::CreateFactory(codegen::Language::csharpLanguage);
    const auto methodUnit =
        factory->CreateMethod("VirtualOp", "void", codegen::ToFlags(codegen::MethodModifier::virtualModifier));

    const std::string rendered = methodUnit->Render(1);

    EXPECT_TRUE(Contains(rendered, "virtual void VirtualOp() {"));
    EXPECT_FALSE(Contains(rendered, "static void VirtualOp()"));
}

// Кейс: C# метод отдает приоритет static над virtual и добавляет ключевое слово sealed.
TEST(CSharpRenderTest, MethodPrefersStaticOverVirtualAndSupportsSealed)
{
    const auto factory = codegen::CreateFactory(codegen::Language::csharpLanguage);
    const auto methodFlags = codegen::ToFlags(codegen::MethodModifier::staticModifier) |
                             codegen::ToFlags(codegen::MethodModifier::virtualModifier) |
                             codegen::ToFlags(codegen::MethodModifier::finalModifier);
    const auto methodUnit = factory->CreateMethod("Compute", "void", methodFlags);
    methodUnit->Append(factory->CreatePrintStatement("run"), 0);

    const std::string rendered = methodUnit->Render(1);

    EXPECT_TRUE(Contains(rendered, "static sealed void Compute() {"));
    EXPECT_FALSE(Contains(rendered, "virtual void Compute()"));
    EXPECT_TRUE(Contains(rendered, "Console.WriteLine(\"run\");"));
}

// Кейс: C# абстрактный метод завершается точкой с запятой и не содержит тела.
TEST(CSharpRenderTest, AbstractMethodUsesSemicolonWithoutBody)
{
    const auto factory = codegen::CreateFactory(codegen::Language::csharpLanguage);
    const auto methodUnit =
        factory->CreateMethod("Execute", "void", codegen::ToFlags(codegen::MethodModifier::abstractModifier));

    const std::string rendered = methodUnit->Render(1);

    EXPECT_TRUE(Contains(rendered, "abstract void Execute();"));
    EXPECT_FALSE(Contains(rendered, "{"));
}

// Кейс: C# юниты поля и печати рендерят ожидаемый синтаксис языка.
TEST(CSharpRenderTest, FieldAndPrintStatementsRenderExpectedSyntax)
{
    const auto factory = codegen::CreateFactory(codegen::Language::csharpLanguage);
    const auto fieldFlags = codegen::ToFlags(codegen::MethodModifier::staticModifier) |
                            codegen::ToFlags(codegen::MethodModifier::finalModifier);
    const auto fieldUnit = factory->CreateField("Version_", "string", fieldFlags);
    const auto printUnit = factory->CreatePrintStatement("ok");

    EXPECT_EQ(fieldUnit->Render(1), " static readonly string Version_;\n");
    EXPECT_EQ(printUnit->Render(1), " Console.WriteLine(\"ok\");\n");
}

// Кейс: C# поле без флагов рендерится как объявление без модификаторов.
TEST(CSharpRenderTest, FieldWithoutModifiersRendersPlainDeclaration)
{
    const auto factory = codegen::CreateFactory(codegen::Language::csharpLanguage);
    const auto fieldUnit = factory->CreateField("count_", "int", 0);

    EXPECT_EQ(fieldUnit->Render(1), " int count_;\n");
}

// Кейс: Модификаторы C# класса корректно рендерят варианты abstract и sealed.
TEST(CSharpRenderTest, ClassModifiersRenderAbstractAndSealed)
{
    const auto factory = codegen::CreateFactory(codegen::Language::csharpLanguage);
    const auto abstractClass =
        factory->CreateClass("AbstractType", codegen::ToFlags(codegen::ClassModifier::abstractModifier));
    const auto sealedClass =
        factory->CreateClass("SealedType", codegen::ToFlags(codegen::ClassModifier::finalModifier));

    EXPECT_TRUE(Contains(abstractClass->Render(), "abstract class AbstractType"));
    EXPECT_TRUE(Contains(sealedClass->Render(), "sealed class SealedType"));
}

// Кейс: Рендер C# класса включает sealed класс, internal поле и protected internal метод.
TEST(CodeRenderTest, CSharpRendersFileScopedSealedClass)
{
    const auto factory = codegen::CreateFactory(codegen::Language::csharpLanguage);
    const auto classFlags = codegen::ToFlags(codegen::ClassModifier::finalModifier);
    const auto classUnit = factory->CreateClass("Service", classFlags);

    const auto fieldFlags = codegen::ToFlags(codegen::MethodModifier::staticModifier) |
                            codegen::ToFlags(codegen::MethodModifier::finalModifier);
    classUnit->Append(factory->CreateField("Version_", "string", fieldFlags),
                      ToAccessFlags(codegen::AccessModifier::internalAccess));

    const auto methodUnit =
        factory->CreateMethod("Log", "void", codegen::ToFlags(codegen::MethodModifier::staticModifier));
    methodUnit->Append(factory->CreatePrintStatement("ok"), 0);
    classUnit->Append(methodUnit, ToAccessFlags(codegen::AccessModifier::protectedInternalAccess));

    const std::string rendered = classUnit->Render();

    EXPECT_TRUE(Contains(rendered, "sealed class Service"));
    EXPECT_TRUE(Contains(rendered, "internal static readonly string Version_;"));
    EXPECT_TRUE(Contains(rendered, "protected internal static void Log()"));
    EXPECT_TRUE(Contains(rendered, "Console.WriteLine(\"ok\");"));
}
