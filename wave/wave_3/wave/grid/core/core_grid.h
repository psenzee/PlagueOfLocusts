#pragma once

#include "point2i.h"
#include "directions.h"
#include "size2u.h"
#include "region.h"

namespace grid {

template <typename T>
struct readable
{
    virtual ~readable();
    
    virtual T        get(const point2i &p)          const = 0;
    virtual T        null_value()                   const = 0;
    virtual size2u   size()                         const = 0;
    
    virtual bool     is_referrable()                const { return false; }
    virtual const T &get_ref(const point2i &p)      const { static T v; return v; }
    
    virtual bool     is_row_accessible()            const { return false; }
    virtual const T *row(int y)                     const { return nullptr; }
    
    virtual bool     is_read_only()                 const { return true; }
    
//  virtual bool operator==(const readable<T> &o)   const { if (this == &o) return true; return equal(o, *this); }
    virtual T    operator()(const point2i &p)       const { return get(p); }
};

template <typename T> readable<T>::~readable() {}

template <typename T>
struct grid : public readable<T>
{
    virtual void  set(const point2i &p, const T &value) = 0;
    
    virtual T    &get_ref(const point2i &p)                { static T v; return v; }
    
    virtual T    *row(int y)                               { return nullptr; }
    
    bool          is_read_only()                     const { return false; }

    virtual grid<T> &operator=(const readable<T> &o)       { if (this == &o) return *this; copy(o, *this); return *this; }
};
/*
template <typename T>
struct layers
{
     std::vector<
};
*/

}
