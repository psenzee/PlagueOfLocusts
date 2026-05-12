#include <cstring>
#include <cstdio>
#include <vector>
#include <map>

#include "bundle.h"
#include "file.h"
#include "paths.h"

struct bundle_index_entry
{
    uint32_t           entry_size;
    uint32_t           file_offset;
//  char               string[entry_size - 8];
};

struct bundle_index_header
{
    char               tag[4]; // = "INDX"
    uint32_t           size;
    uint32_t           entry_count;
//  bundle_index_entry entries[entry_count];
};

struct bundle_file
{
    char             tag[4];
    uint32_t         size;
//  char             data[size - 4]; // ...
};

static ssize_t get_size_from_file_offset(const uint8_t *file)
{
    if (!file) return -1;
    return *(const int32_t *)(file - sizeof(int32_t)) - sizeof(int32_t);
}

static bool validate(const uint8_t *file)
{
    if (!file) return false;
    char tag[] = "FILE";
    return memcmp(file - 8, tag, 4) == 0 && get_size_from_file_offset(file) >= 0;
}

std::string_view bundle::get_file_name() const
{
    return _filename.c_str();
}

void bundle::test()
{
    for (index_t::const_iterator i = _index.begin(), e = _index.end(); i != e; ++i) {
        printf("%p:%zd [%s]\n", (*i).second, get_size_from_file_offset((*i).second), (*i).first.c_str());
    }
}

void bundle::unload()
{
    _filename = "";
    if (_data.data) {
        delete [] (char *)_data.data;
    }
    _data.data = 0;
    _data.size = 0;
    _index.clear();
}

bundle::~bundle()
{
    unload();
}

bool bundle::load(std::string_view filename)
{
    unload();
    _filename = filename;
    _data = file::read_file(filename.data());
    if (!_read_index()) {
        unload();
        return false;
    }
    return true;
}

const uint8_t *bundle::get_file(std::string_view filename) const
{
    std::string fn(filename);
    index_t::const_iterator found = _index.find(fn);
    if (found == _index.end()) {
        return 0;
    }
    if (!validate((*found).second)) {
        printf("bundle for file '%s' appears corrupt!\n", filename.data());
        return 0;
    }
    return (*found).second;
}

ssize_t bundle::get_size(std::string_view filename) const
{
    return get_size_from_file_offset(get_file(filename));
}

bool bundle::_read_index()
{
    bundle_index_header header;
    memcpy(&header, _data.data, sizeof(header));
    if (memcmp(header.tag, "INDX", sizeof(header.tag)) != 0) {
        // failure..
        return false;
    }
    char *p = (char *)_data.data + sizeof(header);
    char index_name[1024];
    for (int i = 0; i < header.entry_count; i++)
    {
        bundle_index_entry entry;
        memcpy(&entry, p, sizeof(entry));
        memset(index_name, 0, sizeof(index_name));
        memcpy(index_name, p + sizeof(entry), entry.entry_size - sizeof(uint32_t));
        //printf("name: %s\n", index_name);
        p += entry.entry_size + sizeof(uint32_t);
        _index[index_name] = (const uint8_t *)_data.data + entry.file_offset + sizeof(uint32_t) + sizeof(uint32_t);
    }
    return true;
}
