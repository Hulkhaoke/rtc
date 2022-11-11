#pragma once

#ifdef _WIN32
#ifdef ZT_DLL_EXPORTS
#define ZT_API __declspec(dllexport)
#else
#define ZT_API __declspec(dllimport)
#endif
#elif __linux__
#define ZT_API
#endif


#ifdef  __cplusplus
extern "C" {
#endif

	ZT_API void ZtInit();
    ZT_API void ZtSend();
    ZT_API void ZtReceive();

#ifdef  __cplusplus
}
#endif
