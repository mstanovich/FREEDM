#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <boost/utility.hpp>
#include <boost/thread/once.hpp>
#include <boost/scoped_ptr.hpp>

namespace freedm {
namespace templates {

/// Dictates that an object should be singleton
template<class T>
class Singleton : private boost::noncopyable
{
public:
    ///Retrieves the instance of the singleton class.
    static T& instance()
    {
        // Warning: If T's constructor throws, instance() will return a null reference.
        boost::call_once(init, flag);
        return *t;
    }
    ///Initializes the singleton object.
    static void init() // never throws
    {
        t.reset(new T());
    }

protected:
    /// Cleanup
    ~Singleton() {}
    /// Copy protection
    Singleton() {}

private:
    static boost::scoped_ptr<T> t; /// Pointer to the instance
    static boost::once_flag flag; /// flag to only construct the object once

};

}
}

template<class T> boost::scoped_ptr<T> freedm::templates::Singleton<T>::t(0);
template<class T> boost::once_flag freedm::templates::Singleton<T>::flag
									= BOOST_ONCE_INIT;
#endif //SINGLETON_HPP
