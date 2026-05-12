#include "obj_file.h"
#include "obj_group.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>

obj_file::~obj_file()
{
    for (auto g: _groups) {
        delete g;
    }
    _groups.clear();
}

void obj_file::add_group(std::string_view name, std::string_view material, const face_list &faces)
{
    obj_group *group = new obj_group(name, faces);
    if (!material.empty()) {
        group->set_material(material);
    }
    _groups.push_back(group);
}

obj_file &obj_file::set_materials(std::string_view library)
{
    _library = library;
    return *this;
}

obj_file &obj_file::set_options(const obj_option &option)
{
    _option = option;
    return *this;
}

std::ostream &obj_file::write(std::ostream &s) const
{
    int sgroup = 1;
    for (auto g: _groups) {
        g->print(_option, sgroup++, s);
    }
    return s;
}

void obj_file::write_file(std::string_view filename) const
{
    std::ofstream file(filename, std::ios::trunc);
    if (!file) {
        throw std::runtime_error("failed to create file");
    }
    if (!_library.empty()) {
        file << "mtllib " << _library << "\n";
    }
    write(file);
    file.close();
}
