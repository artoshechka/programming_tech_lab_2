#include <unit/src/method_unit.hpp>
#include <unit/src/class_unit.hpp>
#include <unit/src/print_operator_unit.hpp>
#include <unit.hpp>
#include <iostream>

namespace
{
std::string generate_program()
{
    ClassUnit my_class("MyClass");
    my_class.Add(std::make_shared<MethodUnit>("testFunc1", "void", 0), ClassUnit::publicAccess);
    my_class.Add(std::make_shared<MethodUnit>("testFunc2", "void", MethodUnit::staticModifier),
                 ClassUnit::privateAccess);
    my_class.Add(
        std::make_shared<MethodUnit>("testFunc3", "void", MethodUnit::virtualModifier | MethodUnit::constModifier),
        ClassUnit::publicAccess);
    auto method = std::make_shared<MethodUnit>("testFunc4", "void", MethodUnit::staticModifier);
    method->Add(std::make_shared<PrintOperatorUnit>(R"(Hello, world!\n)"));
    my_class.Add(method, ClassUnit::protectedAccess);
    return my_class.Compile();
}
} // namespace

int main()
{
    std::cout << generate_program() << std::endl;
    return 0;
}