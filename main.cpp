#include <code_factory.hpp>
#include <codegen_types.hpp>
#include <iostream>
#include <vector>

using codegen::ToFlags;

namespace
{

std::string GenerateProgram(const codegen::ICodeFactory& factory)
{
    const auto myClass = factory.CreateClass("MyClass");

    myClass->Append(factory.CreateMethod("testFunc1", "void", 0),
                    static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

    myClass->Append(factory.CreateMethod("testFunc2", "void", ToFlags(codegen::MethodModifier::staticModifier)),
                    static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::privateAccess));

    myClass->Append(factory.CreateMethod("testFunc3", "void",
                                         ToFlags(codegen::MethodModifier::virtualModifier) |
                                             ToFlags(codegen::MethodModifier::constModifier)),
                    static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::publicAccess));

    const auto method = factory.CreateMethod("testFunc4", "void", ToFlags(codegen::MethodModifier::staticModifier));
    method->Append(factory.CreatePrintStatement("Hello, world!"), 0);
    myClass->Append(method, static_cast<codegen::CodeUnit::Flags>(codegen::AccessModifier::protectedAccess));

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
