#include "Structs.hpp"

const char* GameForm_t::ReadString(unsigned long Offset)
{
	return reinterpret_cast<const char*>(reinterpret_cast<char*>(this) + Offset);
}