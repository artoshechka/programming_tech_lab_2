
#ifndef GUID_0f5d2f9d_8b4a_4e8f_96cf_f3e301ab6b21
#define GUID_0f5d2f9d_8b4a_4e8f_96cf_f3e301ab6b21

#include <memory>
#include <string>

namespace codegen
{

class CodeUnit
{
   public:
    using flags = unsigned int;

   public:
    virtual ~CodeUnit() = default;
    virtual void Append(const std::shared_ptr<CodeUnit>&, flags);
    virtual std::string Render(unsigned int indent_level = 0) const = 0;

   protected:
    virtual std::string MakeIndent(unsigned int indent_level) const;
};

}  // namespace codegen

#endif