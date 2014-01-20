// Author: zhangguoqiang01 <80176975@qq.com>

#include "singleton.h"

template <class T>
pthread_once_t Singleton<T>::m_init = PTHREAD_ONCE_INIT;

template <class T>
pthread_once_t Singleton<T>::m_release = PTHREAD_ONCE_INIT;

template <class T>
T* Singleton<T>::m_instance = NULL;

template <class T>
Singleton<T>::Singleton()
{}

template <class T>
Singleton<T>::~Singleton()
{}

template <class T>
void Singleton<T>::SingletonInit()
{
	m_instance = new T;
}

template <class T>
void Singleton<T>::SingletonRelease()
{
	if (NULL == m_instance) {
		return;	
	}

	delete m_instance;
	m_instance = NULL;
}

template <class T>
T* Singleton<T>::GetInstance()
{
	pthread_once(&m_init, &Singleton::SingletonInit);
	return m_instance;
}

template <class T>
void Singleton<T>::ReleaseInstance()
{
	pthread_once(&m_release, &Singleton::SingletonRelease);
}	

