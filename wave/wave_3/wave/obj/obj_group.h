#pragma once

#include "face.h"
#include "obj_face.h"
#include "obj_index.h"

struct obj_group
{
    obj_group(std::string_view name, const face_list &faces) : _name(name)
    {
        _face_list(faces);
    }
    
    void set_material(std::string_view material)
    {
        _material = material;
    }

    ~obj_group()
    {
        for (auto f: _faces) delete f;
        _faces.clear();
    }

    std::ostream &print(const obj_option &option, int smooth_group, std::ostream &s) const
    {
        if (!_name.empty()) {
            s << "g " << _name << "\n\n";
        }
        if (!_material.empty()) {
            s << "usemtl " << _material << "\n\n";
        }
        if (!option.smooth) {
            s << "s off" << std::endl << "\n";
        }
        else if (!option.normal) {
            s << "s " << smooth_group << "\n\n";
        }
        _index.print(option, s);
        s << "\n";
        std::size_t count = 0;
        for (const auto &f: _faces) {
            f->print(option, s);
            count++;
        }
        s << "# " << count << " faces\n\n";
        return s;
    }

private:
    
    std::string             _name,
                            _material;
    obj_index               _index;
    std::vector<obj_face *> _faces;

    void _face(const face &face, obj_face &oface)
    {
        for (const auto &v: face.vertices) {
            oface.insert(_index.insert(v));
        }
    }

    void _face_list(const face_list &flist)
    {
        for (const auto &f: flist.faces)
        {
            obj_face *face = new obj_face;
            _face(f, *face);
            _faces.push_back(face);
        }
    }
};
