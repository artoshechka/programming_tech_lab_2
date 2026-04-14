
#ifndef GUID_0f5d2f9d_8b4a_4e8f_96cf_f3e301ab6b21
#define GUID_0f5d2f9d_8b4a_4e8f_96cf_f3e301ab6b21

#include <memory>
#include <string>

class Unit
{
  public:
    using Flags = unsigned int;

  public:
    virtual ~Unit() = default;
    virtual void add(const std::shared_ptr<Unit> &, Flags);
    virtual std::string compile(unsigned int level = 0) const = 0;

  protected:
    virtual std::string generateShift(unsigned int level) const;
};

#endif