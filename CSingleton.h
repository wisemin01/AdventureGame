#pragma once

template <class T>
class CSingleton
{
private:
	static T* instance;
public:
	CSingleton() {}
	virtual ~CSingleton() {}

	static T* Instance() {
		if (!instance)
			instance = new T;

		return instance;
	}

	static void DestInstance()
	{
		if (instance) {
			delete instance;
			instance = nullptr;
		}
	}
};

template <class T>
T* CSingleton<T>::instance = null;