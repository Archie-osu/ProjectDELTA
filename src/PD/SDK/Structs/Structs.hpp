#pragma once
#include <guiddef.h>
#include <stdint.h>

enum OSFlavors //mmm, tasty
{
	OSF_Windows = 1,
	OSF_UnixBased = 2,
	OSF_PlayStation = 4,
	OSF_Switch = 5
};

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

	RValue(void* Value);

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

enum eGML_TYPE : __int32
{
	eGMLT_NONE = 0x0,
	eGMLT_ERROR = (-65536),
	eGMLT_DOUBLE = 0x1,
	eGMLT_STRING = 0x2,
	eGMLT_INT32 = 0x4,
};

struct RToken
{
	int kind;
	eGML_TYPE type;
	int ind;
	int ind2;
	RValue value;
	int itemnumb;
	RToken* items;
	int position;
};

struct VMBuffer
{
	int m_size;
	int m_numLocalVarsUsed;
	int m_numArguments;
	const uint8_t* m_pBuffer;
	void** m_pConvertedBuffer;
	int* m_pJumpBuffer;
};

struct CInstance;

struct CCode
{
	CCode* m_pNext;
	int i_kind;
	bool i_compiled;
	const char* i_str;
	RToken i_token;
	RValue i_value;
	VMBuffer* i_pVM;
	VMBuffer* i_pVMDebugInfo;
	char* i_pCode;
	const char* i_pName;
	int i_CodeIndex;
	void* i_pFunc;
	bool i_watch;
	int i_offset;
	int i_locals;
	int i_args;
	int i_flags;
	void* i_pPrototype;
};

/*
struct CInstance : public YYObjectBase
{
	int64 m_CreateCounter;
	CObjectGM *m_pObject;
	CPhysicsObject *m_pPhysicsObject;
	CSkeletonInstance *m_pSkeletonAnimation;
	CSequenceInstance *m_pControllingSeqInst;
	unsigned int m_Instflags;
	int i_id;
	int i_objectindex;
	int i_spriteindex;
	float i_sequencePos;
	float i_lastSequencePos;
	float i_sequenceDir;
	float i_imageindex;
	float i_imagespeed;
	float i_imagescalex;
	float i_imagescaley;
	float i_imageangle;
	float i_imagealpha;
	unsigned int i_imageblend;
	float i_x;
	float i_y;
	float i_xstart;
	float i_ystart;
	float i_xprevious;
	float i_yprevious;
	float i_direction;
	float i_speed;
	float i_friction;
	float i_gravitydir;
	float i_gravity;
	float i_hspeed;
	float i_vspeed;
	YYRECT i_bbox;
	int i_timer[12];
	cInstancePathAndTimeline *m_pPathAndTimeline;
	CCode *i_initcode;
	CCode *i_precreatecode;
	CObjectGM *m_pOldObject;
	int m_nLayerID;
	int i_maskindex;
	__int16 m_nMouseOver;
	CInstance *m_pNext;
	CInstance *m_pPrev;
	SLink m_collisionLink;
	SLink m_dirtyLink;
	SLink m_withLink;
	float i_depth;
	float i_currentdepth;
	float i_lastImageNumber;
	unsigned int m_collisionTestNumber;
};
*/