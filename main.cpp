#include <code_factory.hpp>
#include <codegen_types.hpp>
#include <exception>
#include <iostream>
#include <vector>

using codegen::ToFlags;

namespace
{

std::string GenerateRegularClass(const codegen::ICodeFactory& factory)
{
    const std::string languageName = factory.GetLanguageName();

    const codegen::CodeUnit::Flags classAccess = [&languageName]() {
        if (languageName == "C#") return static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::FileAccess);
        return static_cast<codegen::CodeUnit::Flags>(0);
    }();

    const auto myClass = factory.CreateClass("MyClass", classAccess);

    const std::string fieldType = [&languageName]() {
        if (languageName == "Java") return std::string("String");
        if (languageName == "C#") return std::string("string");
        return std::string("std::string");
    }();

    const codegen::CodeUnit::Flags immutableFieldFlags = [&languageName]() {
        if (languageName == "C++") return ToFlags(codegen::MethodModifier::ConstModifier);
        return ToFlags(codegen::MethodModifier::FinalModifier);
    }();

    myClass->Append(factory.CreateField("name_", fieldType, immutableFieldFlags),
                    static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PrivateAccess));
    myClass->Append(factory.CreateField("instanceCount_", "int", ToFlags(codegen::MethodModifier::StaticModifier)),
                    static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PrivateAccess));

    myClass->Append(factory.CreateMethod("publicMethod", "void", 0),
                    static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));
    myClass->Append(factory.CreateMethod("privateMethod", "void", 0),
                    static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PrivateAccess));
    myClass->Append(factory.CreateMethod("protectedMethod", "void", 0),
                    static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::ProtectedAccess));

    if (languageName == "C#")
    {
        myClass->Append(factory.CreateMethod("privateProtectedMethod", "void", 0),
                        static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PrivateProtectedAccess));
        myClass->Append(factory.CreateMethod("internalMethod", "void", 0),
                        static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::InternalAccess));
        myClass->Append(factory.CreateMethod("protectedInternalMethod", "void", 0),
                        static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::ProtectedInternalAccess));
    }

    const auto printMethod =
        factory.CreateMethod("printMethod", "void", ToFlags(codegen::MethodModifier::StaticModifier));
    printMethod->Append(factory.CreatePrintStatement("Hello, world!"), 0);
    myClass->Append(printMethod, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));

    const auto finalMethod =
        factory.CreateMethod("finalMethod", "void", ToFlags(codegen::MethodModifier::FinalModifier));
    finalMethod->Append(factory.CreatePrintStatement("This is a final method"), 0);
    myClass->Append(finalMethod, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));

    return myClass->Render();
}

std::string GenerateFinalClass(const codegen::ICodeFactory& factory)
{
    const std::string languageName = factory.GetLanguageName();

    if (languageName == "Java")
    {
        const auto finalClass = factory.CreateClass("ImmutableData", ToFlags(codegen::ClassModifier::FinalModifier));

        const auto method = factory.CreateMethod("getValue", "int", 0);
        method->Append(factory.CreatePrintStatement("Returning immutable value..."), 0);
        finalClass->Append(method, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));

        return finalClass->Render();
    } else if (languageName == "C#")
    {
        const auto sealedClass = factory.CreateClass("SealedImpl", ToFlags(codegen::ClassModifier::FinalModifier));

        const auto method = factory.CreateMethod("Execute", "void", 0);
        method->Append(factory.CreatePrintStatement("Sealed class execution..."), 0);
        sealedClass->Append(method, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));

        return sealedClass->Render();
    } else if (languageName == "C++")
    {
        const auto finalClass = factory.CreateClass("FinalClass", ToFlags(codegen::ClassModifier::FinalModifier));

        const auto method = factory.CreateMethod("doSomething", "void", 0);
        method->Append(factory.CreatePrintStatement("Final class cannot be derived..."), 0);
        finalClass->Append(method, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));

        return finalClass->Render();
    }

    return "";
}

