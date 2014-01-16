/***************************************************************************
 * 
 * Copyright (c) 2013 80176975@qq.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file mutex.h
 * @author zhangguoqiang01(80176975@qq.com)
 * @date 2013/12/17 21:27:38
 * @brief 
 *  
 **/



#include "dsalgo/head.h"
template <class T>
class Singleton
{
public:
	static T* GetInstance();
	void ReleaseInstance();
private:
	Singleton();
	~Singleton();
	static void SingletonInit();
	static void SingletonRelease();
private:
	static pthread_once_t m_init;
	static pthread_once_t m_release;
	static T *m_instance;
};
