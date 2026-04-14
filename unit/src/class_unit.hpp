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
        PUBLIC,
        PROTECTED,
        PRIVATE
    };
    static const std::vector<std::string> ACCESS_MODIFIERS;

  public:
    explicit ClassUnit(const std::string &name);
    void add(const std::shared_ptr<Unit> &unit, Flags flags) override;
    std::string compile(unsigned int level = 0) const override;

  private:
    std::string m_name;
    using Fields = std::vector<std::shared_ptr<Unit>>;
    std::vector<Fields> m_fields;
};

#endif