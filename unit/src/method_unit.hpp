#ifndef GUID_6e60ed6b_94e5_41ea_9be8_6d15d770f0a1
#define GUID_6e60ed6b_94e5_41ea_9be8_6d15d770f0a1

#include <string>
#include <unit.hpp>
#include <vector>

namespace codegen
{

enum class MethodModifier : CodeUnit::flags { staticModifier = 1, constModifier = 1 << 1, virtualModifier = 1 << 2 };

class MethodDeclarationUnit : public CodeUnit
{
   public:
    MethodDeclarationUnit(const std::string& name, const std::string& return_type, flags flags_value);
    void Append(const std::shared_ptr<CodeUnit>& unit, flags /* flags_value */ = 0) override;
    std::string Render(unsigned int indent_level = 0) const override;

   private:
    std::string name_;
    std::string return_type_;
    flags flags_;
    std::vector<std::shared_ptr<CodeUnit>> body_;
};

}  // namespace codegen

#endif