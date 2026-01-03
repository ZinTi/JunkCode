#ifndef USER_TYPE_T_H
#define USER_TYPE_T_H

#include <string>
#include <cstdint>

enum class user_type_t : uint8_t {
    ADMIN = 0,
    USER = 1,
    GUEST = 2,
    INVALID = 3
};

#define USER_TYPE_ADMIN     "ADMIN"
#define USER_TYPE_USER      "USER"
#define USER_TYPE_GUEST     "GUEST"
#define USER_TYPE_INVALID   "INVALID"

#endif
