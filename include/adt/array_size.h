#pragma once

template<typename T, unsigned long N>
char (&array_size (T const (&)[N]))[N];

#define array_size(array) (sizeof array_size (array))
