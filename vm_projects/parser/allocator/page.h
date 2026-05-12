#pragma once

#include <cstring>

struct page
{
    inline page(size_t items, size_t item_size) : _item_size(item_size)
    {
        _end = (_current = _data = new char [items * item_size]) + items * item_size;
        memset(_current, 0, items * item_size);
    }
    inline ~page()              { delete [] _data; }
    inline void *create()       { void *data = _current; _current += _item_size; return data; }
    inline bool  full()   const { return _current >= _end; }

private:
    char   *_data, *_current, *_end;
    size_t  _item_size;
};
