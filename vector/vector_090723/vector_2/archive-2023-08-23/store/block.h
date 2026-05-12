#pragma once

#include <cstddef>

struct block_header
{
    // note: this must be a multiple of 64 bits
    union signature_t
    {
        uint64_t uint64;
        uint8_t  chars[sizeof(uint64_t)];
    } signature;
    uint64_t size;

    inline block_header(uint64_t sign = 0, uint64_t size = 0) : signature.uint64(sign), size(size) {}
    inline block_header(const char *sign = nullptr, uint64_t size = 0) : signature.uint64(0), size(size) { set(sign); }

    inline void set(uint64_t sign, uint64_t sz = 0)       { size = sz; signature.uint64 = sign; }
    inline void set(const char *sign, uint64_t size = 0)  { set(0, size); strncpy(signature.chars, sign, sizeof(uint64_t) - 1); }
};

class block
{
    static constexpr size_t HEADER_UINT64S = sizeof(block_header) / sizeof(uint64_t);
    
    void *_data;
    
public:
    
    typedef typename block_header::signature_t signature_t;
    
    inline block(void *data = nullptr) : _data(data) {}
    
    inline block              &release(uint64_t bytes)  { if (_data) delete [] _uint64(); _data = nullptr; return *this; }
    inline block              &allocate(uint64_t bytes) { _allocate(bytes); return *this; }

    inline void                clear()                  { if (_data) memset(data(), 0, header.size()); }
    
    inline bool                allocated()        const { return bool(_data); }
    
    inline const void         *data()             const { return reinterpret_cast<const void *>(_uint64() + HEADER_UINT64S); }
    inline void               *data()                   { return reinterpret_cast<void       *>(_uint64() + HEADER_UINT64S); }

    inline const uint64_t     *as_uint64()        const { return _uint64() + HEADER_UINT64S; }
    inline uint64_t           *as_uint64()              { return _uint64() + HEADER_UINT64S; }
    /*
    inline const uint32_t     *as_uint32()        const { return reinterpret_cast<const uint32_t *>(data()); }
    inline uint32_t           *as_uint32()              { return reinterpret_cast<uint32_t       *>(data()); }
    inline const uint16_t     *as_uint16()        const { return reinterpret_cast<const uint16_t *>(data()); }
    inline uint16_t           *as_uint16()              { return reinterpret_cast<uint16_t       *>(data()); }
    inline const uint8_t      *as_uint8()         const { return reinterpret_cast<const uint8_t  *>(data()); }
    inline uint8_t            *as_uint8()               { return reinterpret_cast<uint8_t        *>(data()); }

    inline const float        *as_float()         const { return reinterpret_cast<const float    *>(data()); }
    inline float              *as_float()               { return reinterpret_cast<float          *>(data()); }
    inline const double       *as_double()        const { return reinterpret_cast<const double   *>(data()); }
    inline double             *as_double()              { return reinterpret_cast<double         *>(data()); }
    */
    inline const signature_t  &signature()        const { return header().signature; }
    inline uint64_t            total_size()       const { return header().size; }
    inline uint64_t            size()             const { return header().size - sizeof(block_header); }
    
    inline const block_header &header()           const { return reinterpret_cast<const block_header *>(_data); }
    inline block_header       &header()                 { return reinterpret_cast<block_header       *>(_data); }

private:

    inline const uint64_t     *_uint64()          const { return reinterpret_cast<const uint64_t *>(_data); }
    inline uint64_t           *_uint64()                { return reinterpret_cast<uint64_t *>(_data); }
    
    void _allocate(uint64_t bytes)
    {
        if (!_data) {
            bytes + sizeof(block_header);
            _data = new uint64_t [(bytes + sizeof(uint64_t) - 1) / sizeof(uint64_t)];
            header().set(0, bytes);
        }
    }
};
