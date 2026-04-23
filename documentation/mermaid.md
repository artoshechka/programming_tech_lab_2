```mermaid
---
config:
  flowchart:
    nodeSpacing: 120
    rankSpacing: 120
  layout: elk
  look: classic
---
classDiagram

        class CodeUnit {
            <<abstract>>
            +Flags
            +Append(unit, flagsValue)
            +Append(unit, accessModifier)
            +Render(indentLevel) string
            #MakeIndent(indentLevel) string
        }

        class Language {
            <<enumeration>>
            Unknown
            CppLanguage
            JavaLanguage
            CSharpLanguage
        }

        class ICodeFactory {
            <<interface>>
            +CreateClass(name, flagsValue) CodeUnit
            +CreateClass(name, modifier) CodeUnit
            +CreateMethod(name, returnType, flagsValue) CodeUnit
            +CreateMethod(name, returnType, modifier) CodeUnit
            +CreateField(name, type, flagsValue) CodeUnit
            +CreateField(name, type, modifier) CodeUnit
            +CreatePrintStatement(text) CodeUnit
            +GetLanguageName() string
        }

        class CreateFactory {
            +CreateFactory(language) ICodeFactory
        }

        class AbstractClassUnit {
            <<abstract>>
            #GetClassName() string
            #GetClassFlags() Flags
        }

        class AbstractMethodUnit {
            <<abstract>>
            +Append(unit, flagsValue)
            +Render(indentLevel) string
            #RenderPrefixModifiers() string
            #RenderSuffixModifiers() string
            #IsAbstractMethod() bool
            #RenderAbstractTerminator() string
            #GetMethodFlags() Flags
        }

        class AbstractPrintUnit {
            <<abstract>>
            +Render(indentLevel) string
            #RenderPrintExpression(text) string
            #GetPrintText() string
        }

        class AbstractFieldUnit {
            <<abstract>>
            +Render(indentLevel) string
            #RenderPrefixModifiers() string
            #RenderSuffixModifiers() string
            #GetFieldFlags() Flags
        }

        class AccessControlledUnit {
            +Render(indentLevel) string
        }


    namespace codegen_cpp {
        class CppCodeFactory {
            +CreateClass(name, flagsValue) CodeUnit
            +CreateMethod(name, returnType, flagsValue) CodeUnit
            +CreateField(name, type, flagsValue) CodeUnit
            +CreatePrintStatement(text) CodeUnit
            +GetLanguageName() string
        }
        
        class CppClassUnit {
            +Append(unit, accessModifier)
            +Append(unit, flagsValue)
            +Render(indentLevel) string
        }

        class CppMethodUnit {
            #RenderPrefixModifiers() string
            #RenderSuffixModifiers() string
            #IsAbstractMethod() bool
            #RenderAbstractTerminator() string
        }

        class CppPrintUnit {
            #RenderPrintExpression(text) string
        }

        class CppFieldUnit {
            #RenderPrefixModifiers() string
        }
    }

    namespace codegen_java {
        class JavaCodeFactory {
            +CreateClass(name, flagsValue) CodeUnit
            +CreateMethod(name, returnType, flagsValue) CodeUnit
            +CreateField(name, type, flagsValue) CodeUnit
            +CreatePrintStatement(text) CodeUnit
            +GetLanguageName() string
        }

        class JavaClassUnit {
            +Append(unit, flagsValue)
            +Render(indentLevel) string
        }

        class JavaMethodUnit {
            #RenderPrefixModifiers() string
            #IsAbstractMethod() bool
            #RenderAbstractTerminator() string
        }

        class JavaPrintUnit {
            #RenderPrintExpression(text) string
        }

        class JavaFieldUnit {
            #RenderPrefixModifiers() string
        }
    }

    namespace codegen_csharp {
        class CSharpCodeFactory {
            +CreateClass(name, flagsValue) CodeUnit
            +CreateMethod(name, returnType, flagsValue) CodeUnit
            +CreateField(name, type, flagsValue) CodeUnit
            +CreatePrintStatement(text) CodeUnit
            +GetLanguageName() string
        }

        class CSharpClassUnit {
            +Append(unit, flagsValue)
            +Render(indentLevel) string
        }

        class CSharpMethodUnit {
            #RenderPrefixModifiers() string
            #IsAbstractMethod() bool
            #RenderAbstractTerminator() string
        }

        class CSharpPrintUnit {
            #RenderPrintExpression(text) string
        }

        class CSharpFieldUnit {
            #RenderPrefixModifiers() string
        }
    }

    ICodeFactory <|.. CppCodeFactory
    ICodeFactory <|.. JavaCodeFactory
    ICodeFactory <|.. CSharpCodeFactory

    CodeUnit <|-- AbstractClassUnit
    CodeUnit <|-- AbstractMethodUnit
    CodeUnit <|-- AbstractPrintUnit
    CodeUnit <|-- AbstractFieldUnit

    AbstractClassUnit <|-- CppClassUnit
    AbstractMethodUnit <|-- CppMethodUnit
    AbstractPrintUnit <|-- CppPrintUnit
    AbstractFieldUnit <|-- CppFieldUnit

    AbstractClassUnit <|-- JavaClassUnit
    AbstractMethodUnit <|-- JavaMethodUnit
    AbstractPrintUnit <|-- JavaPrintUnit
    AbstractFieldUnit <|-- JavaFieldUnit

    AbstractClassUnit <|-- CSharpClassUnit
    AbstractMethodUnit <|-- CSharpMethodUnit
    AbstractPrintUnit <|-- CSharpPrintUnit
    AbstractFieldUnit <|-- CSharpFieldUnit

    CodeUnit <|-- AccessControlledUnit

    CreateFactory --> Language
    CreateFactory --> ICodeFactory

    CSharpCodeFactory --> CSharpClassUnit
    CSharpCodeFactory --> CSharpMethodUnit
    CSharpCodeFactory --> CSharpFieldUnit
    CSharpCodeFactory --> CSharpPrintUnit

    CppCodeFactory --> CppClassUnit
    CppCodeFactory --> CppMethodUnit
    CppCodeFactory --> CppFieldUnit
    CppCodeFactory --> CppPrintUnit

    JavaCodeFactory --> JavaClassUnit
    JavaCodeFactory --> JavaMethodUnit
    JavaCodeFactory --> JavaFieldUnit
    JavaCodeFactory --> JavaPrintUnit

    JavaClassUnit --> AccessControlledUnit
    CSharpClassUnit --> AccessControlledUnit
```