#pragma once
struct RValue;
struct CInstance;
using fnGML = void(__cdecl*)(RValue* Result, CInstance* pSelfInst, CInstance* pOtherInst, int argc, RValue* pArgs);