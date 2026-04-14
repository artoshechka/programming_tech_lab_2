#ifndef GUID_5b1d3d42_1db0_4fd1_901f_05aa2fd3f31d
#define GUID_5b1d3d42_1db0_4fd1_901f_05aa2fd3f31d

#include <unit.hpp>
#include <string>
#include <vector>

class ClassUnit : public Unit
{
  public:
    enum AccessModifier
    {
        publicAccess,
        protectedAccess,
        privateAccess
    };
    static const std::vector<std::string> access_modifiers_;

  public:
    explicit ClassUnit(const std::string &name);
    void Add(const std::shared_ptr<Unit> &unit, flags flags_value) override;
    std::string Compile(unsigned int level = 0) const override;

  private:
    std::string name_;
    using fields = std::vector<std::shared_ptr<Unit>>;
    std::vector<fields> fields_;
};

#endif