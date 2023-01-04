/*#pragma once

#pragma once
#include<mutex>
#include<vector>
#include<functional>
#include<iterator>
template<class Value>
class ThreadThreadSafeVector
{
    std::mutex m_;
    std::vector<Value> vec;
public:
    typedef typename std::vector<Value>::iterator iterator;
    Value get(int k) {
        std::unique_lock<decltype(m_)> lock(m_);
        return vec[k]; // Return a copy.
    }

    template<class Value2>
    void set(int k, Value2&& v) {
        std::unique_lock<decltype(m_)> lock(m_);
        vec[k] = std::forward<Value2>(v);
    }
    int size() {
        std::unique_lock<decltype(m_)> lock(m_);
        return vec.size();
    }
    void push_back(Value value) {
        std::unique_lock<decltype(m_)> lock(m_);
        vec.push_back(value);
    }
    Value operator[](int ind) {
        return vec[ind];
    }
    
    typename std::vector<Value>::iterator end() {
        std::unique_lock<decltype(m_)> lock(m_);
        return vec.end();
    }
    typename std::vector<Value>::iterator begin() {
        std::unique_lock<decltype(m_)> lock(m_);
        return vec.begin();
    }
    void erase(typename std::vector<Value>::iterator k) {
        std::unique_lock<decltype(m_)> lock(m_);
        vec.erase(k);
    }
};
*/
#ifndef ThreadSafeVector_H
#define ThreadSafeVector_H

#include <vector>
#include <mutex>
#include <condition_variable>
#include <string>
template<class Value>
class ThreadSafeVector {
public:
    typedef typename std::vector<Value>::iterator iterator;
    typedef typename std::vector<Value>::const_iterator const_iterator;
    ThreadSafeVector() : vec(), mut(), cond() {}
    ThreadSafeVector(const ThreadSafeVector& orig) : vec(orig.vec), mut(), cond() {}
    ~ThreadSafeVector() {}
    void insert(Value in, const int index)
    {
        std::lock_guard<std::mutex> lock(mut);
        vec[index] = std::move(in);
        cond.notify_one();
    }
    void push_back(Value in)
    {
        std::lock_guard<std::mutex> lock(mut);
        vec.push_back(std::move(in));
        cond.notify_one();
    }
    Value& operator[](const int index)
    {
        std::lock_guard<std::mutex> lock(mut);
        
        return vec[index];
        cond.notify_one();

    }
    std::vector<Value> toVector()
    {
        return vec;
    }
    typename std::vector<Value>::const_iterator begin()
    {
        return vec.begin();
    }
    typename std::vector<Value>::const_iterator end()
    {
        return vec.end();
    }

	typename std::vector<Value>::iterator begin_m()
	{
		return vec.begin();
	}
	typename std::vector<Value>::iterator end_m()
	{
		return vec.end();
	}
    void erase(typename std::vector<Value>::iterator k) {
        std::lock_guard<std::mutex> lock(mut);
        mut.lock();
        vec.erase(k);
        mut.unlock();
        cond.notify_one();
    }
    int size() {
        return vec.size();
    }
    void resize(int n) {
        std::lock_guard<std::mutex> lock(mut);
        vec.resize(n);
        cond.notify_one();
    }
	void clear() {
		std::lock_guard<std::mutex> lock(mut);
		vec.clear();
		cond.notify_one();
	}

private:
    std::vector<Value> vec;
    std::mutex mut;
    std::condition_variable cond;
};

#endif /* ThreadSafeVector_HPP */