std::string GenerateAbstractClass(const codegen::ICodeFactory& factory)
{
    const std::string languageName = factory.GetLanguageName();
    const auto abstractClass = factory.CreateClass("InterfaceImpl", ToFlags(codegen::ClassModifier::AbstractModifier));

    if (languageName == "Java")
    {
        const auto abstractMethod =
            factory.CreateMethod("processData", "void", ToFlags(codegen::MethodModifier::AbstractModifier));
        abstractClass->Append(abstractMethod,
                              static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));

        const auto regularMethod = factory.CreateMethod("validateInput", "boolean", 0);
        regularMethod->Append(factory.CreatePrintStatement("Validating..."), 0);
        abstractClass->Append(regularMethod,
                              static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));
    } else if (languageName == "C#")
    {
        const auto abstractMethod =
            factory.CreateMethod("Execute", "void", ToFlags(codegen::MethodModifier::AbstractModifier));
        abstractClass->Append(abstractMethod,
                              static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));

        const auto virtualMethod =
            factory.CreateMethod("Configure", "void", ToFlags(codegen::MethodModifier::VirtualModifier));
        virtualMethod->Append(factory.CreatePrintStatement("Configuring..."), 0);
        abstractClass->Append(virtualMethod,
                              static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));
    } else if (languageName == "C++")
    {
        const auto pureVirtual = factory.CreateMethod(
            "process", "void",
            ToFlags(codegen::MethodModifier::VirtualModifier) | ToFlags(codegen::MethodModifier::AbstractModifier));
        abstractClass->Append(pureVirtual,
                              static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));

        const auto virtualMethod =
            factory.CreateMethod("cleanup", "void", ToFlags(codegen::MethodModifier::VirtualModifier));
        virtualMethod->Append(factory.CreatePrintStatement("Cleaning up..."), 0);
        abstractClass->Append(virtualMethod,
                              static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));
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
                                              ToFlags(codegen::MethodModifier::StaticModifier) |
                                                  ToFlags(codegen::MethodModifier::FinalModifier)),
                          static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));

        const auto staticMethod =
            factory.CreateMethod("sqrt", "double", ToFlags(codegen::MethodModifier::StaticModifier));
        staticMethod->Append(factory.CreatePrintStatement("Computing square root..."), 0);
        utilClass->Append(staticMethod, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));

        const auto finalStaticMethod = factory.CreateMethod(
            "PI_VALUE", "double",
            ToFlags(codegen::MethodModifier::StaticModifier) | ToFlags(codegen::MethodModifier::FinalModifier));
        utilClass->Append(finalStaticMethod,
                          static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));

        return utilClass->Render();
    } else if (languageName == "C#")
    {
        const auto utilClass = factory.CreateClass("Calculator", 0);

        utilClass->Append(factory.CreateField("Version_", "string",
                                              ToFlags(codegen::MethodModifier::StaticModifier) |
                                                  ToFlags(codegen::MethodModifier::FinalModifier)),
                          static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));

        const auto staticMethod = factory.CreateMethod("Add", "int", ToFlags(codegen::MethodModifier::StaticModifier));
        staticMethod->Append(factory.CreatePrintStatement("Adding numbers..."), 0);
        utilClass->Append(staticMethod, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));

        const auto sealedStatic = factory.CreateMethod(
            "Version", "string",
            ToFlags(codegen::MethodModifier::StaticModifier) | ToFlags(codegen::MethodModifier::FinalModifier));
        utilClass->Append(sealedStatic, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));

        return utilClass->Render();
    } else if (languageName == "C++")
    {
        const auto utilClass = factory.CreateClass("Utils", 0);

        utilClass->Append(factory.CreateField("kVersion_", "const char*",
                                              ToFlags(codegen::MethodModifier::StaticModifier) |
                                                  ToFlags(codegen::MethodModifier::ConstModifier)),
                          static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));

        const auto staticMethod =
            factory.CreateMethod("calculate", "int", ToFlags(codegen::MethodModifier::StaticModifier));
        staticMethod->Append(factory.CreatePrintStatement("Calculating..."), 0);
        utilClass->Append(staticMethod, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::PublicAccess));

        return utilClass->Render();
    }

    return "";
}

}  // namespace

int main()
{
    try
    {
        const std::vector<codegen::Language> languages = {
            codegen::Language::CppLanguage,
            codegen::Language::JavaLanguage,
            codegen::Language::CSharpLanguage,
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
    } catch (const std::exception& error)
    {
        std::cerr << "Error: " << error.what() << "\n";
    } catch (...)
    {
        std::cerr << "Error: unknown exception\n";
    }

    return 1;
}
