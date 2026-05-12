#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string_view>
#include <limits>

class object
{
    mutable uint8_t _rc;
    
public:

    enum Interface { ITERABLE, COUNTABLE, RANDOM_ACCESS, SEQUENTIAL, CONTAINER, NODEABLE };
    
    enum Type { ANY = 0, FRAME, PAIR, FUNC };
    
    object() : _rc(1) {}
    
    virtual ~object() {}
    
    virtual void          retain()                 const { _rc += (_rc < 254); }
    virtual void          release()                const { _rc -= (_rc < 255); /*if (!--_rc) delete const_cast<object *>(this);*/ }
    virtual Type          type()                   const { return ANY; }
    virtual bool          is_type(Type t)          const { return t == type(); }
    virtual std::ostream &print(std::ostream &os)  const { return os << "[object]"; }

    // interfaces
    
    virtual bool          implements(Interface nt) const { return false; }
    
    // ITERABLE
    virtual bool          has_next()               const { return false; }
    virtual object       *next()                         { return nullptr; }
    virtual const object *next()                   const { return nullptr; }
    
    // NODEABLE
    virtual bool          has_next_node()          const { return false; }
    virtual object       *next_node()                    { return nullptr; }
    virtual const object *next_node()              const { return nullptr; }
    
    // COUNTABLE + suboptimal: SEQUENTIAL; can be implemented in terms of iterable, but at a performance cost
    virtual size_t        count()                  const { return 0; }
    
    // RANDOM_ACCESS + suboptimal: SEQUENTIAL
    virtual const object *at(int i)                const { return nullptr; }
    virtual void          set(int i, object *u)    const { }
};
