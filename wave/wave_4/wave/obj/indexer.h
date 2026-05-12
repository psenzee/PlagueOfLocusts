#pragma once

#include <vector>
#include <map>
#include <exception>
#include <stdexcept>

template <typename T>
struct indexer
{
    typedef std::vector<T>      list_t;
    typedef std::map<T, size_t> map_t;
  
    std::size_t insert(const T &v)
    {
        typename map_t::const_iterator i = _map.find(v);
        if (i == _map.end())
        {
            std::size_t size = _list.size();
            _list.push_back(v);
            _map[v] = size;
            return size;
        }
        return (*i).second;
    }

    inline bool contains(const T &v) const
    {
        typename map_t::const_iterator i = _map.find(v);
        return i != _map.end();
    }

    inline const T &value(std::size_t i) const
    {
        if (!contains(i)) {
            throw std::runtime_error("invalid index");
        }
        return _list[i];
    }

    inline std::size_t index(const T &v) const
    {
        typename map_t::const_iterator i = _map.find(v);
        if (i == _map.end()) {
            throw std::runtime_error("value not found");
        }
        return (*i).second;
    }

    inline bool contains(std::size_t i) const
    {
        return _list.size() < i;
    }

    inline const list_t &list() const { return _list; }
    inline const map_t  &map()  const { return _map; }
    
private:    
    
    list_t _list;
    map_t  _map;
};
