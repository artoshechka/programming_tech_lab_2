#include <code_factory.hpp>
#include <codegen_types.hpp>
#include <iostream>
#include <vector>

using codegen::ToFlags;

namespace
{

std::string GenerateRegularClass(const codegen::ICodeFactory& factory)
{
    const std::string languageName = factory.GetLanguageName();

    const codegen::CodeUnit::Flags classAccess = [&languageName]() {
        if (languageName == "C#") return static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::fileAccess);
        return static_cast<codegen::CodeUnit::Flags>(0);
    }();

    const auto myClass = factory.CreateClass("MyClass", classAccess);

    const std::string fieldType = [&languageName]() {
        if (languageName == "Java") return std::string("String");
        if (languageName == "C#") return std::string("string");
        return std::string("std::string");
    }();

    const codegen::CodeUnit::Flags immutableFieldFlags = [&languageName]() {
        if (languageName == "C++") return ToFlags(codegen::MethodModifier::constModifier);
        return ToFlags(codegen::MethodModifier::finalModifier);
    }();

    myClass->Append(factory.CreateField("name_", fieldType, immutableFieldFlags),
                    static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::privateAccess));
    myClass->Append(factory.CreateField("instanceCount_", "int", ToFlags(codegen::MethodModifier::staticModifier)),
                    static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::privateAccess));

    myClass->Append(factory.CreateMethod("publicMethod", "void", 0),
                    static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));
    myClass->Append(factory.CreateMethod("privateMethod", "void", 0),
                    static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::privateAccess));
    myClass->Append(factory.CreateMethod("protectedMethod", "void", 0),
                    static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::protectedAccess));

    if (languageName == "C#")
    {
        myClass->Append(factory.CreateMethod("privateProtectedMethod", "void", 0),
                        static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::privateProtectedAccess));
        myClass->Append(factory.CreateMethod("internalMethod", "void", 0),
                        static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::internalAccess));
        myClass->Append(factory.CreateMethod("protectedInternalMethod", "void", 0),
                        static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::protectedInternalAccess));
    }

    const auto printMethod =
        factory.CreateMethod("printMethod", "void", ToFlags(codegen::MethodModifier::staticModifier));
    printMethod->Append(factory.CreatePrintStatement("Hello, world!"), 0);
    myClass->Append(printMethod, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

    const auto finalMethod =
        factory.CreateMethod("finalMethod", "void", ToFlags(codegen::MethodModifier::finalModifier));
    finalMethod->Append(factory.CreatePrintStatement("This is a final method"), 0);
    myClass->Append(finalMethod, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

    return myClass->Render();
}

std::string GenerateFinalClass(const codegen::ICodeFactory& factory)
{
    const std::string languageName = factory.GetLanguageName();

    if (languageName == "Java")
    {
        const auto finalClass = factory.CreateClass("ImmutableData", ToFlags(codegen::ClassModifier::finalModifier));

        const auto method = factory.CreateMethod("getValue", "int", 0);
        method->Append(factory.CreatePrintStatement("Returning immutable value..."), 0);
        finalClass->Append(method, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

        return finalClass->Render();
    } else if (languageName == "C#")
    {
        const auto sealedClass = factory.CreateClass("SealedImpl", ToFlags(codegen::ClassModifier::finalModifier));

        const auto method = factory.CreateMethod("Execute", "void", 0);
        method->Append(factory.CreatePrintStatement("Sealed class execution..."), 0);
        sealedClass->Append(method, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

        return sealedClass->Render();
    } else if (languageName == "C++")
    {
        const auto finalClass = factory.CreateClass("FinalClass", ToFlags(codegen::ClassModifier::finalModifier));

        const auto method = factory.CreateMethod("doSomething", "void", 0);
        method->Append(factory.CreatePrintStatement("Final class cannot be derived..."), 0);
        finalClass->Append(method, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

        return finalClass->Render();
    }

    return "";
}

std::string GenerateAbstractClass(const codegen::ICodeFactory& factory)
{
    const std::string languageName = factory.GetLanguageName();
    const auto abstractClass = factory.CreateClass("InterfaceImpl", ToFlags(codegen::ClassModifier::abstractModifier));

    if (languageName == "Java")
    {
        const auto abstractMethod =
            factory.CreateMethod("processData", "void", ToFlags(codegen::MethodModifier::abstractModifier));
        abstractClass->Append(abstractMethod,
                              static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

        const auto regularMethod = factory.CreateMethod("validateInput", "boolean", 0);
        regularMethod->Append(factory.CreatePrintStatement("Validating..."), 0);
        abstractClass->Append(regularMethod,
                              static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));
    } else if (languageName == "C#")
    {
        const auto abstractMethod =
            factory.CreateMethod("Execute", "void", ToFlags(codegen::MethodModifier::abstractModifier));
        abstractClass->Append(abstractMethod,
                              static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

        const auto virtualMethod =
            factory.CreateMethod("Configure", "void", ToFlags(codegen::MethodModifier::virtualModifier));
        virtualMethod->Append(factory.CreatePrintStatement("Configuring..."), 0);
        abstractClass->Append(virtualMethod,
                              static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));
    } else if (languageName == "C++")
    {
        const auto pureVirtual = factory.CreateMethod(
            "process", "void",
            ToFlags(codegen::MethodModifier::virtualModifier) | ToFlags(codegen::MethodModifier::abstractModifier));
        abstractClass->Append(pureVirtual,
                              static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

        const auto virtualMethod =
            factory.CreateMethod("cleanup", "void", ToFlags(codegen::MethodModifier::virtualModifier));
        virtualMethod->Append(factory.CreatePrintStatement("Cleaning up..."), 0);
        abstractClass->Append(virtualMethod,
                              static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));
    }

    return abstractClass->Render();
}

std::string GenerateStaticExample(const codegen::ICodeFactory& factory)
{
    const std::string languageName = factory.GetLanguageName();

    if (languageName == "Java")
    {
        const auto utilClass = factory.CreateClass("MathUtils", 0);

        utilClass->Append(factory.CreateField("PI", "double",
                                              ToFlags(codegen::MethodModifier::staticModifier) |
                                                  ToFlags(codegen::MethodModifier::finalModifier)),
                          static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

        const auto staticMethod =
            factory.CreateMethod("sqrt", "double", ToFlags(codegen::MethodModifier::staticModifier));
        staticMethod->Append(factory.CreatePrintStatement("Computing square root..."), 0);
        utilClass->Append(staticMethod, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

        const auto finalStaticMethod = factory.CreateMethod(
            "PI_VALUE", "double",
            ToFlags(codegen::MethodModifier::staticModifier) | ToFlags(codegen::MethodModifier::finalModifier));
        utilClass->Append(finalStaticMethod,
                          static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

        return utilClass->Render();
    } else if (languageName == "C#")
    {
        const auto utilClass = factory.CreateClass("Calculator", 0);

        utilClass->Append(factory.CreateField("Version_", "string",
                                              ToFlags(codegen::MethodModifier::staticModifier) |
                                                  ToFlags(codegen::MethodModifier::finalModifier)),
                          static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

        const auto staticMethod = factory.CreateMethod("Add", "int", ToFlags(codegen::MethodModifier::staticModifier));
        staticMethod->Append(factory.CreatePrintStatement("Adding numbers..."), 0);
        utilClass->Append(staticMethod, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

        const auto sealedStatic = factory.CreateMethod(
            "Version", "string",
            ToFlags(codegen::MethodModifier::staticModifier) | ToFlags(codegen::MethodModifier::finalModifier));
        utilClass->Append(sealedStatic, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

        return utilClass->Render();
    } else if (languageName == "C++")
    {
        const auto utilClass = factory.CreateClass("Utils", 0);

        utilClass->Append(factory.CreateField("kVersion_", "const char*",
                                              ToFlags(codegen::MethodModifier::staticModifier) |
                                                  ToFlags(codegen::MethodModifier::constModifier)),
                          static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

        const auto staticMethod =
            factory.CreateMethod("calculate", "int", ToFlags(codegen::MethodModifier::staticModifier));
        staticMethod->Append(factory.CreatePrintStatement("Calculating..."), 0);
        utilClass->Append(staticMethod, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

        return utilClass->Render();
    }

    return "";
}

}  // namespace

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
        const std::string languageName = factory->GetLanguageName();

        std::cout << "\n" << languageName << " Code Examples\n";

        std::cout << "\nRegular Class with Access Modifiers:\n";
        std::cout << GenerateRegularClass(*factory);

        std::cout << "\nFinal Class:\n";
        std::cout << GenerateFinalClass(*factory);

        std::cout << "\nAbstract Class:\n";
        std::cout << GenerateAbstractClass(*factory);

        std::cout << "\nStatic Methods:\n";
        std::cout << GenerateStaticExample(*factory);
    }

    return 0;
}
