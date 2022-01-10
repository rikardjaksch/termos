#include <kernel/kprintf.h>
#include <kernel/kstdlib.h>

static const char* _hex_digits_lower = "0123456789abcdef";
static const char* _hex_digits_upper = "0123456789ABCDEF";

#define FLAG_LEFT_JUSTIFY   (1U << 0U)
#define FLAG_PLUS           (1U << 1U)
#define FLAG_SPACE          (1U << 1U)
#define FLAG_HASH           (1U << 2U)
#define FLAG_ZERO_PAD       (1U << 3U)
#define FLAG_UPPER_CASE     (1U << 4U)

void print_hex(PutChFunc putch, uint32_t number, uint32_t flags, uint32_t width) {
    // Calculate the amount of digits
    uint32_t printed_chars = 0;
    uint32_t digits = 0;    
    for (uint32_t n = number; n > 0; n >>=4) { ++digits; }
    if (digits == 0) digits = 1;
    
    if ((flags & FLAG_HASH)) {
        putch('0');
        if ((flags & FLAG_UPPER_CASE))
            putch('X');
        else
            putch('x');

        width += 2;
        printed_chars += 2;
    }

    if ((flags & FLAG_ZERO_PAD)) {
        while (printed_chars < width - digits) {
            putch('0');
            printed_chars++;
        }
    }

    if (number == 0) {
        putch('0');
    } else {
        uint8_t shift_count = digits * 4;
        const char* alphabet = (flags & FLAG_UPPER_CASE) ? _hex_digits_upper : _hex_digits_lower;
        while (shift_count) {
            shift_count -= 4;
            putch(alphabet[(number >> shift_count) & 0x0f]);
        }
    }    
}

void print_number(PutChFunc putch, uint32_t number, uint32_t flags, uint32_t width) {
    (void)width;
    (void)flags;
    uint32_t divisor = 1000000000;
    char ch;
    char padding = 1;
    char buf[16];
    char* p = buf;

    // Convert integer number to char array
    for (;;) {
        ch = '0' + (number / divisor);
        number %= divisor;
        if (ch != '0')
            padding = 0;
        if (!padding || divisor == 1)
            *(p++) = ch;
        if (divisor == 1)
            break;
        divisor /= 10;
    }

    uint32_t num_len = p - buf;
    for (uint32_t i = 0; i < num_len; ++i) {
        putch(buf[i]);
    }
}

void print_signed_number(PutChFunc putch, int32_t number, uint32_t flags, uint32_t width) {
    if (number < 0) {
        putch('-');
        print_number(putch, 0 - number, flags, width);
        return;
    }

    if ((flags & FLAG_PLUS)) {
        putch('+');        
    }

    print_number(putch, number, flags, width);
}

void print_string(PutChFunc putch, const char* str, size_t len, uint32_t flags) {
    (void)flags;
    for (size_t i = 0; i < len; ++i) {
        putch(str[i]);
    }
}

void printf_implementation(PutChFunc putch, const char* format, va_list ap) {
    uint32_t flags = 0;
    uint32_t width = 0;
    bool more_flags = false;

    // Iterate over the entire format string, until we reach the end
    // (expecting a null terminated string)
    while (*format) {
        if (*format != '%') {
            putch(*format);
            format++;
            continue;
        } else {
            format++;
        }

        // If we come accross a format specifier, try to parse it.
        // %[flags][width][.precision][length]specifier

        
        // Parse flags field
        flags = 0U;
        do {
            switch (*format) {
                case '-': flags |= FLAG_LEFT_JUSTIFY; format++; more_flags = true;  break;
                case '+': flags |= FLAG_PLUS;         format++; more_flags = true;  break;
                case ' ': flags |= FLAG_SPACE;        format++; more_flags = true;  break;
                case '#': flags |= FLAG_HASH;         format++; more_flags = true;  break;
                case '0': flags |= FLAG_ZERO_PAD;     format++; more_flags = true;  break;
                default:                                        more_flags = false; break;
            }
        } while (more_flags);

        // Parse width field
        width = 0U;
        if (*format >= '0' && *format <= '9') {
            while (*format >= '0' && *format <= '9') {
                width *= 10;
                width += *format - '0';
                format++;
            }            
        } else if (*format == '*') {
            // TODO: Unimplemented for now
            format++;
        }

        // Parse precision field
        // Parse length field
        // Parse specifier
        switch (*format) {
            case 's': {
                const char* str = va_arg(ap, const char*);
                if (!str)
                    str = "(null)";
                print_string(putch, str, strlen(str), flags);
                format++;
                break;
            }

            case 'd': 
            case 'i': {
                int32_t num = va_arg(ap, int32_t);
                print_signed_number(putch, num, flags, width);
                format++;
                break;
            }

            case 'X':
            case 'x': {     
                if (*format == 'X') flags |= FLAG_UPPER_CASE;
                uint32_t num = va_arg(ap, uint32_t);
                print_hex(putch, num, flags, width);
                format++;
                break;
            }
        }
    }
}