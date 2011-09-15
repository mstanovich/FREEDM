

#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#include <map>


/// Template for a thread-safe singleton
#include <boost/utility.hpp>
#include <boost/thread/once.hpp>
#include <boost/scoped_ptr.hpp>

namespace Templates {
/* function object to check the value of a map element */
template <class K, class V>
class set_value {
  private:
    V value;
  public:
    // constructor (initialize value to compare with)
    set_value (const V& v)
     : value(v) {
    }
    // comparison
    void operator() ( std::pair<const K, V> elem) {
        elem.second = value;
        return;
    }
};

}


template <class T>
bool from_string(T& t,
                 const std::string& s,
                 std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}

template <class T>
std::string to_string(const T& value)
{
std::stringstream oss;
oss << value;
return oss.str();
}

#endif /* UTILITY_HPP_ */
