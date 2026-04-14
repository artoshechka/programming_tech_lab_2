/// @file
/// @brief Публичные перечисления генератора кода.
#ifndef GUID_2ad9e5d4_1c48_4bbf_9a49_66a0f923bf3d
#define GUID_2ad9e5d4_1c48_4bbf_9a49_66a0f923bf3d

#include <unit.hpp>

namespace codegen
{

/// @brief Модификаторы доступа для класса/структуры и членов.
enum class AccessModifier : CodeUnit::Flags {
    publicAccess,             ///< Публичный модификатор доступа.
    protectedAccess,          ///< Защищённый модификатор доступа.
    privateAccess,            ///< Приватный модификатор доступа.
    privateProtectedAccess,   ///< Доступ из своего класса и наследников в той же сборке (C#).
    internalAccess,           ///< Доступ из текущей сборки (C#).
    protectedInternalAccess,  ///< Доступ из текущей сборки или из наследников (C#).
    fileAccess                ///< Доступ только в пределах файла для типа (C# 11).
};

/// @brief Модификаторы объявления метода.
enum class MethodModifier : CodeUnit::Flags {
    staticModifier = 1,       ///< Статический метод (бит 0, значение 1).
    constModifier = 1 << 1,   ///< Const метод (бит 1, значение 2).
    virtualModifier = 1 << 2, ///< Виртуальный метод (бит 2, значение 4).
    finalModifier = 1 << 3,   ///< Final метод в Java/C# (бит 3, значение 8).
    abstractModifier = 1 << 4 ///< Abstract метод в Java/C# (бит 4, значение 16).
};

/// @brief Модификаторы объявления класса.
enum class ClassModifier : CodeUnit::Flags {
    finalModifier = 1 << 5,      ///< Final класс в Java/C# (бит 5, значение 32).
    abstractModifier = 1 << 6    ///< Abstract класс в Java/C# (бит 6, значение 64).
};

/// @brief Вспомогательная функция для преобразования MethodModifier в CodeUnit::Flags.
/// Избавляет от повторяющихся static_cast в коде.
/// @param[in] modifier Модификатор метода для преобразования.
/// @return Битовая маска флагов, соответствующая переданному модификатору.
inline constexpr CodeUnit::Flags ToFlags(MethodModifier modifier) noexcept
{
    return static_cast<CodeUnit::Flags>(modifier);
}

/// @brief Вспомогательная функция для преобразования ClassModifier в CodeUnit::Flags.
/// Избавляет от повторяющихся static_cast в коде.
/// @param[in] modifier Модификатор класса для преобразования.
/// @return Битовая маска флагов, соответствующая переданному модификатору.
inline constexpr CodeUnit::Flags ToFlags(ClassModifier modifier) noexcept
{
    return static_cast<CodeUnit::Flags>(modifier);
}

}  // namespace codegen

#endif  // GUID_2ad9e5d4_1c48_4bbf_9a49_66a0f923bf3d