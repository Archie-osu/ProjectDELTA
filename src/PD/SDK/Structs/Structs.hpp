#pragma once
#include <guiddef.h>

enum RVKinds
{
	RV_Real,
	RV_String,
	RV_Array,
	RV_Pointer,
	RV_Vector3,
	RV_Undefined,
	RV_Object,
	RV_Int32,
	RV_Vector4,
	RV_Matrix4x4,
	RV_Int64,
	RV_Accessor,
	RV_JSNull,
	RV_Bool,
	RV_Iterator,
	RV_Reference,
	RV_Unset = 0xFFFFFF
};

using Int32 = __int32;
using Int64 = __int64;
struct RValue;

struct RArray
{
	Int32 nArrayLength;
	RValue* pArray;
};

struct RArrayRef
{
	Int32 nRefCount;
	RArray* pArray;
	RValue* pOwner;
};

struct RStringRef
{
	const char* String;
	Int32 nRefCount;
	Int32 nSize;
};

struct RValue
{
	union
	{
		double DoubleValue;
		Int32 Int32Value;
		Int64 Int64Value;
		void* PointerValue;
		const char** ppCharValue;
		RArrayRef* ArrayValue;
		RStringRef* StringValue;
	};

	Int32 Flags;
	Int32 Kind;

	RValue() : PointerValue(nullptr) {}

	RValue(Int32 Value);

	RValue(Int64 Value);

	RValue(double Value);

	RValue(RStringRef* Value);

	RValue(const char** Value);

	RValue* operator& ();

	RValue& at(int index);
};

//Data.win files, everyone!
//Shoutout to colinator27 for linking me to his awesome github repo:
//https://github.com/colinator27/dog-scepter/blob/master/DogScepterLib/Core/Chunks/GMChunkGEN8.cs

//Note: The offsets are offset from the GameForm_t base.
struct GEN8Chunk_t
{
	Int32 Length;
	bool DisableDebug;
	char FormatID; //The bytecode version probably
	short Unknown;
	Int32 FileNameOffset;
	Int32 ConfigOffset;
	Int32 LastObjectID;
	Int32 LastTileID;
	Int32 GameID;
	GUID LegacyGUID;
	Int32 GameNameOffset;
	Int32 Major, Minor, Release, Build;
	Int32 DefWindowWidth, DefWindowHeight;
	Int32 Info;
	char MD5Hash[16];
	Int32 CRCHash;
	char Timestamp[8];
	Int32 DisplayNameOffset;
};

struct GameForm_t
{
	char FORM[4]; //Literally has the string FORM, not null terminated.
	Int32 FileLength; //Self-explanatory.
	char GEN8[4];
	GEN8Chunk_t Gen8;

	const char* ReadString(unsigned long Offset);
};