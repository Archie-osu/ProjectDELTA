#pragma once
#define ADD_OFFSET(num, addr) unsigned char __pad##num[addr];

#include <vector>
//The main namespace, everything is under here to prevent namespace contamination.
//Version 1.1 by Archie
namespace ghl
{
	struct ptr_t
	{
		using uchar = unsigned char;

	private:
		uchar* pData;
	public:
		ptr_t() : pData(0) {}

		ptr_t(void* lpData) : pData((uchar*)lpData) {}

		uchar* getraw()
		{
			return pData;
		}

		template <typename T>
		T& get()
		{
			return *(T*)(pData);
		}

		ptr_t add(size_t offset)
		{
			return ptr_t(pData + offset);
		}

		ptr_t sub(size_t offset)
		{
			return ptr_t(pData - offset);
		}

		ptr_t follow()
		{
			return ptr_t(*(uchar**)pData);
		}

		ptr_t autofollow(const std::vector<size_t>& offsets)
		{
			ptr_t tmp = this->add(offsets[0]);
			for (size_t idx = 1; idx < offsets.size(); idx++)
			{
				tmp = tmp.follow();
				tmp = tmp.add(offsets[idx]);
			}

			return tmp;
		}

		bool valid()
		{
			return pData;
		}
	};

	void AllocConsoleStream();
}