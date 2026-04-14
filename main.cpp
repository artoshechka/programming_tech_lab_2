#include <code_factory.hpp>
#include <codegen_types.hpp>
#include <iostream>
#include <vector>

using codegen::ToFlags;

namespace
{

std::string GenerateProgram(const codegen::ICodeFactory& factory)
{
    const bool isCSharp = factory.GetLanguageName() == "C#";

    const codegen::CodeUnit::Flags classAccess =
        isCSharp ? static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::fileAccess) : 0;
    const auto myClass = factory.CreateClass("MyClass", classAccess);

    myClass->Append(factory.CreateMethod("publicMethod", "void", 0),
                    static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));
    myClass->Append(factory.CreateMethod("privateMethod", "void", 0),
                    static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::privateAccess));
    myClass->Append(factory.CreateMethod("protectedMethod", "void", 0),
                    static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::protectedAccess));

    if (isCSharp)
    {
        myClass->Append(factory.CreateMethod("privateProtectedMethod", "void", 0),
                        static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::privateProtectedAccess));
        myClass->Append(factory.CreateMethod("internalMethod", "void", 0),
                        static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::internalAccess));
        myClass->Append(factory.CreateMethod("protectedInternalMethod", "void", 0),
                        static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::protectedInternalAccess));
    }

    const auto printMethod = factory.CreateMethod("printMethod", "void", ToFlags(codegen::MethodModifier::staticModifier));
    printMethod->Append(factory.CreatePrintStatement("Hello, world!"), 0);
    myClass->Append(printMethod, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

    // Пример с final методом
    const auto finalMethod = factory.CreateMethod("finalMethod", "void", ToFlags(codegen::MethodModifier::finalModifier));
    finalMethod->Append(factory.CreatePrintStatement("This is a final method"), 0);
    myClass->Append(finalMethod, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

    return myClass->Render();
}

}  // namespace

std::string GenerateFinalClass(const codegen::ICodeFactory& factory)
{
    // Создаём final класс
    const auto finalClass = factory.CreateClass("FinalClass", ToFlags(codegen::ClassModifier::finalModifier));

    // Добавляем public метод
    const auto method = factory.CreateMethod("doSomething", "void", 0);
    method->Append(factory.CreatePrintStatement("Doing something..."), 0);
    finalClass->Append(method, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

    return finalClass->Render();
}

std::string GenerateAbstractClass(const codegen::ICodeFactory& factory)
{
    // Создаём abstract класс
    const auto abstractClass = factory.CreateClass("AbstractClass", ToFlags(codegen::ClassModifier::abstractModifier));

    // Добавляем abstract метод
    const auto abstractMethod = factory.CreateMethod("abstractMethod", "void", ToFlags(codegen::MethodModifier::abstractModifier));
    abstractClass->Append(abstractMethod, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

    // Добавляем обычный метод
    const auto regularMethod = factory.CreateMethod("regularMethod", "void", 0);
    regularMethod->Append(factory.CreatePrintStatement("This is a regular method"), 0);
    abstractClass->Append(regularMethod, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

    return abstractClass->Render();
}


int main()
{
    const std::vector<codegen::Language> languages = {
        codegen::Language::cppLanguage,
        codegen::Language::javaLanguage,
        codegen::Language::csharpLanguage,
    };

    for (const auto language : languages)
    {
        const auto factory = codegen::CreateFactory(language);
        std::cout << "\n===== " << factory->GetLanguageName() << " - Regular Class =====\n";
        std::cout << GenerateProgram(*factory);

        std::cout << "\n===== " << factory->GetLanguageName() << " - Final Class =====\n";
        std::cout << GenerateFinalClass(*factory);

        std::cout << "\n===== " << factory->GetLanguageName() << " - Abstract Class =====\n";
        std::cout << GenerateAbstractClass(*factory) << std::endl;
    }

    return 0;
}
