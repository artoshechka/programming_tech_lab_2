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
        STATIC = 1,
        CONST = 1 << 1,
        VIRTUAL = 1 << 2
    };

  public:
    MethodUnit(const std::string &name, const std::string &returnType, Flags flags);
    void add(const std::shared_ptr<Unit> &unit, Flags /* flags */ = 0) override;
    std::string compile(unsigned int level = 0) const override;

  private:
    std::string m_name;
    std::string m_returnType;
    Flags m_flags;
    std::vector<std::shared_ptr<Unit>> m_body;
};

#endif