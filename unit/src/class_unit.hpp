#ifndef GUID_5b1d3d42_1db0_4fd1_901f_05aa2fd3f31d
#define GUID_5b1d3d42_1db0_4fd1_901f_05aa2fd3f31d

#include <string>
#include <unit.hpp>
#include <vector>

namespace codegen
{

enum class AccessModifier { publicAccess, protectedAccess, privateAccess };

class ClassDeclarationUnit : public CodeUnit
{
   public:
    explicit ClassDeclarationUnit(const std::string& name);
    void Append(const std::shared_ptr<CodeUnit>& unit, AccessModifier access_modifier);
    void Append(const std::shared_ptr<CodeUnit>& unit, flags flags_value) override;
    std::string Render(unsigned int indent_level = 0) const override;

   private:
    static const std::vector<std::string> accessModifiers_;
    std::string name_;
    using fields = std::vector<std::shared_ptr<CodeUnit>>;
    std::vector<fields> fields_;
};

}  // namespace codegen

#endif