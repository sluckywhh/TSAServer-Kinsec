#ifndef MEMORYBLOCK_H
#define MEMORYBLOCK_H

#include <vector>
#include <IMemoryBlock.h>


class MemoryBlock : public IMemoryBlock
{
private:
	
public:
	std::vector<unsigned char> m_data ;
	
	MemoryBlock(){
		
	}
	
	MemoryBlock(std::vector<unsigned char>& source)
	{
		m_data = source;
	}
	
	size_t __stdcall resize(size_t size)
	{
		m_data.resize (size) ;
		return m_data.size() ;
	}
	
	size_t __stdcall size() const
	{
		return m_data.size() ;
	}
	
	size_t __stdcall copy(size_t begin, const unsigned char* data, size_t size)
	{
		if (begin + size > this->size())
		{
			this->resize(begin + size);
		}
		memcpy(&m_data[begin], data, size);
		return size;
	}
	
	unsigned char* __stdcall at(size_t pos)
	{
		if (m_data.size() == 0){
			return NULL;
		}
		return &m_data[pos] ;
	}
	
	const unsigned char* __stdcall at(size_t pos) const
	{
		if (m_data.size() == 0){
			return NULL;
		}
		return &m_data[pos] ;
	}
	
	unsigned char* operator [](size_t i)
	{
		return at(i);
	}
	
};

#endif