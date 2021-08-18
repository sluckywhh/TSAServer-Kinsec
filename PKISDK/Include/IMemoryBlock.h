#ifndef IMEMORYBLOCK_H
#define IMEMORYBLOCK_H
#include <vector>

// 替代 std::vector<unsigned char> ， 由调用方构造，并由调用方负责释放
// 被调用方不得缓存任何 IMemoryBlock 的指针
class IMemoryBlock
{
public:
	virtual  size_t __stdcall resize(size_t) = 0;
	virtual  size_t __stdcall size() const = 0; 
	virtual  unsigned char* __stdcall at(size_t) = 0;
	virtual const unsigned char* __stdcall at(size_t) const = 0;
	virtual  size_t __stdcall copy(size_t begin, const unsigned char* data, size_t size) = 0;
};

#endif