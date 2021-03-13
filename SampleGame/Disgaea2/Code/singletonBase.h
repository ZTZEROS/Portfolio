#pragma once
template <typename T>
class singletonBase
{
protected:
	//�̱��� �ν��Ͻ� ����
	static T* singleton;

	singletonBase(void){};
	~singletonBase(void){};

public:
	static T* getsingleton(void);
	void releaseSingleton(void);
};

//�̱��� �ʱ�ȭ
template <typename T>
T* singletonBase<T>::singleton = 0;

//�̱��� �� �����´�
template <typename T>
T* singletonBase<T>::getsingleton(void)
{
	if (!singleton) singleton = new T;

	return singleton;
}

//�̱��� �޸� ����
template<typename T>
void singletonBase<T>::releaseSingleton(void)
{
	if (singleton)
	{
		delete singleton;

		singleton = 0;
	}
}