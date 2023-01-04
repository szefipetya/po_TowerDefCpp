#pragma once
#include<mutex>
#include<map>
#include<functional>
#include<iterator>
template<class Key, class Value>
class ThreadSafeMap
{
    std::mutex m_;
    std::unordered_map<Key, Value> map;
public:
    ThreadSafeMap<Key, Value>() {

    }
    ThreadSafeMap<Key, Value>(const  ThreadSafeMap<Key, Value>& t) {
        map = t.map;
    }
    typedef typename std::unordered_map<Key, Value>::iterator iterator;
    typedef typename std::unordered_map<Key, Value>::const_iterator const_iterator;

    Value get(Key const& k) {
        std::unique_lock<decltype(m_)> lock(m_);
        return map[k]; // Return a copy.
    }
    Value operator[](Key k) {
        return map[k];
    }
    template<class Value2>
    void set(Key const& k, Value2&& v) {
        std::unique_lock<decltype(m_)> lock(m_);
        map[k] = std::forward<Value2>(v);
    }
    void insert(std::pair<Key, Value> pair) {
        std::unique_lock<decltype(m_)> lock(m_);
        map.insert(pair);
		/*std::cout << "map size now: " << map.size() << std::endl;
		std::cout << "diff: " << size() << std::endl;*/
    }

	int size()
	{
		return map.size();
	}

    typename std::unordered_map<Key, Value>::const_iterator find(Key const& k) {
		std::unique_lock<decltype(m_)> lock(m_);
        return map.find(k);
    }

    typename std::unordered_map<Key, Value>::iterator m_end() {
      //  std::unique_lock<decltype(m_)> lock(m_);
        return map.end();
    }
    typename std::unordered_map<Key, Value>::const_iterator end() const  {
        //  std::unique_lock<decltype(m_)> lock(m_);
        return map.cend();
    }
    typename std::unordered_map<Key, Value>::const_iterator begin() const {
		//        std::unique_lock<decltype(m_)> lock(m_);
        return map.cbegin();
    }
    typename std::unordered_map<Key, Value>::iterator m_begin() {
        //        std::unique_lock<decltype(m_)> lock(m_);
        return map.begin();
    }
    void erase(Key const& k) {
        std::unique_lock<decltype(m_)> lock(m_);
        map.erase(k);
    }
	void erase(typename std::unordered_map<Key, Value>::const_iterator const& k) {
		std::unique_lock<decltype(m_)> lock(m_);
		map.erase(k);
	}
    std::unordered_map< Key, Value> const getMap() const {
        return this->map;
    }
	std::unordered_map< Key, Value> m_getMap() {
		return this->map;
	}

    void setMap(std::unordered_map< Key, Value> map) {
        this->map = map;
    }
	ThreadSafeMap< Key, Value> operator=(const ThreadSafeMap< Key, Value>& other) {
		this->setMap(other.getMap());
	}
    ~ThreadSafeMap() {
        //map.clear();
       // delete m_;
      //  delete map;
    }
};

