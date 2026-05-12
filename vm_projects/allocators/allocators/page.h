#pragma once

#include <cstring>
#include <cstdint>
#include <cstddef>
#include <algorithm>

class page
{
    struct free_item { uint32_t magic, free_next; };

    static constexpr const uint32_t FREE_MAGIC = 0x0A11F0EE;

    struct header_t { page *previous, *next; uint32_t allocated, free_start, item_size, item_count; };

    char *_base;

    char            *data()                            { return _base + sizeof(header_t); }
    const char      *data()                      const { return _base + sizeof(header_t); }
    header_t        *header()                          { return reinterpret_cast<header_t *>(_base); }
    const header_t  *header()                    const { return reinterpret_cast<const header_t *>(_base); }
    free_item       *as_free_item(void *u)             { return reinterpret_cast<free_item *>(u); }
    const free_item *as_free_item(const void *u) const { return reinterpret_cast<const free_item *>(u); }

    uint32_t         index(const void *u)        const
    {
        auto v = reinterpret_cast<const char *>(u);
        return static_cast<uint32_t>((v - (_base + sizeof(header_t))) / header()->item_size);
    }

public:

    page(uint32_t item_count, uint32_t item_size)
    {
        size_t fsz(item_count * item_size + sizeof(header_t));
        _base = reinterpret_cast<char *>(::malloc(fsz));
        memset(_base, 0,  + sizeof(header_t));
        auto *h = header();
        h->free_start = 0;
        item_size = std::max(item_size, static_cast<uint32_t>(sizeof(free_item)));
        h->item_size = item_size;
        h->item_count = item_count;
        free_item *fi = nullptr;
        auto *d = data();
        for (uint32_t i = 0; i < item_count; i++) {
            fi = as_free_item(d);
            d += item_size;
            fi->magic = FREE_MAGIC;
            fi->free_next = i + 1;
        }
        fi->free_next = ~0u;
    }
    ~page() { ::free(_base); }

    void *allocate()
    {
        auto *h = header();
        uint32_t f = h->free_start;
        if (f == ~0u) {
            // it's full
            return nullptr;
        }
        char *data = _base + sizeof(header_t) + f * h->item_size;
        free_item *fi = as_free_item(data);
        h->free_start = fi->free_next;
        h->allocated++;
        fi->magic = 0;
        return data;
    }

    void deallocate(void *u)
    {
        auto *h = header();
        auto *d = _base + sizeof(header_t);
        if (u < d || u > d + size() || !h->allocated) { // guards
            // throw exception or something
            return;
        }
        free_item *fi = as_free_item(u);
        if (fi->magic == FREE_MAGIC) {
            // throw exception or something
            return;
        }
        fi->magic = FREE_MAGIC;
        fi->free_next = h->free_start;
        h->free_start = index(u);
        h->allocated--;
    }

    bool        is_free(const void *u) const { return as_free_item(u)->magic == FREE_MAGIC; }
    bool        is_full()              const { return header()->free_start == ~0u; }
    bool        is_empty()             const { return header()->allocated == 0; }
    size_t      size()                 const { return header()->item_size * header()->item_count; }
    size_t      full_size()            const { return size() + sizeof(header_t); }
    size_t      item_size()            const { return header()->item_size; }
    size_t      item_count()           const { return header()->item_count; }

    void        set_previous(page *p)        { header()->previous = p; }
    void        set_next(page *p)            { header()->next = p; }
    page       *get_previous()               { return header()->previous; }
    page       *get_next()                   { return header()->next; }
    const page *get_previous()         const { return header()->previous; }
    const page *get_next()             const { return header()->next; }
};
