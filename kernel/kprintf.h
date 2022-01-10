#pragma once

#include <kernel/types.h>

/*
https://github.com/mpaland/printf/blob/master/printf.h
https://github.com/SerenityOS/serenity/blob/3055f73d485c105952416e5a9cb43e47b7816aa3/AK/PrintfImplementation.h
*/

typedef void (*PutChFunc)(char character);

/*
Simple printf-like implementation that takes
a format string and variable argument list, parses
it and send the generated formatted text, character by character,
through to the 'putch' function callback.
*/
void printf_implementation(PutChFunc putch, const char* format, va_list ap);