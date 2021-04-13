//The RValue struct has methods inside of it, that's why there's a CPP file for it.
#include "RValue.hpp"

enum Kinds_t
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

RValue::RValue(std::nullptr_t)
{
	i64Value = 0;
	nFlags = 0;
	nKind = KIND_UNSET;
}

RValue::RValue(Real v)
{
	dValue = v;
	nKind = KIND_REAL;
}

RValue::RValue(Int32 v)
{
	iValue = v;
	nKind = KIND_INT32;
}

RValue::RValue(Int64 v)
{
	i64Value = v;
	nKind = KIND_INT64;
}

RValue::RValue(Pointer pv)
{
	pValue = pv;
	nKind = KIND_POINTER;
}

RValue::RValue(const char* szv)
{
	szValue = &szv;
	nKind = KIND_STRING;
}

RValue::RValue(const char** szv)
{
	szValue = szv;
	nKind = KIND_STRING;
	nFlags = 0;
}

RValue& RValue::at(int Index)
{
	return pArrayReference->pArray->pArray[Index];
}

const char* RValue::ToString()
{
	return *szValue;
}

void RValue::operator=(Real v)
{
	dValue = v;
	nKind = KIND_REAL;
}

void RValue::operator=(Int32 v)
{
	iValue = v;
	nKind = KIND_INT32;
}

void RValue::operator=(Int64 v)
{
	i64Value = v;
	nKind = KIND_INT64;
}

void RValue::operator=(Pointer pv)
{
	pValue = pv;
	nKind = KIND_POINTER;
}

void RValue::operator=(const char* szv)
{
	szValue = &szv;
	nKind = KIND_STRING;
}

RValue& RValue::operator[](int Index)
{
	return pArrayReference->pArray->pArray[Index];
}

RValue::operator int()
{
	return iValue;
}

RValue::operator double()
{
	return dValue;
}

RValue::operator double* ()
{
	return &dValue;
}
