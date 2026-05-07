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
    codegen::MethodModifier flags = codegen::MethodModifier::Unknown;
    codegen::AccessModifier access = codegen::AccessModifier::PublicAccess;
    std::vector<std::string> printStatements;
};

struct FieldConfig
{
    std::string name;
    std::string type;
    codegen::MethodModifier flags = codegen::MethodModifier::Unknown;
    codegen::AccessModifier access = codegen::AccessModifier::PrivateAccess;
};

template <typename EnumType>
inline EnumType ToFlags(EnumType value)
{
    return value;
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

inline codegen::MethodModifier GetImmutableFieldFlags(codegen::Language language)
{
    if (IsLanguage(language, codegen::Language::CppLanguage))
    {
        return ToFlags(codegen::MethodModifier::ConstModifier);
    }
    return ToFlags(codegen::MethodModifier::FinalModifier);
}

inline std::string Demo(codegen::Language language, codegen::ClassModifier classFlags,
                        const std::vector<MethodConfig>& methods, const std::vector<FieldConfig>& fields,
                        const std::string& className = "DemoClass")
{
    const auto factory = codegen::CreateFactory(language);
    const auto classUnit = factory->CreateClass(className, classFlags);

    for (const auto& field : fields)
    {
        classUnit->Append(factory->CreateField(field.name, field.type, field.flags),
                          field.access | codegen::AccessModifier::Unknown);
    }

    for (const auto& method : methods)
    {
        const auto methodUnit = factory->CreateMethod(method.name, method.returnType, method.flags);
        for (const auto& text : method.printStatements)
        {
            methodUnit->Append(factory->CreatePrintStatement(text), 0);
        }
        classUnit->Append(methodUnit, method.access | codegen::AccessModifier::Unknown);
    }

    return classUnit->Render();
}

inline std::string RenderRegularClass(codegen::Language language)
{
    codegen::ClassModifier classFlags = codegen::ClassModifier::Unknown;

    std::vector<MethodConfig> methods = {
        {"publicMethod", "void", codegen::MethodModifier::Unknown, codegen::AccessModifier::PublicAccess, {}},
        {"privateMethod", "void", codegen::MethodModifier::Unknown, codegen::AccessModifier::PrivateAccess, {}},
        {"protectedMethod", "void", codegen::MethodModifier::Unknown, codegen::AccessModifier::ProtectedAccess, {}},
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
        methods.push_back({"privateProtectedMethod",
                           "void",
                           codegen::MethodModifier::Unknown,
                           codegen::AccessModifier::PrivateProtectedAccess,
                           {}});
        methods.push_back(
            {"internalMethod", "void", codegen::MethodModifier::Unknown, codegen::AccessModifier::InternalAccess, {}});
        methods.push_back({"protectedInternalMethod",
                           "void",
                           codegen::MethodModifier::Unknown,
                           codegen::AccessModifier::ProtectedInternalAccess,
                           {}});
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
                    {{"getValue",
                      "int",
                      codegen::MethodModifier::Unknown,
                      codegen::AccessModifier::PublicAccess,
                      {"Returning immutable value..."}}},
                    {}, "ImmutableData");
    }

    if (IsLanguage(language, codegen::Language::CSharpLanguage))
    {
        return Demo(language, ToFlags(codegen::ClassModifier::FinalModifier),
                    {{"Execute",
                      "void",
                      codegen::MethodModifier::Unknown,
                      codegen::AccessModifier::PublicAccess,
                      {"Sealed class execution..."}}},
                    {}, "SealedImpl");
    }

    return Demo(language, ToFlags(codegen::ClassModifier::FinalModifier),
                {{"doSomething",
                  "void",
                  codegen::MethodModifier::Unknown,
                  codegen::AccessModifier::PublicAccess,
                  {"Final class cannot be derived..."}}},
                {}, "FinalClass");
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
                        {"validateInput",
                         "boolean",
                         codegen::MethodModifier::Unknown,
                         codegen::AccessModifier::PublicAccess,
                         {"Validating..."}},
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

    return Demo(language, codegen::ClassModifier::Unknown,
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
        return Demo(language, codegen::ClassModifier::Unknown,
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
        return Demo(language, codegen::ClassModifier::Unknown,
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

    return Demo(language, codegen::ClassModifier::Unknown,
                {
                    {"calculate",
                     "int",
                     ToFlags(codegen::MethodModifier::StaticModifier),
                     codegen::AccessModifier::PublicAccess,
                     {"Calculating..."}},
                },
                {
                    {"kVersion_", "const char*", ToFlags(codegen::MethodModifier::StaticModifier),
                     codegen::AccessModifier::PublicAccess},
                },
                "Utils");
}

inline std::string RenderIncorrectExamples(codegen::Language language)
{
    const auto factory = codegen::CreateFactory(language);
    std::string result;

    const auto appendFailure = [&result](const std::string& title, const std::string& explanation, const auto& action) {
        result += title + "\n";
        result += explanation + "\n";
        try
        {
            result += action();
        } catch (const std::exception& error)
        {
            result += std::string("Expected failure: ") + error.what() + "\n";
        }
        result += "\n";
    };

    // Universal: empty class name must be rejected by all generators.
    appendFailure("Empty class name:", "Empty class name violates the generator invariant.",
                  [&]() { return Demo(language, codegen::ClassModifier::Unknown, {}, {}, ""); });

    // Universal: field without a type cannot be declared in any language.
    appendFailure("Empty field type:", "A field without a type cannot be declared.", [&]() {
        return Demo(language, codegen::ClassModifier::Unknown, {},
                    {{"missingType", "", codegen::MethodModifier::Unknown, codegen::AccessModifier::PrivateAccess}},
                    "BrokenFieldDemo");
    });

    // Universal: out-of-range class modifier must be rejected by all generators.
    appendFailure("Unsupported class modifier:", "Out-of-range ClassModifier value must be rejected.", [&]() {
        return factory
            ->CreateClass("Broken" + factory->GetLanguageName() + "Demo", static_cast<codegen::ClassModifier>(999))
            ->Render();
    });

    // C++ and Java reject VirtualModifier on a field. C# silently masks it out — no error there.
    if (!IsLanguage(language, codegen::Language::CSharpLanguage))
    {
        appendFailure("Virtual modifier on field:", factory->GetLanguageName() + " does not support virtual fields.",
                      [&]() {
                          return Demo(language, codegen::ClassModifier::Unknown, {},
                                      {{"brokenField", "int", ToFlags(codegen::MethodModifier::VirtualModifier),
                                        codegen::AccessModifier::PrivateAccess}},
                                      "BrokenFieldDemo");
                      });
    }

    return result;
}

}  // namespace examples

#endif
