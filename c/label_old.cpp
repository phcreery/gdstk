/*
Copyright 2020 Lucas Heitzmann Gabrielli.
This file is part of gdstk, distributed under the terms of the
Boost Software License - Version 1.0.  See the accompanying
LICENSE file or <http://www.boost.org/LICENSE_1_0.txt>
*/

#include "label.h"
#include "../include/gdstk/label.hpp"
#include "../include/gdstk/utils.hpp"

extern "C" {

// Forward declaration of error conversion function (defined in cell.cpp)
extern ErrorCode convert_error_code(gdstk::ErrorCode cpp_error);

// Label functions
Label* label_new(const char* text, Vec2 origin, double rotation, uint32_t layer, uint32_t texttype) {
    if (!text) return nullptr;
    
    gdstk::Label* cpp_label = new gdstk::Label();
    cpp_label->text = gdstk::copy_string(text, NULL);
    cpp_label->origin = *reinterpret_cast<const gdstk::Vec2*>(&origin);
    cpp_label->rotation = rotation;
    cpp_label->tag = gdstk::make_tag(layer, texttype);
    return reinterpret_cast<Label*>(cpp_label);
}

void label_free(Label* label) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->clear();
        delete cpp_label;
    }
}

void label_clear(Label* label) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->clear();
    }
}

void label_copy_from(Label* label, const Label* source) {
    if (label && source) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        const gdstk::Label* cpp_source = reinterpret_cast<const gdstk::Label*>(source);
        cpp_label->copy_from(*cpp_source);
    }
}

void label_print(const Label* label) {
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        cpp_label->print();
    }
}

// Property access functions
uint32_t label_layer(const Label* label) {
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        return gdstk::get_layer(cpp_label->tag);
    }
    return 0;
}

uint32_t label_texttype(const Label* label) {
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        return gdstk::get_type(cpp_label->tag);
    }
    return 0;
}

Vec2 label_origin(const Label* label) {
    Vec2 result = {0.0, 0.0};
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        result.x = cpp_label->origin.x;
        result.y = cpp_label->origin.y;
    }
    return result;
}

double label_rotation(const Label* label) {
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        return cpp_label->rotation;
    }
    return 0.0;
}

void label_set_layer(Label* label, uint32_t layer) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        gdstk::set_layer(cpp_label->tag, layer);
    }
}

void label_set_texttype(Label* label, uint32_t texttype) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        gdstk::set_type(cpp_label->tag, texttype);
    }
}

void label_set_origin(Label* label, Vec2 origin) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->origin.x = origin.x;
        cpp_label->origin.y = origin.y;
    }
}

void label_set_rotation(Label* label, double rotation) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->rotation = rotation;
    }
}

void label_set_text(Label* label, const char* text) {
    if (label && text) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        if (cpp_label->text) {
            gdstk::free_allocation(cpp_label->text);
        }
        cpp_label->text = gdstk::copy_string(text, NULL);
    }
}

const char* label_text(const Label* label) {
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        return cpp_label->text;
    }
    return nullptr;
}

void label_set_text(Label* label, const char* text) {
    if (label && text) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->text = text;
    }
}

Vec2 label_origin(const Label* label) {
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        return *reinterpret_cast<const Vec2*>(&cpp_label->origin);
    }
    return {0, 0};
}

void label_set_origin(Label* label, Vec2 origin) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->origin = *reinterpret_cast<const gdstk::Vec2*>(&origin);
    }
}

double label_rotation(const Label* label) {
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        return cpp_label->rotation;
    }
    return 0.0;
}

void label_set_rotation(Label* label, double rotation) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->rotation = rotation;
    }
}

double label_magnification(const Label* label) {
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        return cpp_label->magnification;
    }
    return 1.0;
}

void label_set_magnification(Label* label, double magnification) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->magnification = magnification;
    }
}

bool label_x_reflection(const Label* label) {
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        return cpp_label->x_reflection;
    }
    return false;
}

void label_set_x_reflection(Label* label, bool x_reflection) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->x_reflection = x_reflection;
    }
}

Tag label_tag(const Label* label) {
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        return cpp_label->tag;
    }
    return 0;
}

void label_set_tag(Label* label, Tag tag) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->tag = tag;
    }
}

uint32_t label_layer(const Label* label) {
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        return gdstk::get_layer(cpp_label->tag);
    }
    return 0;
}

uint32_t label_texttype(const Label* label) {
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        return gdstk::get_type(cpp_label->tag);
    }
    return 0;
}

void label_set_layer(Label* label, uint32_t layer) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        gdstk::set_layer(cpp_label->tag, layer);
    }
}

void label_set_texttype(Label* label, uint32_t texttype) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        gdstk::set_type(cpp_label->tag, texttype);
    }
}

void label_translate(Label* label, Vec2 displacement) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->translate(*reinterpret_cast<const gdstk::Vec2*>(&displacement));
    }
}

void label_scale(Label* label, double scaling, Vec2 center) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->scale(scaling, *reinterpret_cast<const gdstk::Vec2*>(&center));
    }
}

void label_mirror(Label* label, Vec2 p1, Vec2 p2) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->mirror(*reinterpret_cast<const gdstk::Vec2*>(&p1), 
                         *reinterpret_cast<const gdstk::Vec2*>(&p2));
    }
}

void label_rotate(Label* label, double angle, Vec2 center) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->rotate(angle, *reinterpret_cast<const gdstk::Vec2*>(&center));
    }
}

void label_transform(Label* label, double magnification, bool x_reflection, double rotation, Vec2 origin) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->transform(magnification, x_reflection, rotation, 
                            *reinterpret_cast<const gdstk::Vec2*>(&origin));
    }
}

void label_apply_repetition(Array* result, const Label* label) {
    if (result && label) {
        gdstk::Array<gdstk::Label>* cpp_result = reinterpret_cast<gdstk::Array<gdstk::Label>*>(result);
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        cpp_label->apply_repetition(*cpp_result);
    }
}

ErrorCode label_to_gds(const Label* label, FILE* out, double scaling) {
    if (label && out) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        gdstk::ErrorCode result = cpp_label->to_gds(out, scaling);
        return convert_error_code(result);
    }
    return GDSTK_INVALID_FILE;
}

ErrorCode label_to_svg(const Label* label, FILE* out, double scaling, uint32_t precision) {
    if (label && out) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        gdstk::ErrorCode result = cpp_label->to_svg(out, scaling, precision);
        return convert_error_code(result);
    }
    return GDSTK_INVALID_FILE;
}

} // extern "C"
