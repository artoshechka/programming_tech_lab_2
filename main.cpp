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

    return myClass->Render();
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
        std::cout << "===== " << factory->GetLanguageName() << " =====\n";
        std::cout << GenerateProgram(*factory) << std::endl;
    }

    return 0;
}
