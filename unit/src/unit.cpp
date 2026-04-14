#include <stdexcept>
#include <unit.hpp>

using codegen::CodeUnit;

void CodeUnit::Append(const std::shared_ptr<CodeUnit>&, flags)
{
    throw std::runtime_error("Not supported");
}

std::string CodeUnit::MakeIndent(unsigned int indent_level) const
{
    static const auto DEFAULT_SHIFT = " ";
    std::string result;
    for (unsigned int i = 0; i < indent_level; ++i)
    {
        result += DEFAULT_SHIFT;
    }
    return result;
}
