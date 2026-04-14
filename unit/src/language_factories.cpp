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
/// @note По умолчанию возвращает C++-фабрику при некорректном значении параметра.
std::shared_ptr<ICodeFactory> CreateFactory(Language language)
{
    switch (language)
    {
        case Language::cppLanguage:
            return std::make_shared<cpp::CppCodeFactory>();
        case Language::javaLanguage:
            return std::make_shared<java::JavaCodeFactory>();
        case Language::csharpLanguage:
            return std::make_shared<csharp::CSharpCodeFactory>();
        default:
            return std::make_shared<cpp::CppCodeFactory>();
    }
}

}  // namespace codegen
