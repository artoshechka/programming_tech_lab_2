#include <code_factory.hpp>
#include <examples.hpp>
#include <exception>
#include <iostream>
#include <string>

namespace
{

constexpr int kBarWidth = 52;

void PrintSection(const std::string& title, const std::string& code)
{
    std::cout << "\n[" << title << "]\n" << std::string(kBarWidth, '-') << "\n" << code;
}

void PrintLanguageExamples(codegen::Language language)
{
    const auto factory = codegen::CreateFactory(language);
    const auto lang = factory->GetLanguageName();

    std::cout << "\n" << std::string(kBarWidth, '=') << "\n";
    std::cout << " " << lang << " Code Examples\n";
    std::cout << std::string(kBarWidth, '=') << "\n";

    PrintSection("Regular Class", examples::RenderRegularClass(language));
    PrintSection("Final Class", examples::RenderFinalClass(language));
    PrintSection("Abstract Class", examples::RenderAbstractClass(language));
    PrintSection("Static Methods", examples::RenderStaticExample(language));
    PrintSection("Error Handling", examples::RenderIncorrectExamples(language));
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
