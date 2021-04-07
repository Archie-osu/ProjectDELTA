#pragma once
#include <Windows.h>
#include "ghlib.hpp"
#include <variant>
//Useful functions I didn't bother to put anywhere else.
namespace Useful
{
	BOOL __stdcall EnumWndCallback(HWND hwnd, LPARAM);

	HWND GetProcessWindow();

	inline HWND g_GameWindow; //Don't use, use GetProcessWindow() instead!kdy
}


//Includes reversed classes
namespace Game
{
	enum Kinds_t : int
	{
		KIND_REAL = 0,
		KIND_STRING = 1,
		KIND_ARRAY = 2,
		KIND_POINTER = 3,
		KIND_VECTOR3 = 4,
		KIND_UNDEFINED = 5,
		KIND_OBJECT = 6,
		KIND_INT32 = 7,
		KIND_VECTOR4 = 8,
		KIND_MATRIX4 = 9,
		KIND_INT64 = 10,
		KIND_ACCESSOR = 11,
		KIND_JSNULL = 12,
		KIND_BOOL = 13, //Handled the same way as KIND_REAL
		KIND_ITERATOR = 14,
		KIND_REFERENCE = 15,
		KIND_UNSET = 0xFFFFFF
	};

	//Thanks to https://github.com/nkrapivin for helping with this
	struct YYCVar
	{
		union
		{
			double dValue; //Real
			__int32 iValue; //Int32
			__int64 llValue; //Int64
			void* pValue; //Pointer
		}; //0x08
		
		__int32 unknown; //0xC
		Kinds_t eKind; //0x10

		YYCVar() : dValue(0.0) {}

		YYCVar(double v)
		{
			this->SetValue(v);
		}

		YYCVar(__int32 v)
		{
			this->SetValue(v);
		}

		YYCVar(__int64 v)
		{
			this->SetValue(v);
		}

		YYCVar(void* pv)
		{
			this->SetValue(pv);
		}

		void SetValue(double v)
		{
			this->dValue = v;
			this->eKind = KIND_REAL;
		}

		void SetValue(__int64 v)
		{
			this->llValue = v;
			this->eKind = KIND_INT64;
		}

		void SetValue(__int32 v)
		{
			this->iValue = v;
			this->eKind = KIND_INT32;
		}

		void SetValue(void* pv)
		{
			this->pValue = pv;
			this->eKind = KIND_POINTER;
		}

		template <typename T>
		T& Value()
		{
			switch (this->eKind)
			{
				case KIND_REAL: /* Fallthrough */
				case KIND_BOOL:
					return (T&)this->dValue;
				case KIND_INT32:
					return (T&)this->iValue;
				case KIND_INT64:
					return (T&)this->llValue;
				case KIND_POINTER:
					return (T&)this->pValue;
				default:
					T ret = (T)0;
					return (T&)ret;
			}
		}

		const char* Kind()
		{
			switch (this->eKind)
			{
				case KIND_REAL: /* Fallthrough */
				case KIND_BOOL:
					return "Real";
				case KIND_INT32:
					return "Int32";
				case KIND_INT64:
					return "Int64";
				case KIND_POINTER:
					return "Pointer";
				case KIND_UNDEFINED:
					return "Undefined";
				case KIND_UNSET:
					return "Unset";
				case KIND_ARRAY:
					return "Array";
				case KIND_ACCESSOR:
					return "Accessor";
				case KIND_STRING:
					return "String";
				case KIND_OBJECT:
					return "Object";
				default:
					return "<UNK>";
			}
		}
	};

	typedef struct _CCrewData
	{
		YYCVar Kris_MaxHP; //ebp+0x10
		YYCVar Susie_MaxHP; //ebp+0x20
		YYCVar Ralsei_MaxHP; //ebp+0x30
		YYCVar Unknown; //ebp+40
		YYCVar Kris_CurHP; //ebp+50
		YYCVar Susie_CurHP; //ebp+60
		YYCVar Ralsei_CurHP; //ebp+70
	} CCrewData;

	typedef struct _CGlobals
	{
		YYCVar Interact; //ecx+10
		YYCVar Unknown; //ecx+20
		YYCVar Unknown2; //ecx+30
		YYCVar DebugMode; //ecx+40
	} CGlobals;
}

namespace Globals
{
	inline ghl::ptr_t pCrewData; //Cast to CCrewData, holds information about the crew (HP...)
	inline ghl::ptr_t nRoomNumber; //Cast to __int32 (int), holds the current room number.
	inline ghl::ptr_t pGlobalVars; //Cast to CGlobals, holds information like Interact, etc.
}
