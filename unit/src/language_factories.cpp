/// @file
/// @brief Точка входа для создания языковых фабрик генерации кода.
#include <code_factory.hpp>
#include <src/cpp/cpp_factory.hpp>
#include <src/csharp/csharp_factory.hpp>
#include <src/java/java_factory.hpp>

namespace codegen
{

/// @brief Фабрика для создания языковой фабрики по выбранному языку.
/// Является точкой входа для получения конкретной реализации ICodeFactory.
/// @param[in] language Целевой язык программирования.
/// @return Указатель на создаваемую фабрику для выбранного языка.
/// @throw std::invalid_argument Если передан неизвестный язык.
std::shared_ptr<ICodeFactory> CreateFactory(Language language)
{
    switch (language)
    {
        case Language::CppLanguage:
            return std::make_shared<cpp::CppCodeFactory>();
        case Language::JavaLanguage:
            return std::make_shared<java::JavaCodeFactory>();
        case Language::CSharpLanguage:
            return std::make_shared<csharp::CSharpCodeFactory>();
        default:
            throw std::invalid_argument("Unsupported language");
    }
}

}  // namespace codegen
