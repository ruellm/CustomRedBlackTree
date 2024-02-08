/// Copyright © 2019-2024 Lincoln Global Inc. All Rights Reserved.
///  \file AklCustomRBNodeCreator.h
///  Declaration of the AklCustomRBNodeCreator class
///  \author Ruell Magpayo
#pragma once

#include <vector>
template <typename T>
class AklCustomRBNodeCreator
{
public:
	AklCustomRBNodeCreator() : 
		m_currBlockCount(0),
		m_nodeSize(0),
		m_maxNodeCount(0),
		m_memOffset(0)
	{

	}
	~AklCustomRBNodeCreator()
	{
		Release();
	}

	void Initialize(int nodeSize)
	{
		m_nodeSize			= nodeSize;
		m_currBlockCount	= 0;

		Expand();
	}

	void Release()
	{
		for (int i = 0; i < m_workArea.size(); i++)
		{
			free(m_workArea[i]);
		}

		m_workArea.clear();
		m_workArea.shrink_to_fit();

		m_currBlockCount = 0;
		m_maxNodeCount = 0;
		m_memOffset = 0;
	}

	T* Obtain()
	{
		if (m_currBlockCount + 1 > m_maxNodeCount)
		{
			Expand();
			m_memOffset = 0;
		}

		unsigned char* currentMemory = reinterpret_cast<unsigned char*>(m_workArea[m_workArea.size() - 1]);
		T* node = new(currentMemory + m_memOffset) T();
		m_memOffset += sizeof(T);

		++m_currBlockCount;
		return node;
	}
	
private:

	void Expand()
	{
		void* memory = static_cast<T*>(std::malloc(sizeof(T) * m_nodeSize));
		m_workArea.push_back(memory);
		m_maxNodeCount += m_nodeSize;
	}

	int m_currBlockCount;
	std::vector<void*> m_workArea;
	int m_nodeSize;
	int m_maxNodeCount;
	int m_memOffset;
};

