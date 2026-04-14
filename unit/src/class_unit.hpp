#ifndef GUID_5b1d3d42_1db0_4fd1_901f_05aa2fd3f31d
#define GUID_5b1d3d42_1db0_4fd1_901f_05aa2fd3f31d

#include <string>
#include <unit.hpp>
#include <vector>

namespace codegen
{

enum class AccessModifier { publicAccess, protectedAccess, privateAccess };

class ClassUnit : public Unit
{
   public:
    explicit ClassUnit(const std::string& name);
    void Add(const std::shared_ptr<Unit>& unit, AccessModifier access_modifier);
    void Add(const std::shared_ptr<Unit>& unit, flags flags_value) override;
    std::string Compile(unsigned int level = 0) const override;

   private:
    static const std::vector<std::string> accessModifiers_;
    std::string name_;
    using fields = std::vector<std::shared_ptr<Unit>>;
    std::vector<fields> fields_;
};

}  // namespace codegen

#endif