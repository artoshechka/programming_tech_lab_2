#include <iostream>
#include <src/class_unit.hpp>
#include <src/method_unit.hpp>
#include <src/print_operator_unit.hpp>
#include <unit.hpp>

namespace
{
std::string GenerateProgram()
{
    codegen::ClassDeclarationUnit my_class("MyClass");
    my_class.Append(std::make_shared<codegen::MethodDeclarationUnit>("testFunc1", "void", 0),
                    codegen::AccessModifier::publicAccess);
    my_class.Append(
        std::make_shared<codegen::MethodDeclarationUnit>(
            "testFunc2", "void", static_cast<codegen::CodeUnit::Flags>(codegen::MethodModifier::staticModifier)),
        codegen::AccessModifier::privateAccess);
    my_class.Append(std::make_shared<codegen::MethodDeclarationUnit>(
                        "testFunc3", "void",
                        static_cast<codegen::CodeUnit::Flags>(codegen::MethodModifier::virtualModifier) |
                            static_cast<codegen::CodeUnit::Flags>(codegen::MethodModifier::constModifier)),
                    codegen::AccessModifier::publicAccess);
    auto method = std::make_shared<codegen::MethodDeclarationUnit>(
        "testFunc4", "void", static_cast<codegen::CodeUnit::Flags>(codegen::MethodModifier::staticModifier));
    method->Append(std::make_shared<codegen::PrintStatementUnit>(R"(Hello, world!\n)"));
    my_class.Append(method, codegen::AccessModifier::protectedAccess);
    return my_class.Render();
}
}  // namespace

int main()
{
    std::cout << GenerateProgram() << std::endl;
    return 0;
}