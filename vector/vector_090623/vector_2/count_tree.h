#pragma once

#include <cstddef>
#include <iostream>
#include <array>
#include <limits>
#include <algorithm>

template <size_t N, typename ValueT = float>
struct _node
{
    typedef ValueT                  value_t;
    typedef _node<N, ValueT>        self_t;
    typedef std::array<self_t *, N> set_t;

    inline _node(uint32_t index = 0) : _index(index), _count(0), _child_count(0), _leaf_count(0), _children({ nullptr }),
                                       _min(std::numeric_limits<value_t>::max()), _max(std::numeric_limits<value_t>::min()), _sum(0){}
    inline ~_node() { for (auto u : _children) delete u; _children = { 0 }; }

    inline self_t        *next(uint32_t index)               { return _children[index % N]; }
    inline const self_t  *next(uint32_t index)         const { return _children[index % N]; }
    inline self_t        *tally(value_t value)               { _min = std::min(_min, value); _max = std::max(_max, value); _sum += value; _leaf_count++; return this; }
    inline self_t        *increment()                        { _count++; return this; }
    inline self_t        *force_next(uint32_t index)         { size_t i(index % N); auto *n = _children[i]; if (n) return n; _child_count++; return _children[i] = new self_t(index); }
    inline size_t         child_count()                const { return _child_count; }
    inline size_t         calculate_child_count()      const { size_t count = 0; for (size_t i = 0; i < N; i++) count += _children[i] ? 1 : 0; return count; }

    size_t child_count_recursive() const
    {
        size_t cc = child_count();
        for (size_t i = 0; i < N; i++)
            if (_children[i])
                cc += _children[i]->child_count_recursive();
        return cc;
    }
    
    std::ostream &print(std::ostream &os, size_t minimum, size_t &count, bool singular = false, size_t depth = 0) const
    {
        if (_count < minimum)
            return os;
        _print_one(os, singular ? 0 : depth);
        count++;
        size_t cc = child_count();
        if (cc == 1) {
            for (size_t i = 0; i < N; i++) {
                if (_children[i]) {
                    size_t child_count = 0;
                    _children[i]->print(os, minimum, child_count, true, depth + 1);
                    count += child_count;
                }
            }
        } else {
            os << std::endl;
            for (size_t i = 0; i < N; i++) {
                if (_children[i]) {
                    size_t child_count = 0;
                    _children[i]->print(os, minimum, child_count, false, depth + 1);
                    count += child_count;
                }
            }
        }
        return os;
    }

private:

    uint32_t _index, _count, _leaf_count, _child_count;
    value_t  _min, _max, _sum;
    set_t    _children;
    
    static std::ostream &_indent(std::ostream &os, size_t depth) { for (size_t i = 0; i < depth; i++) os << " "; return os; }
    
    std::ostream &_print_counts(std::ostream &os) const
    {
        if (_count > 1) {
            os << " [" << _count << " " << _child_count << " " << child_count_recursive();
            if (_leaf_count > 0) os << " " << _leaf_count;
            os << "]";
        }
        return os;
    }
    
    std::ostream &_print_values(std::ostream &os) const
    {
        if (_leaf_count) {
            os << "(" << _min;
            if (_min != _max) os << _min << "-" << ".." << _max << " sum " << _sum;
            os << ") ";
        }
        return os;
    }

    std::ostream &_print_one(std::ostream &os, size_t depth = 0) const
    {
        _indent(os, depth);
        _print_values(_print_counts(os << "#" << _index) << " ");
        return os;
    }
};

template <size_t N, typename ValueT = float>
struct _node_iterator
{
    typedef ValueT                    value_t;
    typedef _node<N, ValueT>          node_t;
    typedef _node_iterator<N, ValueT> self_t;
    
    size_t  _depth;
    node_t *_root, *_current;
    
    inline _node_iterator(node_t *root)  : _depth(0), _root(root), _current(root) {}
    inline _node_iterator(self_t &other) : _depth(_depth), _root(_root), _current(_current) {}

    inline node_t *advance(uint32_t index)     { node_t *n = _depth >= N ? nullptr : _current->_next(index); if (n) { _current = n; _depth++; } return n; }
    inline node_t *append(uint32_t index)      { if (_depth < N) { _current = _current->force_next(index); _depth++; } return _current; }

    inline node_t *increment()                 { return _current->increment(); }
    inline node_t *tally(value_t value)        { return _current->tally(value); }
    
    inline size_t  count()               const { return _current->_count; }
    inline value_t minimum()             const { return _current->_min; }
    inline value_t maximum()             const { return _current->_max; }
    inline value_t sum()                 const { return _current->_sum; }
};

template <size_t N, typename ValueT = float>
struct _const_node_iterator
{
    typedef ValueT                    value_t;
    typedef _node<N, ValueT>          node_t;
    typedef _node_iterator<N, ValueT> self_t;
    
    mutable size_t _depth;
    const node_t *_root, *_current;
    
    inline _const_node_iterator(const node_t *root)  : _depth(0), _root(root), _current(root) {}
    inline _const_node_iterator(const self_t &other) : _depth(_depth), _root(_root), _current(_current) {}

    inline const node_t *advance(size_t index) { node_t *n = _depth >= N ? nullptr : _current->_next(index); if (n) { _current = n; _depth++; } return n; }
    
    inline size_t  count()   const { return _current->_count; }
    inline value_t minimum() const { return _current->_min; }
    inline value_t maximum() const { return _current->_max; }
    inline value_t sum()     const { return _current->_sum; }
};

template <size_t N, typename ValueT = float>
struct count_tree
{
    typedef ValueT                          value_t;
    typedef _node<N, ValueT>                node_t;
    typedef count_tree<N, ValueT>           self_t;
    typedef _node_iterator<N, ValueT>       iterator_t;
    typedef _const_node_iterator<N, ValueT> const_iterator_t;
    
    node_t *_root;
    
    inline count_tree()  { _root = new node_t; }
    inline ~count_tree() { delete _root; _root = 0; }
    
    inline iterator_t       iterator()       { return iterator_t(_root); }
    inline const_iterator_t iterator() const { return const_iterator_t(_root); }
    
    inline self_t &prune(size_t minimum = 2) { _root->prune(minimum); return *this; }
    
    inline std::ostream &print(std::ostream &os, size_t minimum = 2) const
    {
        os << std::endl << "-----" << std::endl;
        size_t count = 0;
        return _root->print(os, minimum, count) << "printed count " << count << std::endl;
    }
};

template <size_t N>
void test_count_tree()
{
    // we don't necessarily need a tree ---
    // we could just:
    // 1. sort by norm_frexp, then ranks from the index
    // 2. n = 0
    // 3. count distribution by rank n
    // 4. n++ recursively descend
    // or do everything as if normalized??
    
    typedef count_tree<N, float> tree_t;
    typedef typename tree_t::iterator_t iterator_t;
    tree_t     t;
    iterator_t it(t.iterator());
    for (size_t i = 0; i < 1024 * 1024; i++) {
        //if ((rand() % 7) == 0) {
        if ((i % 7) == 6) {
            it.tally(rand() * 0.00001);
            it = t.iterator();
            it.increment();
        } else {
            it.append(rand() % N);
            it.increment();
        }
    }
    t.print(std::cout, 0);
}
