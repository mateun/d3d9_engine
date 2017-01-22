#pragma once
#ifdef D3D9_ENGINE_EXPORTS
#define D3D9_ENGINE_API __declspec (dllexport)
#else
#define D3D9_ENGINE_API __declspec (dllimport)
#endif