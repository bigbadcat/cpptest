/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: LockQueue.h
* Summary: 提供线程安全访问的锁定队列。
*
* Author: XuXiang
* Date: 2018-11-24 16:50
*******************************************************/

#ifndef __LockQueue_h__
#define __LockQueue_h__

#include "../Macro.h"
#include <string>
#include <mutex>
#include <queue>
using namespace std;

namespace XX
{
	template<class T>
	class LockQueue
	{
		typedef std::queue<T*> TQueue;			//只接受指针形式的管理
	public:
		LockQueue();
		~LockQueue();

		//入队
		void Push(T *item);

		//出队(没有数据会抛出异常)
		T* Pop();

		//尝试出队(没有数据返回NULL)
		T* TryPop();

		//将数据全部移到另一个队列
		void MoveAll(TQueue &dst);

		//获取队列数据
		inline size_t GetSize()const { return m_Data.size(); }

	private:
		//数据队列
		TQueue m_Data;

		//互斥锁
		std::mutex m_Mutex;
	};

	template<class T>
	LockQueue<T>::LockQueue()
	{
	}

	template<class T>
	LockQueue<T>::~LockQueue()
	{
		while (m_Data.size() > 0)
		{
			T *item = m_Data.front();
			m_Data.pop();
			SAFE_DELETE(item);
		}
	}

	template<class T>
	void LockQueue<T>::Push(T *item)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Data.push(item);
	}

	template<class T>
	T* LockQueue<T>::Pop()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		T *item = m_Data.front();
		m_Data.pop();
		return item;
	}

	template<class T>
	T* LockQueue<T>::TryPop()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		if (m_Data.size() <= 0)
		{
			return NULL;
		}
		T *item = m_Data.front();
		m_Data.pop();
		return item;
	}

	template<class T>
	void LockQueue<T>::MoveAll(TQueue &dst)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		while (m_Data.size() > 0)
		{
			T *item = m_Data.front();
			m_Data.pop();
			dst.push(item);
		}
	}
}

#endif