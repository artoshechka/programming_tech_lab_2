#ifndef GUID_6e60ed6b_94e5_41ea_9be8_6d15d770f0a1
#define GUID_6e60ed6b_94e5_41ea_9be8_6d15d770f0a1

#include <unit.hpp>
#include <string>
#include <vector>

class MethodUnit : public Unit
{
  public:
    enum Modifier
    {
        staticModifier = 1,
        constModifier = 1 << 1,
        virtualModifier = 1 << 2
    };

  public:
    MethodUnit(const std::string &name, const std::string &return_type, flags flags_value);
    void Add(const std::shared_ptr<Unit> &unit, flags /* flags_value */ = 0) override;
    std::string Compile(unsigned int level = 0) const override;

  private:
    std::string name_;
    std::string return_type_;
    flags flags_;
    std::vector<std::shared_ptr<Unit>> body_;
};

#endif