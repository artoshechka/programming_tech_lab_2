#ifndef GUID_8f9f34c0_9d5f_4874_bf16_e50baf617204
#define GUID_8f9f34c0_9d5f_4874_bf16_e50baf617204

#include <string>
#include <unit.hpp>

namespace codegen
{

class PrintStatementUnit : public CodeUnit
{
   public:
    explicit PrintStatementUnit(const std::string& text);
    std::string Render(unsigned int indent_level = 0) const override;

   private:
    std::string text_;
};

}  // namespace codegen

#endif