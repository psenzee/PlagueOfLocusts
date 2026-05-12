#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include "obj_option.h"
#include "face_list.h"

struct obj_group;

struct obj_file
{
    ~obj_file();

    void          add_group(std::string_view name, std::string_view material, const face_list &faces);
    std::ostream &write(std::ostream &s) const;
    obj_file     &set_materials(std::string_view library);
    obj_file     &set_options(const obj_option &option);
    void          write_file(std::string_view filename) const;
    
private:

    std::vector<obj_group *> _groups;
    std::string              _library;
    obj_option               _option;
};
