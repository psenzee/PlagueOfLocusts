#pragma once

#include <array>
#include <vector>
#include <algorithm>

namespace autodiff {

class term
{
public:
    enum Type { TYPE_NONE, TYPE_FUNCTION, TYPE_VARIABLE, TYPE_SCALAR };
    
    virtual Type         type()                  const { return TYPE_NONE; }
    virtual size_t       arity()                 const { return 0; }
    virtual term        *derivative()                  { return nullptr; }
    virtual void         add(term *a)                  {};
    virtual term        *at(size_t i)                  { return nullptr; };
    virtual size_t       size()                  const { return 0; }
    virtual void         set_id(const char *id)        {}
    virtual const char  *get_id()                const { return nullptr; }
    virtual const char  *name()                  const { return "*"; }
    virtual void         set_value(float v)            {}
    virtual float        get_value()             const { return 0; }
};

class variable : public term
{
public:
    typedef std::string str_t;

    virtual Type         type()                    const { return TYPE_VARIABLE; }
    term                *derivative();
    void                 set_id(const char *id)          { _id = id; }
    const char          *get_id()                  const { return _id; }
    
private:
    std::string _id;
};

template <size_t N>
class fixed_composite : public term
{
    std::array<term *, N> _children;
    size_t                _count;
public:
    fixed_composite() : _children({ 0 }), _count(0) {}
    virtual void    add(term *a)       { _children[_count++] = a; }
    virtual size_t  arity()      const { return N; }
    virtual term   *at(size_t i)       { return _children[i]; }
    virtual size_t  size()       const { return _children.size(); }
};

class composite : public term
{
    std::vector<term *> _children;
public:
    virtual void    add(term *a)       { _children.push_back(a); }
    virtual size_t  arity()      const { return size(); }
    virtual term   *at(size_t i)       { return _children[i]; }
    virtual size_t  size()       const { return _children.size(); }
};

template <size_t N>
class nary_function : public fixed_composite<N>
{
public:
    virtual Type type() const { return TYPE_FUNCTION; }
};

class multiply : public nary_function<2>
{
    const char *name() const { return "*"; }
    term *derivative();
};

class add : public nary_function<2>
{
    const char *name() const { return "+"; }
};

class subtract : public nary_function<2>
{
    const char *name() const { return "-"; }
};

class scalar : public term
{
    float _value;
public:
    inline scalar(float v = 0) : _value(v) {}
    term *derivative();
    Type  type()                const { return TYPE_SCALAR; }
    void  set_value(float v)          { _value = v; }
    float get_value()           const { return _value; }
};

class scalar_0 : public scalar
{
public:
    inline scalar_0() : scalar(0) {}
};

class scalar_1 : public scalar
{
public:
    inline scalar_1() : scalar(1) {}
};

term *variable::derivative() { return new scalar_1(); }
term *scalar::derivative()   { return new scalar_0(); }


/*
class PairUtil
{
public:

    static Object *duplicate(Object *item, int times)
    {
        Object *a = NULL, *p = NULL;
        
        for (int i = 0; i < times; i++)
        {
            if (a == NULL)
            {
                a = getPair(item, NULL);
                p = a;
            }
            else
            {
                setTail(p, getPair(item, NULL));
                p = getTail(p);
            }
        }
        
        return a;
    }
    static Object *copyList(Object *a)
    {
        Object *b = NULL, *p = NULL;
        
        if (isNil(a))       return NULL;
        else if (isAtom(a)) return getCopy(a);
        else
        {
            while (!isNil(a) && !isAtom(a))
            {
                if (b == NULL)
                    p = b = getPair(getHead(a), NULL);
                else
                {
                    setTail(p, getPair(getHead(a), NULL));
                    p = getTail(p);
                }
                a = getTail(a);
            }
        }
        return b;
    }
    static Object *copyTree(Object *a)
    {
        Object *b = NULL, *p = NULL;
        
        if (isNil(a))        return NULL;
        else if (isAtom(a))  return getCopy(a);
        else
        {
            while (!isNil(a) && !isAtom(a))
            {
                if (b == NULL)
                    p = b = getPair(copyTree(getHead(a)), NULL);
                else
                {
                    setTail(p, getPair(copyTree(getHead(a)), NULL));
                    p = getTail(p);
                }
                a = getTail(a);
            }
        }
        return b;
    }
    inline static Object *setAssoc(Object *key, Object *value, Object *list)
    {
        return getPair(getPair(key, value), list);
    }

    static Object *getAssoc(Object *key, Object *list)
    {
        if (!isPair(list))
            return NULL;
        
        Object *first = NULL;
        while (list != NULL)
        {
            first = getHead(list);
            if (!isNil(first) && isList(first))
                if (isInstanceEqual(getHead(first), key))
                    return getHead(getTail(first));
        }
        return NULL;
    }
    inline static Object *getTailAt(Object *e, int n)
    {
        while (e != NULL && n > 0)
        {
            e = getTail(e);
            n--;
        }
        return e;
    }
    inline static Object *getAt(Object *e, int n)
    {
        e = getTailAt(e, n);
        return e == NULL ? NULL : getHead(e);
    }
    static Object *getSublist(Object *list, Object *find)
    {
        if (list->compare(find) == 0)
            return list;
        
        while (list != NULL && !list->compare(find) == 0)
            list = (IPair *)getTail(list);
        
        return list;
    }
    static Object *append(Object *a, Object *b, bool destructive = false)
    {
        if (isNil(a)) return b;
        if (isNil(b)) return a;
        
        return destructive ? m_destructiveAppend(a, b) : m_append(a, b);
    }
    
    // private ..
    static Object *m_append(Object *a, Object *c)
    {
        Object *b = NULL, *p = NULL;
        
        if (isAtom(c))
            c = getPair(c, NULL);
        if (isAtom(a))
            return getPair(a, c);
        
        else
        {
            while (!isNil(a) && !isAtom(a))
            {
                if (b == NULL)
                    p = b = getPair(getHead(a), NULL);
                else
                {
                    setTail(p, getPair(getHead(a), NULL));
                    p = getTail(p);
                }
                a = getTail(a);
            }
        }
        
        setTail(p, c);
        return b;
    }
    static Object *m_destructiveAppend(Object *a, Object *b)
    {
        if (isAtom(a))
        {
            if (isAtom(b))
                return getPair(a, getPair(b, NULL));
            else
                return getPair(a, b);
        }
        else
        {
            if (isAtom(b))
            {
                setTail(getLast(a), getPair(b, NULL));
                return a;
            }
            else
            {
                setTail(getLast(a), b);
                return a;
            }
        }
    }
};
 */

}


}
