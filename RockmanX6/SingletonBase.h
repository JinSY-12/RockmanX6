#pragma once

template <class T>
class SingletonBase
{
protected:
	// 싱글톤 인스턴스 선언
	static T* singleton;

	SingletonBase() {}
	~SingletonBase() {}

public:
	// 싱글톤 가져오기
	static T* getSingleton(void);
	// 싱글톤 메모리에서 해제
	void releaseSingleton(void);
};

template <typename T>
T* SingletonBase<T>::singleton = 0;

// 싱글톤 가져오기
template <typename T>
T* SingletonBase<T>::getSingleton(void)
{
	if (!singleton) singleton = new T;

	return singleton;
}

// 싱글톤 메모리에서 해제
template <typename T>
void SingletonBase<T>::releaseSingleton(void)
{
	if (singleton)
	{
		delete singleton;
		singleton = 0;
	}
}