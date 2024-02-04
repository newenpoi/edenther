#pragma once

template <class T>

class Singleton
{
protected:
    Singleton() {} // Prevents instanciation outside the class.
    virtual ~Singleton() {}

public:
    Singleton(const Singleton&);
    Singleton& operator = (const Singleton&);

    static T& getInstance()
    {
        static T instance;
        return instance;
    }
};

template<class T>
inline Singleton<T>::Singleton(const Singleton&)
{
}

