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
    Unknown = 0,               ///< Неизвестный модификатор метода.
    StaticModifier = 1 << 0,   ///< Статический метод (бит 0, значение 1).
    ConstModifier = 1 << 1,    ///< Const метод (бит 1, значение 2).
    VirtualModifier = 1 << 2,  ///< Виртуальный метод (бит 2, значение 4).
    FinalModifier = 1 << 3,    ///< Final метод в Java/C# (бит 3, значение 8).
    AbstractModifier = 1 << 4  ///< Abstract метод в Java/C# (бит 4, значение 16).
};

/// @brief Модификаторы объявления класса.
enum class ClassModifier : CodeUnit::Flags {
    Unknown = 0,               ///< Неизвестный модификатор класса.
    FinalModifier = 1 << 5,    ///< Final класс в Java/C# (бит 5, значение 32).
    AbstractModifier = 1 << 6  ///< Abstract класс в Java/C# (бит 6, значение 64).
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