#include "RValue.hpp"
#include <stdint.h>

struct CInstanceBase
{
	RValue* yyVars;
};

struct YYObjectBase : public CInstanceBase
{
	YYObjectBase* m_pNextObject;
	YYObjectBase* m_pPrevObject;
	YYObjectBase* m_prototype;
	void* m_pcre;
	void* m_pcreExtra;
	const char* m_class;
	void* m_GetOwnProperty;
	void* m_deleteProperty;
	void* m_defineOwnProperty;
	void* m_yyVarsMap;
	void* m_pWeakRefs;

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

struct YYRECT
{
	int32_t left, top, right, bottom;
};

struct SLink
{
	SLink* next;
	SLink* prev;
	void* list;
};

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
	void* i_initcode;
	void* i_precreatecode;
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
};