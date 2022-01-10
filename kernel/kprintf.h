#pragma once

#include <kernel/types.h>

/*
https://github.com/mpaland/printf/blob/master/printf.h
https://github.com/SerenityOS/serenity/blob/3055f73d485c105952416e5a9cb43e47b7816aa3/AK/PrintfImplementation.h
*/

int kprintf(
    // An output function which takes on character and an argument pointer
    void(*out)(char character, void* arg),
    // An argument pointer for user data passed to output function
    void* user_data,
    // A string that specifies the format of the output
    const char* format,
    ...);