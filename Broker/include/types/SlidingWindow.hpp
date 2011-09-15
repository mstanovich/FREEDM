#include <deque>
#include <boost/thread.hpp>

#ifndef SLIDINGWINDOW_H
#define SLIDINGWINDOW_H

// Sliding Window Container
template<typename Data>
class SlidingWindow
{
private:
    std::deque<Data>            m_queue; /// queue of data to send
    mutable boost::mutex        m_mutex; /// threadsafe mutex
    boost::condition_variable   m_condition_variable; /// A notify for wait pop
public:
    typedef typename std::deque<Data>::iterator iterator; /// container iterator
    /// Put an item into the container
    void Push(Data const& data)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        m_queue.push_back(data);
        lock.unlock();
        m_condition_variable.notify_one();
    }
    /// Check if container is empty
    bool IsEmpty() const
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        return m_queue.empty();
    }
    /// Pop with boolean for success failure
    bool TryPop(Data& popped_value)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        if(m_queue.empty())
        {
            return false;
        }

        popped_value=m_queue.front();
        m_queue.pop_front();
        return true;
    }
    /// Wait until the queue is non-empty and then pop
    void WaitPop(Data& popped_value)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        while(m_queue.empty())
        {
            m_condition_variable.wait(lock);
        }

        popped_value=m_queue.front();
        m_queue.pop_front();
    }
    /// Pop immediately.
    void pop()
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        m_queue.pop_front();
    }
    /// Check container size
    unsigned int size()
    {
        return m_queue.size();
    }
    /// Iterator to front of container
    iterator begin()
    {
        return m_queue.begin();
    }
    /// Iterator to end of container
    iterator end()
    {
        return m_queue.end();
    }
    /// Return the first item in the container
    Data front()
    {
        return m_queue.front();
    }
};

#endif
