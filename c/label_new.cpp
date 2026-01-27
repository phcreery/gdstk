#include "label.h"
#include "../include/gdstk/label.hpp"
#include <cstdlib>
#include <cstring>

// Label C wrapper functions

Label* label_new(const char* text, Vec2 origin, double rotation, uint32_t layer, uint32_t texttype) {
    gdstk::Label* cpp_label = static_cast<gdstk::Label*>(malloc(sizeof(gdstk::Label)));
    if (!cpp_label) return nullptr;
    
    // Initialize the C++ object in-place
    new(cpp_label) gdstk::Label();
    
    // Set properties
    if (text) {
        cpp_label->text = (char*)malloc(strlen(text) + 1);
        if (cpp_label->text) {
            strcpy(cpp_label->text, text);
        }
    }
    
    cpp_label->origin.x = origin.x;
    cpp_label->origin.y = origin.y;
    cpp_label->rotation = rotation;
    cpp_label->tag.layer = layer;
    cpp_label->tag.type = texttype;
    
    return reinterpret_cast<Label*>(cpp_label);
}

void label_free(Label* label) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->~Label(); // Call destructor
        free(cpp_label);
    }
}

void label_clear(Label* label) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        if (cpp_label->text) {
            free(cpp_label->text);
            cpp_label->text = nullptr;
        }
    }
}

void label_print(const Label* label) {
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        // Note: gdstk::Label::print() is non-const, so we can't call it here safely
        // This is a limitation in the GDSTK C++ API
        printf("Label: text=%s, origin=(%g, %g), layer=%u, texttype=%u\n", 
               cpp_label->text ? cpp_label->text : "", 
               cpp_label->origin.x, cpp_label->origin.y,
               cpp_label->tag.layer, cpp_label->tag.type);
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
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        if (cpp_label->text) {
            free(cpp_label->text);
        }
        if (text) {
            cpp_label->text = (char*)malloc(strlen(text) + 1);
            if (cpp_label->text) {
                strcpy(cpp_label->text, text);
            }
        } else {
            cpp_label->text = nullptr;
        }
    }
}

Vec2 label_origin(const Label* label) {
    Vec2 result = {0, 0};
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        result.x = cpp_label->origin.x;
        result.y = cpp_label->origin.y;
    }
    return result;
}

void label_set_origin(Label* label, Vec2 origin) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->origin.x = origin.x;
        cpp_label->origin.y = origin.y;
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

uint32_t label_layer(const Label* label) {
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        return cpp_label->tag.layer;
    }
    return 0;
}

uint32_t label_texttype(const Label* label) {
    if (label) {
        const gdstk::Label* cpp_label = reinterpret_cast<const gdstk::Label*>(label);
        return cpp_label->tag.type;
    }
    return 0;
}

void label_set_layer(Label* label, uint32_t layer) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->tag.layer = layer;
    }
}

void label_set_texttype(Label* label, uint32_t texttype) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        cpp_label->tag.type = texttype;
    }
}

void label_transform(Label* label, double magnification, bool x_reflection, 
                    double rotation, Vec2 origin) {
    if (label) {
        gdstk::Label* cpp_label = reinterpret_cast<gdstk::Label*>(label);
        gdstk::Vec2 cpp_origin = {origin.x, origin.y};
        cpp_label->transform(magnification, x_reflection, rotation, cpp_origin);
    }
}
