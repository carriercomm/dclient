#pragma once

#if defined(JSON_DLL_BUILD)
#  define JSON_API __declspec(dllexport)
#elif defined(JSON_DLL)
#  define JSON_API __declspec(dllimport)
#else
#  define JSON_API
#endif
