#include <iostream>
#include <src/class_unit.hpp>
#include <src/method_unit.hpp>
#include <src/print_operator_unit.hpp>
#include <unit.hpp>

namespace
{
std::string generate_program()
{
    codegen::ClassUnit my_class("MyClass");
    my_class.Add(std::make_shared<codegen::MethodUnit>("testFunc1", "void", 0), codegen::AccessModifier::publicAccess);
    my_class.Add(std::make_shared<codegen::MethodUnit>(
                     "testFunc2", "void", static_cast<codegen::Unit::flags>(codegen::MethodModifier::staticModifier)),
                 codegen::AccessModifier::privateAccess);
    my_class.Add(std::make_shared<codegen::MethodUnit>(
                     "testFunc3", "void",
                     static_cast<codegen::Unit::flags>(codegen::MethodModifier::virtualModifier) |
                         static_cast<codegen::Unit::flags>(codegen::MethodModifier::constModifier)),
                 codegen::AccessModifier::publicAccess);
    auto method = std::make_shared<codegen::MethodUnit>(
        "testFunc4", "void", static_cast<codegen::Unit::flags>(codegen::MethodModifier::staticModifier));
    method->Add(std::make_shared<codegen::PrintOperatorUnit>(R"(Hello, world!\n)"));
    my_class.Add(method, codegen::AccessModifier::protectedAccess);
    return my_class.Compile();
}
}  // namespace

int main()
{
    std::cout << generate_program() << std::endl;
    return 0;
}