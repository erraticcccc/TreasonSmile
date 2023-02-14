#pragma once
#include "includes.h"

DWORD GetProcId(const wchar_t* ProcName);

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);


