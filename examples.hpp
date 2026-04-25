#ifndef GUID_45b3aa22_8dff_4d2f_9de8_3d70f352c26d
#define GUID_45b3aa22_8dff_4d2f_9de8_3d70f352c26d

#include <code_factory.hpp>
#include <codegen_types.hpp>
#include <string>
#include <utility>
#include <vector>

namespace examples
{

struct MethodConfig
{
    std::string name;
    std::string returnType;
    codegen::CodeUnit::Flags flags = 0;
    codegen::AccessModifier access = codegen::AccessModifier::PublicAccess;
    std::vector<std::string> printStatements;
};

struct FieldConfig
{
    std::string name;
    std::string type;
    codegen::CodeUnit::Flags flags = 0;
    codegen::AccessModifier access = codegen::AccessModifier::PrivateAccess;
};

template <typename EnumType>
inline codegen::CodeUnit::Flags ToFlags(EnumType value)
{
    return static_cast<codegen::CodeUnit::Flags>(value);
}

inline bool IsLanguage(codegen::Language actual, codegen::Language expected)
{
    return actual == expected;
}

inline std::string GetStringType(codegen::Language language)
{
    if (IsLanguage(language, codegen::Language::JavaLanguage))
    {
        return "String";
    }
    if (IsLanguage(language, codegen::Language::CSharpLanguage))
    {
        return "string";
    }
    return "std::string";
}

inline codegen::CodeUnit::Flags GetImmutableFieldFlags(codegen::Language language)
{
    if (IsLanguage(language, codegen::Language::CppLanguage))
    {
        return ToFlags(codegen::MethodModifier::ConstModifier);
    }
    return ToFlags(codegen::MethodModifier::FinalModifier);
}

inline std::string Demo(codegen::Language language, codegen::CodeUnit::Flags classFlags,
                        const std::vector<MethodConfig>& methods, const std::vector<FieldConfig>& fields,
                        const std::string& className = "DemoClass")
{
    const auto factory = codegen::CreateFactory(language);
    const auto classUnit = factory->CreateClass(className, classFlags);

    for (const auto& field : fields)
    {
        classUnit->Append(factory->CreateField(field.name, field.type, field.flags), field.access);
    }

    for (const auto& method : methods)
    {
        const auto methodUnit = factory->CreateMethod(method.name, method.returnType, method.flags);
        for (const auto& text : method.printStatements)
        {
            methodUnit->Append(factory->CreatePrintStatement(text), 0);
        }
        classUnit->Append(methodUnit, method.access);
    }

    return classUnit->Render();
}

inline std::string RenderRegularClass(codegen::Language language)
{
    codegen::CodeUnit::Flags classFlags = 0;
    if (IsLanguage(language, codegen::Language::CSharpLanguage))
    {
        classFlags = codegen::AccessModifier::FileAccess | codegen::AccessModifier::Unknown;
    }

    std::vector<MethodConfig> methods = {
        {"publicMethod", "void", 0, codegen::AccessModifier::PublicAccess, {}},
        {"privateMethod", "void", 0, codegen::AccessModifier::PrivateAccess, {}},
        {"protectedMethod", "void", 0, codegen::AccessModifier::ProtectedAccess, {}},
        {"printMethod",
         "void",
         ToFlags(codegen::MethodModifier::StaticModifier),
         codegen::AccessModifier::PublicAccess,
         {"Hello, world!"}},
        {"finalMethod",
         "void",
         ToFlags(codegen::MethodModifier::FinalModifier),
         codegen::AccessModifier::PublicAccess,
         {"This is a final method"}},
    };

    if (IsLanguage(language, codegen::Language::CSharpLanguage))
    {
        methods.push_back({"privateProtectedMethod", "void", 0, codegen::AccessModifier::PrivateProtectedAccess, {}});
        methods.push_back({"internalMethod", "void", 0, codegen::AccessModifier::InternalAccess, {}});
        methods.push_back({"protectedInternalMethod", "void", 0, codegen::AccessModifier::ProtectedInternalAccess, {}});
    }

    const std::vector<FieldConfig> fields = {
        {"name_", GetStringType(language), GetImmutableFieldFlags(language), codegen::AccessModifier::PrivateAccess},
        {"instanceCount_", "int", ToFlags(codegen::MethodModifier::StaticModifier),
         codegen::AccessModifier::PrivateAccess},
    };

    return Demo(language, classFlags, methods, fields, "DemoClass");
}

inline std::string RenderFinalClass(codegen::Language language)
{
    if (IsLanguage(language, codegen::Language::JavaLanguage))
    {
        return Demo(language, ToFlags(codegen::ClassModifier::FinalModifier),
                    {{"getValue", "int", 0, codegen::AccessModifier::PublicAccess, {"Returning immutable value..."}}},
                    {}, "ImmutableData");
    }

    if (IsLanguage(language, codegen::Language::CSharpLanguage))
    {
        return Demo(language, ToFlags(codegen::ClassModifier::FinalModifier),
                    {{"Execute", "void", 0, codegen::AccessModifier::PublicAccess, {"Sealed class execution..."}}}, {},
                    "SealedImpl");
    }

    return Demo(
        language, ToFlags(codegen::ClassModifier::FinalModifier),
        {{"doSomething", "void", 0, codegen::AccessModifier::PublicAccess, {"Final class cannot be derived..."}}}, {},
        "FinalClass");
}

inline std::string RenderAbstractClass(codegen::Language language)
{
    if (IsLanguage(language, codegen::Language::JavaLanguage))
    {
        return Demo(language, ToFlags(codegen::ClassModifier::AbstractModifier),
                    {
                        {"processData",
                         "void",
                         ToFlags(codegen::MethodModifier::AbstractModifier),
                         codegen::AccessModifier::PublicAccess,
                         {}},
                        {"validateInput", "boolean", 0, codegen::AccessModifier::PublicAccess, {"Validating..."}},
                    },
                    {}, "InterfaceImpl");
    }

    if (IsLanguage(language, codegen::Language::CSharpLanguage))
    {
        return Demo(language, ToFlags(codegen::ClassModifier::AbstractModifier),
                    {
                        {"Execute",
                         "void",
                         ToFlags(codegen::MethodModifier::AbstractModifier),
                         codegen::AccessModifier::PublicAccess,
                         {}},
                        {"Configure",
                         "void",
                         ToFlags(codegen::MethodModifier::VirtualModifier),
                         codegen::AccessModifier::PublicAccess,
                         {"Configuring..."}},
                    },
                    {}, "InterfaceImpl");
    }

    return Demo(language, ToFlags(codegen::ClassModifier::AbstractModifier),
                {
                    {"process",
                     "void",
                     ToFlags(codegen::MethodModifier::VirtualModifier | codegen::MethodModifier::AbstractModifier),
                     codegen::AccessModifier::PublicAccess,
                     {}},
                    {"cleanup",
                     "void",
                     ToFlags(codegen::MethodModifier::VirtualModifier),
                     codegen::AccessModifier::PublicAccess,
                     {"Cleaning up..."}},
                },
                {}, "InterfaceImpl");
}

inline std::string RenderStaticExample(codegen::Language language)
{
    if (IsLanguage(language, codegen::Language::JavaLanguage))
    {
        return Demo(language, 0,
                    {
                        {"sqrt",
                         "double",
                         ToFlags(codegen::MethodModifier::StaticModifier),
                         codegen::AccessModifier::PublicAccess,
                         {"Computing square root..."}},
                        {"PI_VALUE",
                         "double",
                         ToFlags(codegen::MethodModifier::StaticModifier | codegen::MethodModifier::FinalModifier),
                         codegen::AccessModifier::PublicAccess,
                         {}},
                    },
                    {
                        {"PI", "double",
                         ToFlags(codegen::MethodModifier::StaticModifier | codegen::MethodModifier::FinalModifier),
                         codegen::AccessModifier::PublicAccess},
                    },
                    "MathUtils");
    }

    if (IsLanguage(language, codegen::Language::CSharpLanguage))
    {
        return Demo(language, 0,
                    {
                        {"Add",
                         "int",
                         ToFlags(codegen::MethodModifier::StaticModifier),
                         codegen::AccessModifier::PublicAccess,
                         {"Adding numbers..."}},
                        {"Version",
                         "string",
                         ToFlags(codegen::MethodModifier::StaticModifier | codegen::MethodModifier::FinalModifier),
                         codegen::AccessModifier::PublicAccess,
                         {}},
                    },
                    {
                        {"Version_", "string",
                         ToFlags(codegen::MethodModifier::StaticModifier | codegen::MethodModifier::FinalModifier),
                         codegen::AccessModifier::PublicAccess},
                    },
                    "Calculator");
    }

    return Demo(language, 0,
                {
                    {"calculate",
                     "int",
                     ToFlags(codegen::MethodModifier::StaticModifier),
                     codegen::AccessModifier::PublicAccess,
                     {"Calculating..."}},
                },
                {
                    {"kVersion_", "const char*",
                     ToFlags(codegen::MethodModifier::StaticModifier | codegen::MethodModifier::ConstModifier),
                     codegen::AccessModifier::PublicAccess},
                },
                "Utils");
}

}  // namespace examples

#endif
