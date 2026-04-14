/// @file
/// @brief Публичные перечисления генератора кода.
#ifndef GUID_2ad9e5d4_1c48_4bbf_9a49_66a0f923bf3d
#define GUID_2ad9e5d4_1c48_4bbf_9a49_66a0f923bf3d

#include <unit.hpp>

namespace codegen
{

/// @brief Модификатор доступа секции класса.
enum class AccessModifier : CodeUnit::Flags {
    publicAccess,     ///< Публичный модификатор доступа.
    protectedAccess,  ///< Защищённый модификатор доступа.
    privateAccess     ///< Приватный модификатор доступа.
};

/// @brief Модификаторы объявления метода.
enum class MethodModifier : CodeUnit::Flags {
    staticModifier = 1,       ///< Статический метод (бит 0, значение 1).
    constModifier = 1 << 1,   ///< Const метод (бит 1, значение 2).
    virtualModifier = 1 << 2  ///< Виртуальный метод (бит 2, значение 4).
};

/// @brief Вспомогательная функция для преобразования MethodModifier в CodeUnit::Flags.
/// Избавляет от повторяющихся static_cast в коде.
/// @param[in] modifier Модификатор метода для преобразования.
/// @return Битовая маска флагов, соответствующая переданному модификатору.
inline constexpr CodeUnit::Flags ToFlags(MethodModifier modifier) noexcept
{
    return static_cast<CodeUnit::Flags>(modifier);
}

}  // namespace codegen

#endif  // GUID_2ad9e5d4_1c48_4bbf_9a49_66a0f923bf3d