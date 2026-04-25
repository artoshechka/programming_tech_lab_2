#include <code_factory.hpp>
#include <examples.hpp>
#include <exception>
#include <iostream>

namespace
{

void PrintLanguageExamples(codegen::Language language)
{
    const auto factory = codegen::CreateFactory(language);

    std::cout << "\n" << factory->GetLanguageName() << " Code Examples\n";
    std::cout << "\nRegular Class with Access Modifiers:\n" << examples::RenderRegularClass(language);
    std::cout << "\nFinal Class:\n" << examples::RenderFinalClass(language);
    std::cout << "\nAbstract Class:\n" << examples::RenderAbstractClass(language);
    std::cout << "\nStatic Methods:\n" << examples::RenderStaticExample(language);
}

}  // namespace

int main()
{
    try
    {
        PrintLanguageExamples(codegen::Language::CppLanguage);
        PrintLanguageExamples(codegen::Language::JavaLanguage);
        PrintLanguageExamples(codegen::Language::CSharpLanguage);
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
