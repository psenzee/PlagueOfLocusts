#pragma once

#include "object.h"
#include "reader.h"
#include <vector>
#include <memory>

struct collection : public object
{
    collection() : object(COLLECTION) {}
    
    std::size_t write(binary_out_stream &os) const;
    std::size_t read(binary_in_stream &is, bool read_type = true);
    
    void add(std::unique_ptr<object> obj) { _objects.emplace_back(std::move(obj)); }

private:

    std::vector< std::unique_ptr<object> > _objects;
};
