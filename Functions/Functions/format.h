#pragma once

#include <stdarg.h>
#include <stddef.h>

int format_vars_array(char *buffer, char symbol, char lo, char hi, const char *format, size_t argcount, const char **args);
int format_vars_array(char *buffer, const char *format, size_t argcount, const char **args);
int format_va_list(char *buffer, char symbol, char lo, char hi, const char *format, va_list args);
int format_vars_config(char *buffer, char symbol, char lo, char hi, const char *format, ...);
int format_vars(char *buffer, const char *format, ...);

void print_format_xyz(const char *fmt, const char *x, const char *y, const char *z);
void print_format_abc(const char *fmt, const char *a, const char *b, const char *c);

void test_format_abc();
void test_format_xyz_vars();
