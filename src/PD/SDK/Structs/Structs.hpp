#pragma once
#include <guiddef.h>
#include <stdint.h>
#include <string>
#include "RValue/RValue.hpp"
#include "RefThing/RefThing.hpp"
#include "RArray/RArray.hpp"

#pragma warning(disable : 26495)

enum OSFlavors //mmm, tasty
{
	OSF_Windows = 1,
	OSF_UnixBased = 2,
	OSF_PlayStation = 4,
	OSF_Switch = 5
};

//Data.win files, everyone!
//Shoutout to colinator27 for linking me to his awesome github repo:
//https://github.com/colinator27/dog-scepter/blob/master/DogScepterLib/Core/Chunks/GMChunkGEN8.cs

//Note: The offsets are offset from the GameForm_t base.
struct GEN8Chunk_t
{
	int32_t Length;
	bool DisableDebug;
	char FormatID; //The bytecode version probably
	short Unknown;
	int32_t FileNameOffset;
	int32_t ConfigOffset;
	int32_t LastObjectID;
	int32_t LastTileID;
	int32_t GameID;
	GUID LegacyGUID;
	int32_t GameNameOffset;
	int32_t Major, Minor, Release, Build;
	int32_t DefWindowWidth, DefWindowHeight;
	int32_t Info;
	char MD5Hash[16];
	int32_t CRCHash;
	char Timestamp[8];
	int32_t DisplayNameOffset;
};

struct GameForm_t
{
	char FORM[4]; //Literally has the string FORM, not null terminated.
	int32_t FileLength; //Self-explanatory.
	char GEN8String[4];
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
	void** vTable;
	int m_size;
	int m_numLocalVarsUsed;
	int m_numArguments;
	const uint8_t* m_pBuffer;
	void** m_pConvertedBuffer;
	int* m_pJumpBuffer;
};

struct CInstance;

#pragma pack(push, 4)
struct CCode
{
	void** vTable;
	CCode* m_pNext;
	int i_kind;
	int i_compiled;
	const char* i_str;
	RToken i_token;
	RValue i_value;
	VMBuffer* i_pVM;
	VMBuffer* i_pVMDebugInfo;
	char* i_pCode;
	const char* i_pName; //0x5C
	int i_CodeIndex;
	void* i_pFunc;
	bool i_watch;
	int i_offset;
	int i_locals;
	int i_args;
	int i_flags;
	void* i_pPrototype;
};
#pragma pack(pop)


struct YYRECT
{
	int32_t left;
	int32_t top;
	int32_t right;
	int32_t bottom;
};

struct SLink
{
	SLink* next;
	SLink* prev;
	struct SLinkListEx* list;
};

template <typename Key, typename Value>
struct CHashMapElement
{
	Value v;
	Key k;
	unsigned int hash;
};

template <typename Key, typename Value>
struct CHashMap
{
	int m_curSize;
	int m_numUsed;
	int m_curMask;
	int m_growThreshold;
	
	CHashMapElement<Key, Value>* mElements;
};

struct CInstanceBase
{
	RValue* yyvars;
};

struct YYObjectBase;
using FnGetOwnProperty = void(YYObjectBase*, RValue*, const char*);
using FnDeleteProperty = void(YYObjectBase*, RValue*, const char*, bool);
using FnDefineOwnProperty = void(YYObjectBase*, const char*, RValue*, bool);

struct YYObjectBase : CInstanceBase
{
	YYObjectBase* m_pNextObject;
	YYObjectBase* m_pPrevObject;
	YYObjectBase* m_prototype;
	void* m_pcre;
	void* m_pcreExtra;
	const char* m_class;
	FnGetOwnProperty* m_getOwnProperty;
	FnDeleteProperty* m_deleteProperty;
	FnDefineOwnProperty* m_defineOwnProperty;
	CHashMap<int, RValue*>* m_yyvarsMap;
	void** m_pWeakRefs;
	uint32_t m_numWeakRefs;
	uint32_t m_nvars;
	uint32_t m_flags;
	uint32_t m_capacity;
	uint32_t m_visited;
	uint32_t m_visitedGC;
	int32_t m_GCgen;
	int32_t m_GCcreationframe;
	int m_slot;
	int m_kind;
	int m_rvalueInitType;
	int m_curSlot;
};
/*
struct CInstance : public YYObjectBase
{
	int64_t m_CreateCounter;
	void* m_pObject;
	void* m_pPhysicsObject;
	void* m_pSkeletonAnimation;
	void* m_pControllingSeqInst;
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
	void* m_pPathAndTimeline;
	CCode* i_initcode;
	CCode* i_precreatecode;
	void* m_pOldObject;
	int m_nLayerID;
	int i_maskindex;
	__int16 m_nMouseOver;
	CInstance* m_pNext;
	CInstance* m_pPrev;
	SLink m_collisionLink;
	SLink m_dirtyLink;
	SLink m_withLink;
	float i_depth;
	float i_currentdepth;
	float i_lastImageNumber;
	unsigned int m_collisionTestNumber;
};*/

struct CStream
{
	bool m_ReadOnly;
	int internal_buffer_size; //These might not be i64, but i32 - someone check that please.
	int internal_current_position;
	void* internal_buffer;
};

__declspec(align(8)) struct CScript
{
	void** vTable;
	CStream* s_text;
	CCode* s_code;
	void* s_pFunc;
	CInstance* s_pStaticObject;

	union
	{
		const char* s_script;
		int s_compiledIndex;
	};

	const char* s_name;
	int s_offset;
};

struct VMExec;

/*
* This changes version to version, not worth having
struct VMExec
{
	VMExec* pPrev;
	VMExec* pNext;
	uint8_t* pStack;
	int localCount;
	YYObjectBase* pLocals;
	YYObjectBase* pSelf;
	YYObjectBase* pOther;
	CCode* pCCode;
	RValue* pArgs;
	int argumentCount;
	const uint8_t* pCode;
	uint8_t* pBP;
	VMBuffer* pBuffer;
	int line;
	const char* pName;
	VMBuffer* pDebugInfo;
	const char* pScript;
	int stackSize;
	int offs;
	int boffs;
	int retCount;
	int bufferSize;
	int prevoffs;
	void** buff;
	int* jt;
};
*/