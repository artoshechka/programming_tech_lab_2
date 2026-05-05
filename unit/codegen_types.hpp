/// @file
/// @brief Публичные перечисления генератора кода.
#ifndef GUID_2ad9e5d4_1c48_4bbf_9a49_66a0f923bf3d
#define GUID_2ad9e5d4_1c48_4bbf_9a49_66a0f923bf3d

#include <unit.hpp>

namespace codegen
{

/// @brief Модификаторы доступа для класса/структуры и членов.
enum class AccessModifier : CodeUnit::Flags {
    Unknown = 0,               ///< Неизвестный модификатор доступа.
    PublicAccess = 1 << 0,     ///< Публичный модификатор доступа.
    ProtectedAccess = 1 << 1,  ///< Защищённый модификатор доступа.
    PrivateAccess = 1 << 2,    ///< Приватный модификатор доступа.
    PrivateProtectedAccess =
        PrivateAccess | ProtectedAccess,  ///< Доступ из своего класса и наследников в той же сборке (C#).
    InternalAccess = 1 << 4,              ///< Доступ из текущей сборки (C#).
    ProtectedInternalAccess = ProtectedAccess | InternalAccess,  ///< Доступ из текущей сборки или из наследников (C#).
    FileAccess = 1 << 6                                          ///< Доступ только в пределах файла для типа (C# 11).
};

/// @brief Модификаторы объявления метода.
enum class MethodModifier : CodeUnit::Flags {
    Unknown = 0,                                                 ///< Неизвестный модификатор метода.
    StaticModifier = 1 << 0,                                     ///< Статический метод (бит 0, значение 1).
    ConstModifier = 1 << 1,                                      ///< Const метод (бит 1, значение 2).
    VirtualModifier = 1 << 2,                                    ///< Виртуальный метод (бит 2, значение 4).
    FinalModifier = 1 << 3,                                      ///< Final метод в Java/C# (бит 3, значение 8).
    AbstractModifier = 1 << 4,                                   ///< Abstract метод в Java/C# (бит 4, значение 16).
    StaticConstModifier = StaticModifier | ConstModifier,        ///< Комбинация static + const.
    StaticVirtualModifier = StaticModifier | VirtualModifier,    ///< Комбинация static + virtual.
    FinalConstModifier = FinalModifier | ConstModifier,          ///< Комбинация final + const.
    StaticFinalModifier = StaticModifier | FinalModifier,        ///< Комбинация static + final.
    StaticAbstractModifier = StaticModifier | AbstractModifier,  ///< Комбинация static + abstract.
    FinalAbstractModifier = FinalModifier | AbstractModifier,    ///< Комбинация final + abstract.
    StaticFinalAbstractModifier =
        StaticModifier | FinalModifier | AbstractModifier  ///< Комбинация static + final + abstract.
};

/// @brief Модификаторы объявления класса.
enum class ClassModifier : CodeUnit::Flags {
    Unknown = 0,                                              ///< Неизвестный модификатор класса.
    FinalModifier = 1 << 7,                                   ///< Final класс в Java/C# (бит 7, значение 128).
    AbstractModifier = 1 << 8,                                ///< Abstract класс в Java/C# (бит 8, значение 256).
    AbstractFinalModifier = AbstractModifier | FinalModifier  ///< Комбинация abstract + final.
};

/// @brief Преобразует набор битов в enum-маску модификаторов доступа.
inline constexpr AccessModifier ToAccessModifierMask(CodeUnit::Flags flags) noexcept
{
    return static_cast<AccessModifier>(flags);
}

/// @brief Преобразует набор битов в enum-маску модификаторов метода.
inline constexpr MethodModifier ToMethodModifierMask(CodeUnit::Flags flags) noexcept
{
    return static_cast<MethodModifier>(flags);
}

/// @brief Преобразует enum-маску модификаторов метода в саму себя.
inline constexpr MethodModifier ToMethodModifierMask(MethodModifier flags) noexcept
{
    return flags;
}

/// @brief Преобразует набор битов в enum-маску модификаторов класса.
inline constexpr ClassModifier ToClassModifierMask(CodeUnit::Flags flags) noexcept
{
    return static_cast<ClassModifier>(flags);
}

/// @brief Преобразует enum-маску модификаторов класса в саму себя.
inline constexpr ClassModifier ToClassModifierMask(ClassModifier flags) noexcept
{
    return flags;
}

/// @brief Побитовое ИЛИ для флагов доступа.
inline constexpr CodeUnit::Flags operator|(AccessModifier lhs, AccessModifier rhs) noexcept
{
    return static_cast<CodeUnit::Flags>(lhs) | static_cast<CodeUnit::Flags>(rhs);
}

/// @brief Побитовое И для флагов доступа.
inline constexpr CodeUnit::Flags operator&(AccessModifier lhs, AccessModifier rhs) noexcept
{
    return static_cast<CodeUnit::Flags>(lhs) & static_cast<CodeUnit::Flags>(rhs);
}

/// @brief Побитовое ИЛИ между битовой маской и флагом доступа.
inline constexpr CodeUnit::Flags operator|(CodeUnit::Flags lhs, AccessModifier rhs) noexcept
{
    return lhs | static_cast<CodeUnit::Flags>(rhs);
}

/// @brief Побитовое И между битовой маской и флагом доступа.
inline constexpr CodeUnit::Flags operator&(CodeUnit::Flags lhs, AccessModifier rhs) noexcept
{
    return lhs & static_cast<CodeUnit::Flags>(rhs);
}

/// @brief Побитовое ИЛИ между флагом доступа и битовой маской.
inline constexpr CodeUnit::Flags operator|(AccessModifier lhs, CodeUnit::Flags rhs) noexcept
{
    return static_cast<CodeUnit::Flags>(lhs) | rhs;
}

/// @brief Побитовое И между флагом доступа и битовой маской.
inline constexpr CodeUnit::Flags operator&(AccessModifier lhs, CodeUnit::Flags rhs) noexcept
{
    return static_cast<CodeUnit::Flags>(lhs) & rhs;
}

/// @brief Побитовое ИЛИ для флагов методов.
inline constexpr MethodModifier operator|(MethodModifier lhs, MethodModifier rhs) noexcept
{
    return static_cast<MethodModifier>(static_cast<CodeUnit::Flags>(lhs) | static_cast<CodeUnit::Flags>(rhs));
}

/// @brief Побитовое И для флагов методов.
inline constexpr CodeUnit::Flags operator&(MethodModifier lhs, MethodModifier rhs) noexcept
{
    return static_cast<CodeUnit::Flags>(lhs) & static_cast<CodeUnit::Flags>(rhs);
}

/// @brief Побитовое ИЛИ между битовой маской и флагом метода.
inline constexpr CodeUnit::Flags operator|(CodeUnit::Flags lhs, MethodModifier rhs) noexcept
{
    return lhs | static_cast<CodeUnit::Flags>(rhs);
}

/// @brief Побитовое И между битовой маской и флагом метода.
inline constexpr CodeUnit::Flags operator&(CodeUnit::Flags lhs, MethodModifier rhs) noexcept
{
    return lhs & static_cast<CodeUnit::Flags>(rhs);
}

/// @brief Побитовое ИЛИ между флагом метода и битовой маской.
inline constexpr CodeUnit::Flags operator|(MethodModifier lhs, CodeUnit::Flags rhs) noexcept
{
    return static_cast<CodeUnit::Flags>(lhs) | rhs;
}

/// @brief Побитовое И между флагом метода и битовой маской.
inline constexpr CodeUnit::Flags operator&(MethodModifier lhs, CodeUnit::Flags rhs) noexcept
{
    return static_cast<CodeUnit::Flags>(lhs) & rhs;
}

/// @brief Побитовое ИЛИ для флагов классов.
inline constexpr ClassModifier operator|(ClassModifier lhs, ClassModifier rhs) noexcept
{
    return static_cast<ClassModifier>(static_cast<CodeUnit::Flags>(lhs) | static_cast<CodeUnit::Flags>(rhs));
}

/// @brief Побитовое И для флагов классов.
inline constexpr CodeUnit::Flags operator&(ClassModifier lhs, ClassModifier rhs) noexcept
{
    return static_cast<CodeUnit::Flags>(lhs) & static_cast<CodeUnit::Flags>(rhs);
}

/// @brief Побитовое ИЛИ между битовой маской и флагом класса.
inline constexpr CodeUnit::Flags operator|(CodeUnit::Flags lhs, ClassModifier rhs) noexcept
{
    return lhs | static_cast<CodeUnit::Flags>(rhs);
}

/// @brief Побитовое И между битовой маской и флагом класса.
inline constexpr CodeUnit::Flags operator&(CodeUnit::Flags lhs, ClassModifier rhs) noexcept
{
    return lhs & static_cast<CodeUnit::Flags>(rhs);
}

/// @brief Побитовое ИЛИ между флагом класса и битовой маской.
inline constexpr CodeUnit::Flags operator|(ClassModifier lhs, CodeUnit::Flags rhs) noexcept
{
    return static_cast<CodeUnit::Flags>(lhs) | rhs;
}

/// @brief Побитовое И между флагом класса и битовой маской.
inline constexpr CodeUnit::Flags operator&(ClassModifier lhs, CodeUnit::Flags rhs) noexcept
{
    return static_cast<CodeUnit::Flags>(lhs) & rhs;
}

/// @brief Проверяет наличие любого бита из маски в наборе флагов.
inline constexpr bool HasFlag(CodeUnit::Flags flags, CodeUnit::Flags mask) noexcept
{
    return (flags & mask) != 0;
}

/// @brief Проверяет наличие флага доступа в наборе флагов.
inline constexpr bool HasFlag(CodeUnit::Flags flags, AccessModifier flag) noexcept
{
    return HasFlag(flags, static_cast<CodeUnit::Flags>(flag));
}

/// @brief Проверяет наличие флага метода в наборе флагов.
inline constexpr bool HasFlag(CodeUnit::Flags flags, MethodModifier flag) noexcept
{
    return HasFlag(flags, static_cast<CodeUnit::Flags>(flag));
}

/// @brief Проверяет наличие флага метода в enum-маске.
inline constexpr bool HasFlag(MethodModifier flags, MethodModifier flag) noexcept
{
    return HasFlag(static_cast<CodeUnit::Flags>(flags), flag);
}

/// @brief Проверяет наличие флага класса в наборе флагов.
inline constexpr bool HasFlag(CodeUnit::Flags flags, ClassModifier flag) noexcept
{
    return HasFlag(flags, static_cast<CodeUnit::Flags>(flag));
}

/// @brief Проверяет наличие флага класса в enum-маске.
inline constexpr bool HasFlag(ClassModifier flags, ClassModifier flag) noexcept
{
    return HasFlag(static_cast<CodeUnit::Flags>(flags), flag);
}

}  // namespace codegen

#endif  // GUID_2ad9e5d4_1c48_4bbf_9a49_66a0f923bf3d