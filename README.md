# Лабораторная работа по предмету: "Технологии программирования"
## Тема: "Абстрактная фабрика"
> 4 курс 2 семестр \
> Студент группы 932223 - **Артеменко Антон Дмитриевич** 

## 1. Постановка задачи
Необходимо расширить реализацию генератора программ, чтобы обеспечить поддержку нескольких языков программирования в рамках одной архитектуры:
- C++
- C#
- Java

https://disk.yandex.ru/i/dtd6RCsC1FCtcg
Сгенерированные программы должны:
- корректно формироваться для выбранного языка;
- компилироваться без ошибок средствами соответствующего языка.

В рамках лабораторной работы требуется:
- добавить модификаторы классов и методов, отсутствующие в C++, но присутствующие в C# и Java;
- для C# ориентироваться на материалы: https://metanit.com/sharp/tutorial/3.2.php;
- для Java ориентироваться на материалы: http://proglang.su/java/modifiers (до п. 3.4 включительно);
- не включать модификаторы Java: synchronized, transient, volatile.

Для решения задачи использовать паттерн проектирования «Абстрактная фабрика».

## 2. Предлагаемое решение
### Зависимости проекта
В проекте используется:
- **CMake** v3.12
- **Стандарт C++** 17

### UML-диаграмма классов
```mermaid
%%{init: {'flowchart': {'nodeSpacing': 120, 'rankSpacing': 120}}}%%
classDiagram
    namespace core {
        class CodeUnit {
            <<abstract>>
            +Flags
            +Append(unit, flagsValue)
            +Render(indentLevel) string
            #MakeIndent(indentLevel) string
        }

        class ICodeFactory {
            <<interface>>
            +CreateClass(name, flagsValue) CodeUnit
            +CreateMethod(name, returnType, flagsValue) CodeUnit
            +CreatePrintStatement(text) CodeUnit
            +GetLanguageName() string
        }

    }

    namespace cpp {
        class CppCodeFactory {
            +CreateClass(name, flagsValue) CodeUnit
            +CreateMethod(name, returnType, flagsValue) CodeUnit
            +CreatePrintStatement(text) CodeUnit
            +GetLanguageName() string
        }

        class ClassDeclarationUnit {
            +Append(unit, accessModifier)
            +Append(unit, flagsValue)
            +Render(indentLevel) string
        }

        class MethodDeclarationUnit {
            +Append(unit, flagsValue)
            +Render(indentLevel) string
        }

        class PrintStatementUnit {
            +Render(indentLevel) string
        }
    }

    namespace java {
        class JavaCodeFactory {
            +CreateClass(name, flagsValue) CodeUnit
            +CreateMethod(name, returnType, flagsValue) CodeUnit
            +CreatePrintStatement(text) CodeUnit
            +GetLanguageName() string
        }

        class JavaClassUnit {
            +Append(unit, flagsValue)
            +Render(indentLevel) string
        }

        class JavaMethodUnit {
            +Append(unit, flagsValue)
            +Render(indentLevel) string
        }

        class JavaPrintUnit {
            +Render(indentLevel) string
        }
    }

    namespace csharp {
        class CSharpCodeFactory {
            +CreateClass(name, flagsValue) CodeUnit
            +CreateMethod(name, returnType, flagsValue) CodeUnit
            +CreatePrintStatement(text) CodeUnit
            +GetLanguageName() string
        }

        class CSharpClassUnit {
            +Append(unit, flagsValue)
            +Render(indentLevel) string
        }

        class CSharpMethodUnit {
            +Append(unit, flagsValue)
            +Render(indentLevel) string
        }

        class CSharpPrintUnit {
            +Render(indentLevel) string
        }
    }

    namespace common {
        class AccessControlledUnit {
            +Render(indentLevel) string
        }
    }

    ICodeFactory <|.. CppCodeFactory
    ICodeFactory <|.. JavaCodeFactory
    ICodeFactory <|.. CSharpCodeFactory

    CodeUnit <|-- ClassDeclarationUnit
    CodeUnit <|-- MethodDeclarationUnit
    CodeUnit <|-- PrintStatementUnit

    CodeUnit <|-- JavaClassUnit
    CodeUnit <|-- JavaMethodUnit
    CodeUnit <|-- JavaPrintUnit

    CodeUnit <|-- CSharpClassUnit
    CodeUnit <|-- CSharpMethodUnit
    CodeUnit <|-- CSharpPrintUnit

    CodeUnit <|-- AccessControlledUnit

    CppCodeFactory ..> ClassDeclarationUnit
    CppCodeFactory ..> MethodDeclarationUnit
    CppCodeFactory ..> PrintStatementUnit

    JavaCodeFactory ..> JavaClassUnit
    JavaCodeFactory ..> JavaMethodUnit
    JavaCodeFactory ..> JavaPrintUnit

    CSharpCodeFactory ..> CSharpClassUnit
    CSharpCodeFactory ..> CSharpMethodUnit
    CSharpCodeFactory ..> CSharpPrintUnit

    AccessControlledUnit <|-- CSharpClassUnit
    AccessControlledUnit <|-- JavaClassUnit
```

### Архитектура решения
Основные компоненты:
- **main.cpp** — точка входа демонстрации. Создает фабрики для C++, Java и C#, генерирует примеры классов и выводит результат в консоль.
- **code_factory** — контракт абстрактной фабрики:
    - **ICodeFactory** — общий интерфейс создания узлов (`CreateClass`, `CreateMethod`, `CreatePrintStatement`);
    - **CreateFactory(Language)** — выбор конкретной фабрики по целевому языку.
- **unit / CodeUnit** — базовая иерархия узлов генерации:
    - **CodeUnit** — абстрактный базовый узел с `Append` и `Render`;
    - конкретные узлы классов, методов и печати для каждого языка.
- **unit/src/cpp** — C++-реализация генерации:
    - **CppCodeFactory**;
    - **ClassDeclarationUnit**, **MethodDeclarationUnit**, **PrintStatementUnit**.
- **unit/src/java** — Java-реализация генерации:
    - **JavaCodeFactory**;
    - **JavaClassUnit**, **JavaMethodUnit**, **JavaPrintUnit**.
- **unit/src/csharp** — C#-реализация генерации:
    - **CSharpCodeFactory**;
    - **CSharpClassUnit**, **CSharpMethodUnit**, **CSharpPrintUnit**.
- **unit/codegen_types.hpp** — общие перечисления модификаторов:
    - **AccessModifier** (включая C#-специфичные значения);
    - **MethodModifier** (`static`, `virtual`, `const`, `final`, `abstract`);
    - **ClassModifier** (`final`, `abstract`).

## 3. Инструкция для пользователя
Сборка проекта производится следующим образом:

<details>
<summary>Windows</summary>

Создайте директорию `build` и перейдите в нее:
```powershell
mkdir build
cd build
```

Сконфигурируйте и соберите проект:
```powershell
cmake .. && cmake --build .
```
Запустите программу:
```powershell
./program_generator
```

</details>

<details>
<summary>Linux / macOS</summary>

Создайте директорию `build` и перейдите в нее:
```bash
mkdir -p build && cd build
```

Сконфигурируйте и соберите проект:
```bash
cmake ..
cmake --build .
```

Запустите программу:
```bash
./program_generator
```

</details>

### Форматирование кода

Для автоматического форматирования всех исходных файлов используйте команду:

```bash
find . -name "*.cpp" -o -name "*.hpp" | grep -v "/build/" | xargs clang-format -i
```
