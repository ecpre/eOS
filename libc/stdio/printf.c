#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef enum states {
	INIT_S,
	LEN_S,
	SHORT_S,
	LONG_S,
	FIN_S
} state_t;

typedef enum lengths {
	DEFAULT_L,
	SHORT_SHORT_L,
	SHORT_L,
	LONG_L,
	LONG_LONG_L
} length_t;

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

char hexconv[] = "0123456789abcdef";

int intprint(bool sign, uint64_t num, int base) {
	if (sign) {
		int64_t* nnum = (int64_t*) &num;
		if (*nnum<0) {
			putchar('-');
			return 1 + intprint(false, num, base);
		}
	}
	char buf[32];
	char printbuf[32];
	int digit = 0;
	do {
		uint64_t remainder = num % base;
		num /= base;
		buf[digit] = hexconv[remainder];
		digit++;
	} while (num > 0);
	for (int i = 0; i<digit; i++) {
		printbuf[i] = buf[digit-i-1];
	}
	print(printbuf, digit);
	return digit;
}

int num_write(bool sign, int base, length_t length, va_list* args) {
	switch(length) {
	case SHORT_SHORT_L:
	case SHORT_L:
	case DEFAULT_L:
		if (sign)
			return intprint(sign, va_arg(*args, int), base);
		else return intprint(sign, va_arg(*args, unsigned int), base);
	case LONG_L:
		if (sign)
			return intprint(sign, va_arg(*args, long), base);
		else return intprint(sign, va_arg(*args, unsigned long), base);
	case LONG_LONG_L:
		if (sign)
			return intprint(sign, va_arg(*args, long long), base);
		else return intprint(sign, va_arg(*args, unsigned long long), base);
	}
	return 0;
}

int printf(const char* format, ...) {
	va_list args;
	va_start(args, format);

	int written = 0;
	
	state_t state = INIT_S;
	length_t length = DEFAULT_L;

	while (*format) {
		switch(state) {
		case INIT_S:
			switch(*format) {
			case '%':	state = LEN_S;
					format++;
					break;
			default:	putchar(*format);
					format++;
					written++;
					break;
			}
			break;
		case LEN_S:
			switch(*format) {
			case 'h':	state = SHORT_S;
					length = SHORT_L;
					format++;
					break;
			case 'l':	state = LONG_S;
					length = LONG_L;
					format++;
					break;
			default:	state = FIN_S;
					break;

			}
			break;
		case SHORT_S:
			switch(*format) {
			case 'h':	length = SHORT_SHORT_L;
					format++;
					__attribute__((fallthrough));
			default:	state = FIN_S;
					break;
			}
			break;
		case LONG_S:
			switch(*format) {
			case 'l':	length = SHORT_SHORT_L;
					format++;
					__attribute__((fallthrough));
			default:	state = FIN_S;
					break;
			}
			break;
		case FIN_S:
			switch(*format) {
			case 'c':	char c = (char) va_arg(args, int);
					putchar(c);
					format++;
					written++;
					break;
			case 's':	const char* str = va_arg(args, char*);
					size_t len = strlen(str);
					print(str, len);
					format++;
					written+=len;
					break;
			case 'i':
			case 'd':	written += num_write(true, 10, length, &args);
					format++;
					break;
			case 'u':	written += num_write(false, 10, length, &args);
					format++;
					break;
			case 'x':
			case 'p':	print("0x", 2);
					written += (2 + num_write(false, 16, length, &args));
					format++;
					break;
			case 'o':	print("0o", 2);
					written += (2 + num_write(false, 8, length, &args));
					format++;
					break;
			case '%':	putchar('%');
					written++;
					format++;
					break;
					// double break so i don't forget when adding new format specs
			default:	break;

			}
			state = INIT_S;
			length = DEFAULT_L;
			break;
		}
	}

	va_end(args);
	return written;
}
