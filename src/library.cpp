/*
Copyright 2020 Lucas Heitzmann Gabrielli.
This file is part of gdstk, distributed under the terms of the
Boost Software License - Version 1.0.  See the accompanying
LICENSE file or <http://www.boost.org/LICENSE_1_0.txt>
*/

#include "library.h"

#include <zlib.h>

#include <cfloat>
#include <cinttypes>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>

#include "allocator.h"
#include "cell.h"
#include "flexpath.h"
#include "gdsii.h"
#include "label.h"
#include "map.h"
#include "oasis.h"
#include "polygon.h"
#include "rawcell.h"
#include "reference.h"
#include "utils.h"
#include "vec.h"

namespace gdstk {

struct ByteArray {
    uint64_t size;
    uint8_t* bytes;
    Property* properties;
};

void Library::print(bool all) const {
    printf("Library <%p> %s, unit %lg, precision %lg, %" PRIu64 " cells, %" PRIu64
           " raw cells, owner <%p>\n",
           this, name, unit, precision, cell_array.size, rawcell_array.size, owner);
    if (all) {
        for (uint64_t i = 0; i < cell_array.size; i++) {
            printf("{%" PRIu64 "} ", i);
            cell_array[i]->print(true);
        }
        for (uint64_t i = 0; i < rawcell_array.size; i++) {
            printf("{%" PRIu64 "} ", i);
            rawcell_array[i]->print(true);
        }
    }
    properties_print(properties);
}

void Library::copy_from(const Library& library, bool deep_copy) {
    uint64_t len;
    name = copy_string(library.name, len);
    unit = library.unit;
    precision = library.precision;
    if (deep_copy) {
        cell_array.capacity = library.cell_array.capacity;
        cell_array.size = library.cell_array.size;
        cell_array.items = (Cell**)allocate(sizeof(Cell*) * cell_array.capacity);
        Cell** src = library.cell_array.items;
        Cell** dst = cell_array.items;
        for (uint64_t i = 0; i < library.cell_array.size; i++, src++, dst++) {
            *dst = (Cell*)allocate_clear(sizeof(Cell));
            (*dst)->copy_from(**src, NULL, true);
        }
    } else {
        cell_array.copy_from(library.cell_array);
    }
    // raw cells should be immutable, so there's no need to perform a deep copy
    rawcell_array.copy_from(library.rawcell_array);
}

void Library::top_level(Array<Cell*>& top_cells, Array<RawCell*>& top_rawcells) const {
    Map<Cell*> cell_deps = {0};
    Map<RawCell*> rawcell_deps = {0};
    cell_deps.resize(cell_array.size * 2);
    rawcell_deps.resize(rawcell_array.size * 2);

    Cell** c_item = cell_array.items;
    for (uint64_t i = 0; i < cell_array.size; i++, c_item++) {
        Cell* cell = *c_item;
        cell->get_dependencies(false, cell_deps);
        cell->get_raw_dependencies(false, rawcell_deps);
    }

    RawCell** r_item = rawcell_array.items;
    for (uint64_t i = 0; i < rawcell_array.size; i++) {
        (*r_item++)->get_dependencies(false, rawcell_deps);
    }

    c_item = cell_array.items;
    for (uint64_t i = 0; i < cell_array.size; i++) {
        Cell* cell = *c_item++;
        if (cell_deps.get(cell->name) != cell) top_cells.append(cell);
    }

    r_item = rawcell_array.items;
    for (uint64_t i = 0; i < rawcell_array.size; i++) {
        RawCell* rawcell = *r_item++;
        if (rawcell_deps.get(rawcell->name) != rawcell) top_rawcells.append(rawcell);
    }
}

void Library::write_gds(const char* filename, uint64_t max_points, std::tm* timestamp) const {
    FILE* out = fopen(filename, "wb");
    if (out == NULL) {
        fputs("[GDSTK] Unable to open GDSII file for output.\n", stderr);
        return;
    }

    uint64_t len = strlen(name);
    if (len % 2) len++;
    if (!timestamp) {
        time_t now = time(NULL);
        timestamp = localtime(&now);
    }
    uint16_t buffer_start[] = {6,
                               0x0002,
                               0x0258,
                               28,
                               0x0102,
                               (uint16_t)(timestamp->tm_year + 1900),
                               (uint16_t)(timestamp->tm_mon + 1),
                               (uint16_t)timestamp->tm_mday,
                               (uint16_t)timestamp->tm_hour,
                               (uint16_t)timestamp->tm_min,
                               (uint16_t)timestamp->tm_sec,
                               (uint16_t)(timestamp->tm_year + 1900),
                               (uint16_t)(timestamp->tm_mon + 1),
                               (uint16_t)timestamp->tm_mday,
                               (uint16_t)timestamp->tm_hour,
                               (uint16_t)timestamp->tm_min,
                               (uint16_t)timestamp->tm_sec,
                               (uint16_t)(4 + len),
                               0x0206};
    big_endian_swap16(buffer_start, COUNT(buffer_start));
    fwrite(buffer_start, sizeof(uint16_t), COUNT(buffer_start), out);
    fwrite(name, 1, len, out);

    uint16_t buffer_units[] = {20, 0x0305};
    big_endian_swap16(buffer_units, COUNT(buffer_units));
    fwrite(buffer_units, sizeof(uint16_t), COUNT(buffer_units), out);
    uint64_t units[] = {gdsii_real_from_double(precision / unit),
                        gdsii_real_from_double(precision)};
    big_endian_swap64(units, COUNT(units));
    fwrite(units, sizeof(uint64_t), COUNT(units), out);

    double scaling = unit / precision;
    Cell** cell = cell_array.items;
    for (uint64_t i = 0; i < cell_array.size; i++, cell++) {
        (*cell)->to_gds(out, scaling, max_points, precision, timestamp);
    }

    RawCell** rawcell = rawcell_array.items;
    for (uint64_t i = 0; i < rawcell_array.size; i++, rawcell++) (*rawcell)->to_gds(out);

    uint16_t buffer_end[] = {4, 0x0400};
    big_endian_swap16(buffer_end, COUNT(buffer_end));
    fwrite(buffer_end, sizeof(uint16_t), COUNT(buffer_end), out);

    fclose(out);
}

void Library::write_oas(const char* filename, double tolerance, uint8_t deflate_level,
                        uint16_t config_flags) const {
    OasisState state = {0};
    state.config_flags = config_flags;

    OasisStream out;
    out.data_size = 1024 * 1024;
    out.cursor = NULL;
    out.data = (uint8_t*)allocate(out.data_size);
    out.file = fopen(filename, "wb");
    if (out.file == NULL) {
        fputs("[GDSTK] Unable to open OASIS file for output.\n", stderr);
        return;
    }
    char header[] = {'%', 'S', 'E', 'M', 'I',  '-',  'O',
                     'A', 'S', 'I', 'S', '\r', '\n', (char)OasisRecord::START,
                     3,   '1', '.', '0'};
    fwrite(header, 1, COUNT(header), out.file);

    state.scaling = unit / precision;
    oasis_write_real(out, 1e-6 / precision);
    fputc(1, out.file);  // flag indicating that table-offsets will be stored in the END record

    Map<uint64_t> cell_name_map = {0};
    Map<Property*> cell_property_map = {0};

    Map<uint64_t> text_string_map = {0};

    properties_to_oas(properties, out, state);

    // Build cell name map. Other maps are built as the file is written.
    uint64_t c_size = cell_array.size;
    cell_name_map.resize((uint64_t)(2.0 + 10.0 / MAP_CAPACITY_THRESHOLD * c_size));
    cell_property_map.resize((uint64_t)(2.0 + 10.0 / MAP_CAPACITY_THRESHOLD * c_size));
    Cell** cell_p = cell_array.items;
    for (uint64_t i = 0; i < c_size; i++) {
        Cell* cell = (*cell_p++);
        cell_name_map.set(cell->name, i);
        cell_property_map.set(cell->name, cell->properties);
    }

    bool use_cblock = config_flags & OASIS_CONFIG_USE_CBLOCK;
    cell_p = cell_array.items;
    for (uint64_t i = 0; i < c_size; i++) {
        Cell* cell = *cell_p++;
        fputc((int)OasisRecord::CELL_REF_NUM, out.file);
        oasis_write_unsigned_integer(out, cell_name_map.get(cell->name));

        if (use_cblock) {
            out.cursor = out.data;
        }

        // TODO: Use modal variables
        // Cell contents
        Polygon** poly_p = cell->polygon_array.items;
        for (uint64_t j = cell->polygon_array.size; j > 0; j--) {
            (*poly_p++)->to_oas(out, state);
        }

        FlexPath** flexpath_p = cell->flexpath_array.items;
        for (uint64_t j = cell->flexpath_array.size; j > 0; j--) {
            FlexPath* path = *flexpath_p++;
            if (path->gdsii_path) {
                path->to_oas(out, state);
            } else {
                Array<Polygon*> array = {0};
                path->to_polygons(array);
                poly_p = array.items;
                for (uint64_t k = array.size; k > 0; k--) {
                    Polygon* poly = *poly_p++;
                    poly->to_oas(out, state);
                    poly->clear();
                    free_allocation(poly);
                }
                array.clear();
            }
        }

        RobustPath** robustpath_p = cell->robustpath_array.items;
        for (uint64_t j = cell->robustpath_array.size; j > 0; j--) {
            RobustPath* path = *robustpath_p++;
            if (path->gdsii_path) {
                path->to_oas(out, state);
            } else {
                Array<Polygon*> array = {0};
                path->to_polygons(array);
                poly_p = array.items;
                for (uint64_t k = array.size; k > 0; k--) {
                    Polygon* poly = *poly_p++;
                    poly->to_oas(out, state);
                    poly->clear();
                    free_allocation(poly);
                }
                array.clear();
            }
        }

        Reference** ref_p = cell->reference_array.items;
        for (uint64_t j = cell->reference_array.size; j > 0; j--) {
            Reference* ref = *ref_p++;
            if (ref->type == ReferenceType::RawCell) {
                fputs("[GDSTK] Reference to a RawCell cannot be used in a OASIS file.\n", stderr);
                continue;
            }
            uint8_t info = 0xF0;
            bool has_repetition = ref->repetition.get_size() > 1;
            if (has_repetition) info |= 0x08;
            if (ref->x_reflection) info |= 0x01;
            int64_t m;
            if (ref->magnification == 1.0 && is_multiple_of_pi_over_2(ref->rotation, m)) {
                if (m < 0) {
                    info |= ((uint8_t)(0x03 & ((m % 4) + 4))) << 1;
                } else {
                    info |= ((uint8_t)(0x03 & (m % 4))) << 1;
                }
                oasis_putc((int)OasisRecord::PLACEMENT, out);
                oasis_putc(info, out);
                uint64_t index = cell_name_map.get(
                    (ref->type == ReferenceType::Cell) ? ref->cell->name : ref->name);
                oasis_write_unsigned_integer(out, index);
            } else {
                if (ref->magnification != 1) info |= 0x04;
                if (ref->rotation != 0) info |= 0x02;
                oasis_putc((int)OasisRecord::PLACEMENT_TRANSFORM, out);
                oasis_putc(info, out);
                uint64_t index = cell_name_map.get(
                    (ref->type == ReferenceType::Cell) ? ref->cell->name : ref->name);
                oasis_write_unsigned_integer(out, index);
                if (ref->magnification != 1) {
                    oasis_write_real(out, ref->magnification);
                }
                if (ref->rotation != 0) {
                    oasis_write_real(out, ref->rotation * (180.0 / M_PI));
                }
            }
            oasis_write_integer(out, (int64_t)llround(ref->origin.x * state.scaling));
            oasis_write_integer(out, (int64_t)llround(ref->origin.y * state.scaling));
            if (has_repetition) oasis_write_repetition(out, ref->repetition, state.scaling);
            properties_to_oas(ref->properties, out, state);
        }

        Label** label_p = cell->label_array.items;
        for (uint64_t j = cell->label_array.size; j > 0; j--) {
            Label* label = *label_p++;
            uint8_t info = 0x7B;
            bool has_repetition = label->repetition.get_size() > 1;
            if (has_repetition) info |= 0x04;
            oasis_putc((int)OasisRecord::TEXT, out);
            oasis_putc(info, out);
            uint64_t index;
            if (text_string_map.has_key(label->text)) {
                index = text_string_map.get(label->text);
            } else {
                index = text_string_map.size;
                text_string_map.set(label->text, index);
            }
            oasis_write_unsigned_integer(out, index);
            oasis_write_unsigned_integer(out, label->layer);
            oasis_write_unsigned_integer(out, label->texttype);
            oasis_write_integer(out, (int64_t)llround(label->origin.x * state.scaling));
            oasis_write_integer(out, (int64_t)llround(label->origin.y * state.scaling));
            if (has_repetition) oasis_write_repetition(out, label->repetition, state.scaling);
            properties_to_oas(label->properties, out, state);
        }

        if (use_cblock) {
            uint64_t uncompressed_size = out.cursor - out.data;
            out.cursor = NULL;

            z_stream s = {0};
            // TODO: use custom allocator
            if (deflateInit2(&s, deflate_level, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
                fputs("[GDSTK] Unable to initialize zlib.\n", stderr);
            }
            s.avail_out = deflateBound(&s, uncompressed_size);
            uint8_t* buffer = (uint8_t*)allocate(s.avail_out);
            s.next_out = buffer;
            s.avail_in = uncompressed_size;
            s.next_in = out.data;
            int ret = deflate(&s, Z_FINISH);
            if (ret != Z_STREAM_END) {
                fputs("[GDSTK] Unable to compress CBLOCK.\n", stderr);
            }

            fputc((int)OasisRecord::CBLOCK, out.file);
            fputc(0, out.file);
            oasis_write_unsigned_integer(out, uncompressed_size);
            oasis_write_unsigned_integer(out, s.total_out);
            fwrite(buffer, 1, s.total_out, out.file);
            free_allocation(buffer);
            deflateEnd(&s);
        }
    }

    uint64_t cell_name_offset = c_size > 0 ? ftell(out.file) : 0;
    cell_p = cell_array.items;
    for (uint64_t i = 0; i < c_size; i++) {
        fputc((int)OasisRecord::CELLNAME_IMPLICIT, out.file);
        char* name_ = (*cell_p++)->name;
        uint64_t len = strlen(name_);
        oasis_write_unsigned_integer(out, len);
        fwrite(name_, 1, len, out.file);
        properties_to_oas(cell_property_map.get(name_), out, state);
    }

    uint64_t text_string_offset = text_string_map.size > 0 ? ftell(out.file) : 0;
    for (MapItem<uint64_t>* item = text_string_map.next(NULL); item;
         item = text_string_map.next(item)) {
        fputc((int)OasisRecord::TEXTSTRING, out.file);
        uint64_t len = strlen(item->key);
        oasis_write_unsigned_integer(out, len);
        fwrite(item->key, 1, len, out.file);
        oasis_write_unsigned_integer(out, item->value);
    }

    uint64_t prop_name_offset = state.property_name_map.size > 0 ? ftell(out.file) : 0;
    for (MapItem<uint64_t>* item = state.property_name_map.next(NULL); item;
         item = state.property_name_map.next(item)) {
        fputc((int)OasisRecord::PROPNAME, out.file);
        uint64_t len = strlen(item->key);
        oasis_write_unsigned_integer(out, len);
        fwrite(item->key, 1, len, out.file);
        oasis_write_unsigned_integer(out, item->value);
    }

    uint64_t prop_string_offset = state.property_value_array.size > 0 ? ftell(out.file) : 0;
    PropertyValue** value_p = state.property_value_array.items;
    for (uint64_t i = state.property_value_array.size; i > 0; i--) {
        PropertyValue* value = *value_p++;
        fputc((int)OasisRecord::PROPSTRING_IMPLICIT, out.file);
        oasis_write_unsigned_integer(out, value->size);
        fwrite(value->bytes, 1, value->size, out.file);
    }

    fputc((int)OasisRecord::END, out.file);

    // END header (1) + table-offsets (?) + b-string length (2) + padding + validation (1 or 5) =
    // 256
    uint64_t pad_len = 252 + ftell(out.file);

    // Table offsets
    fputc(1, out.file);
    oasis_write_unsigned_integer(out, cell_name_offset);
    fputc(1, out.file);
    oasis_write_unsigned_integer(out, text_string_offset);
    fputc(1, out.file);
    oasis_write_unsigned_integer(out, prop_name_offset);
    fputc(1, out.file);
    oasis_write_unsigned_integer(out, prop_string_offset);
    fputc(1, out.file);
    fputc(0, out.file);  // LAYERNAME table
    fputc(1, out.file);
    fputc(0, out.file);  // XNAME table

    pad_len -= ftell(out.file);
    oasis_write_unsigned_integer(out, pad_len);
    for (; pad_len > 0; pad_len--) fputc(0, out.file);

    // TODO: Add support for validation schemes (fix pad_len)
    fputc(0, out.file);

    free_allocation(out.data);
    fclose(out.file);

    cell_name_map.clear();
    cell_property_map.clear();
    text_string_map.clear();
    state.property_name_map.clear();
    state.property_value_array.clear();
}

Library read_gds(const char* filename, double unit, double tolerance) {
    const char* gdsii_record_names[] = {
        "HEADER",    "BGNLIB",   "LIBNAME",   "UNITS",      "ENDLIB",      "BGNSTR",
        "STRNAME",   "ENDSTR",   "BOUNDARY",  "PATH",       "SREF",        "AREF",
        "TEXT",      "LAYER",    "DATATYPE",  "WIDTH",      "XY",          "ENDEL",
        "SNAME",     "COLROW",   "TEXTNODE",  "NODE",       "TEXTTYPE",    "PRESENTATION",
        "SPACING",   "STRING",   "STRANS",    "MAG",        "ANGLE",       "UINTEGER",
        "USTRING",   "REFLIBS",  "FONTS",     "PATHTYPE",   "GENERATIONS", "ATTRTABLE",
        "STYPTABLE", "STRTYPE",  "ELFLAGS",   "ELKEY",      "LINKTYPE",    "LINKKEYS",
        "NODETYPE",  "PROPATTR", "PROPVALUE", "BOX",        "BOXTYPE",     "PLEX",
        "BGNEXTN",   "ENDEXTN",  "TAPENUM",   "TAPECODE",   "STRCLASS",    "RESERVED",
        "FORMAT",    "MASK",     "ENDMASKS",  "LIBDIRSIZE", "SRFNAME",     "LIBSECUR"};

    Library library = {0};
    // One extra char in case we need a 0-terminated string with max size (should never happen, but
    // it doesn't hurt to be prepared).
    uint8_t buffer[65537];
    int16_t* data16 = (int16_t*)(buffer + 4);
    int32_t* data32 = (int32_t*)(buffer + 4);
    uint64_t* data64 = (uint64_t*)(buffer + 4);
    char* str = (char*)(buffer + 4);
    uint32_t record_length;

    Cell* cell = NULL;
    Polygon* polygon = NULL;
    FlexPath* path = NULL;
    Reference* reference = NULL;
    Label* label = NULL;

    double factor = 1;
    double width = 0;
    int16_t key = 0;

    FILE* in = fopen(filename, "rb");
    if (in == NULL) {
        fputs("[GDSTK] Unable to open GDSII file for input.\n", stderr);
        return library;
    }

    while ((record_length = gdsii_read_record(in, buffer)) > 0) {
        uint32_t data_length;

        // printf("%02X %s (%" PRIu32 " bytes)", buffer[2], gdsii_record_names[buffer[2]],
        // record_length);

        switch ((GdsiiDataType)buffer[3]) {
            case GdsiiDataType::BitArray:
            case GdsiiDataType::TwoByteSignedInteger:
                data_length = (record_length - 4) / 2;
                big_endian_swap16((uint16_t*)data16, data_length);
                // for (uint32_t i = 0; i < data_length; i++) printf(" %" PRId16, data16[i]);
                break;
            case GdsiiDataType::FourByteSignedInteger:
            case GdsiiDataType::FourByteReal:
                data_length = (record_length - 4) / 4;
                big_endian_swap32((uint32_t*)data32, data_length);
                // for (uint32_t i = 0; i < data_length; i++) printf(" %" PRId32, data32[i]);
                break;
            case GdsiiDataType::EightByteReal:
                data_length = (record_length - 4) / 8;
                big_endian_swap64(data64, data_length);
                // for (uint32_t i = 0; i < data_length; i++) printf(" %" PRIu64, data64[i]);
                break;
            default:
                data_length = record_length - 4;
                // for (uint32_t i = 0; i < data_length; i++) printf(" %c", str[i]);
        }

        // putchar('\n');

        switch ((GdsiiRecord)(buffer[2])) {
            case GdsiiRecord::HEADER:
            case GdsiiRecord::BGNLIB:
            case GdsiiRecord::ENDSTR:
                break;
            case GdsiiRecord::LIBNAME:
                if (str[data_length - 1] == 0) data_length--;
                library.name = (char*)allocate(data_length + 1);
                memcpy(library.name, str, data_length);
                library.name[data_length] = 0;
                break;
            case GdsiiRecord::UNITS: {
                const double db_in_user = gdsii_real_to_double(data64[0]);
                const double db_in_meters = gdsii_real_to_double(data64[1]);
                if (unit > 0) {
                    factor = db_in_meters / unit;
                    library.unit = unit;
                } else {
                    factor = db_in_user;
                    library.unit = db_in_meters / db_in_user;
                }
                library.precision = db_in_meters;
            } break;
            case GdsiiRecord::ENDLIB: {
                Map<Cell*> map = {0};
                uint64_t c_size = library.cell_array.size;
                map.resize((uint64_t)(2.0 + 10.0 / MAP_CAPACITY_THRESHOLD * c_size));
                Cell** c_item = library.cell_array.items;
                for (uint64_t i = c_size; i > 0; i--, c_item++) map.set((*c_item)->name, *c_item);
                c_item = library.cell_array.items;
                for (uint64_t i = c_size; i > 0; i--) {
                    cell = *c_item++;
                    Reference** ref = cell->reference_array.items;
                    for (uint64_t j = cell->reference_array.size; j > 0; j--) {
                        reference = *ref++;
                        Cell* cp = map.get(reference->name);
                        if (cp) {
                            free_allocation(reference->name);
                            reference->type = ReferenceType::Cell;
                            reference->cell = cp;
                        }
                    }
                }
                map.clear();
                fclose(in);
                return library;
            } break;
            case GdsiiRecord::BGNSTR:
                cell = (Cell*)allocate_clear(sizeof(Cell));
                break;
            case GdsiiRecord::STRNAME:
                if (cell) {
                    if (str[data_length - 1] == 0) data_length--;
                    cell->name = (char*)allocate(data_length + 1);
                    memcpy(cell->name, str, data_length);
                    cell->name[data_length] = 0;
                    library.cell_array.append(cell);
                }
                break;
            case GdsiiRecord::BOUNDARY:
            case GdsiiRecord::BOX:
                polygon = (Polygon*)allocate_clear(sizeof(Polygon));
                if (cell) cell->polygon_array.append(polygon);
                break;
            case GdsiiRecord::PATH:
                path = (FlexPath*)allocate_clear(sizeof(FlexPath));
                path->num_elements = 1;
                path->elements = (FlexPathElement*)allocate_clear(sizeof(FlexPathElement));
                path->gdsii_path = true;
                if (cell) cell->flexpath_array.append(path);
                break;
            case GdsiiRecord::SREF:
            case GdsiiRecord::AREF:
                reference = (Reference*)allocate_clear(sizeof(Reference));
                reference->magnification = 1;
                if (cell) cell->reference_array.append(reference);
                break;
            case GdsiiRecord::TEXT:
                label = (Label*)allocate_clear(sizeof(Label));
                if (cell) cell->label_array.append(label);
                break;
            case GdsiiRecord::LAYER:
                if (polygon)
                    polygon->layer = data16[0];
                else if (path)
                    path->elements[0].layer = data16[0];
                else if (label)
                    label->layer = data16[0];
                break;
            case GdsiiRecord::DATATYPE:
            case GdsiiRecord::BOXTYPE:
                if (polygon)
                    polygon->datatype = data16[0];
                else if (path)
                    path->elements[0].datatype = data16[0];
                break;
            case GdsiiRecord::WIDTH:
                if (data32[0] < 0) {
                    width = factor * -data32[0];
                    if (path) path->scale_width = false;
                } else {
                    width = factor * data32[0];
                    if (path) path->scale_width = true;
                }
                break;
            case GdsiiRecord::XY:
                if (polygon) {
                    polygon->point_array.ensure_slots(data_length / 2);
                    double* d = (double*)polygon->point_array.items + polygon->point_array.size;
                    int32_t* s = data32;
                    for (uint32_t i = data_length; i > 0; i--) *d++ = factor * (*s++);
                    polygon->point_array.size += data_length / 2;
                } else if (path) {
                    Array<Vec2> point_array = {0};
                    if (path->spine.point_array.size == 0) {
                        path->spine.tolerance = tolerance;
                        path->spine.append(Vec2{factor * data32[0], factor * data32[1]});
                        path->elements[0].half_width_and_offset.append(Vec2{width / 2, 0});
                        point_array.ensure_slots(data_length / 2 - 1);
                        double* d = (double*)point_array.items;
                        int32_t* s = data32 + 2;
                        for (uint32_t i = data_length - 2; i > 0; i--) *d++ = factor * (*s++);
                        point_array.size += data_length / 2 - 1;
                    } else {
                        point_array.ensure_slots(data_length / 2);
                        double* d = (double*)point_array.items;
                        int32_t* s = data32;
                        for (uint32_t i = data_length; i > 0; i--) *d++ = factor * (*s++);
                        point_array.size += data_length / 2;
                    }
                    path->segment(point_array, NULL, NULL, false);
                    point_array.clear();
                } else if (reference) {
                    Vec2 origin = Vec2{factor * data32[0], factor * data32[1]};
                    reference->origin = origin;
                    if (reference->repetition.type != RepetitionType::None) {
                        Repetition* repetition = &reference->repetition;
                        if (reference->rotation == 0 && !reference->x_reflection) {
                            repetition->spacing.x =
                                (factor * data32[2] - origin.x) / repetition->columns;
                            repetition->spacing.y =
                                (factor * data32[5] - origin.y) / repetition->rows;
                        } else {
                            repetition->type = RepetitionType::Regular;
                            repetition->v1.x =
                                (factor * data32[2] - origin.x) / repetition->columns;
                            repetition->v1.y =
                                (factor * data32[3] - origin.y) / repetition->columns;
                            repetition->v2.x = (factor * data32[4] - origin.x) / repetition->rows;
                            repetition->v2.y = (factor * data32[5] - origin.y) / repetition->rows;
                        }
                    }
                } else if (label) {
                    label->origin.x = factor * data32[0];
                    label->origin.y = factor * data32[1];
                }
                break;
            case GdsiiRecord::ENDEL:
                if (polygon) {
                    // Polygons are closed in GDSII (first and last points are the same)
                    polygon->point_array.size--;
                    polygon = NULL;
                }
                path = NULL;
                reference = NULL;
                label = NULL;
                break;
            case GdsiiRecord::SNAME: {
                if (reference) {
                    if (str[data_length - 1] == 0) data_length--;
                    reference->name = (char*)allocate(data_length + 1);
                    memcpy(reference->name, str, data_length);
                    reference->name[data_length] = 0;
                    reference->type = ReferenceType::Name;
                }
            } break;
            case GdsiiRecord::COLROW:
                if (reference) {
                    Repetition* repetition = &reference->repetition;
                    repetition->type = RepetitionType::Rectangular;
                    repetition->columns = data16[0];
                    repetition->rows = data16[1];
                }
                break;
            case GdsiiRecord::TEXTTYPE:
                if (label) label->texttype = data16[0];
                break;
            case GdsiiRecord::PRESENTATION:
                if (label) label->anchor = (Anchor)(data16[0] & 0x000F);
                break;
            case GdsiiRecord::STRING:
                if (label) {
                    if (str[data_length - 1] == 0) data_length--;
                    label->text = (char*)allocate(data_length + 1);
                    memcpy(label->text, str, data_length);
                    label->text[data_length] = 0;
                }
                break;
            case GdsiiRecord::STRANS:
                if (reference)
                    reference->x_reflection = (data16[0] & 0x8000) != 0;
                else if (label)
                    label->x_reflection = (data16[0] & 0x8000) != 0;
                if (data16[0] & 0x0006)
                    fputs(
                        "[GDSTK] Absolute magnification and rotation of references is not supported.\n",
                        stderr);
                break;
            case GdsiiRecord::MAG:
                if (reference)
                    reference->magnification = gdsii_real_to_double(data64[0]);
                else if (label)
                    label->magnification = gdsii_real_to_double(data64[0]);
                break;
            case GdsiiRecord::ANGLE:
                if (reference)
                    reference->rotation = M_PI / 180.0 * gdsii_real_to_double(data64[0]);
                else if (label)
                    label->rotation = M_PI / 180.0 * gdsii_real_to_double(data64[0]);
                break;
            case GdsiiRecord::PATHTYPE:
                if (path) {
                    switch (data16[0]) {
                        case 0:
                            path->elements[0].end_type = EndType::Flush;
                            break;
                        case 1:
                            path->elements[0].end_type = EndType::Round;
                            break;
                        case 2:
                            path->elements[0].end_type = EndType::HalfWidth;
                            break;
                        default:
                            path->elements[0].end_type = EndType::Extended;
                    }
                }
                break;
            case GdsiiRecord::PROPATTR:
                key = data16[0];
                break;
            case GdsiiRecord::PROPVALUE:
                if (str[data_length - 1] != 0) str[data_length++] = 0;
                if (polygon) {
                    set_gds_property(polygon->properties, key, str);
                } else if (path) {
                    set_gds_property(path->properties, key, str);
                } else if (reference) {
                    set_gds_property(reference->properties, key, str);
                } else if (label) {
                    set_gds_property(label->properties, key, str);
                }
                break;
            case GdsiiRecord::BGNEXTN:
                if (path) path->elements[0].end_extensions.u = factor * data32[0];
                break;
            case GdsiiRecord::ENDEXTN:
                if (path) path->elements[0].end_extensions.v = factor * data32[0];
                break;
            // case GdsiiRecord::TEXTNODE:
            // case GdsiiRecord::NODE:
            // case GdsiiRecord::SPACING:
            // case GdsiiRecord::UINTEGER:
            // case GdsiiRecord::USTRING:
            // case GdsiiRecord::REFLIBS:
            // case GdsiiRecord::FONTS:
            // case GdsiiRecord::GENERATIONS:
            // case GdsiiRecord::ATTRTABLE:
            // case GdsiiRecord::STYPTABLE:
            // case GdsiiRecord::STRTYPE:
            // case GdsiiRecord::ELFLAGS:
            // case GdsiiRecord::ELKEY:
            // case GdsiiRecord::LINKTYPE:
            // case GdsiiRecord::LINKKEYS:
            // case GdsiiRecord::NODETYPE:
            // case GdsiiRecord::PLEX:
            // case GdsiiRecord::TAPENUM:
            // case GdsiiRecord::TAPECODE:
            // case GdsiiRecord::STRCLASS:
            // case GdsiiRecord::RESERVED:
            // case GdsiiRecord::FORMAT:
            // case GdsiiRecord::MASK:
            // case GdsiiRecord::ENDMASKS:
            // case GdsiiRecord::LIBDIRSIZE:
            // case GdsiiRecord::SRFNAME:
            // case GdsiiRecord::LIBSECUR:
            default:
                if (buffer[2] < COUNT(gdsii_record_names))
                    fprintf(stderr, "[GDSTK] Record type %s (0x%02X) is not supported.\n",
                            gdsii_record_names[buffer[2]], buffer[2]);
                else
                    fprintf(stderr, "[GDSTK] Unknown record type 0x%02X.\n", buffer[2]);
        }
    }

    fclose(in);
    return Library{0};
}

Library read_oas(const char* filename, double unit, double tolerance) {
    Library library = {0};

    OasisStream in = {0};
    in.file = fopen(filename, "rb");
    if (in.file == NULL) {
        fputs("[GDSTK] Unable to open OASIS file for input.\n", stderr);
        fclose(in.file);
        return library;
    }

    // Check header bytes and START record
    char header[14];
    if (fread(header, 1, 14, in.file) < 14 || memcmp(header, "%SEMI-OASIS\r\n\x01", 14) != 0) {
        fputs("[GDSTK] Invalid OASIS header found.\n", stderr);
        fclose(in.file);
        return library;
    }

    // Process START record
    uint64_t len;
    uint8_t* version = oasis_read_string(in, false, len);
    if (memcmp(version, "1.0", 3) != 0) {
        fputs("[GDSTK] Unsupported OASIS file version.\n", stderr);
    }
    free_allocation(version);

    double factor = 1 / oasis_read_real(in);
    library.precision = 1e-6 * factor;
    if (unit > 0) {
        library.unit = unit;
        factor *= 1e-6 / unit;
    } else {
        library.unit = 1e-6;
    }

    uint64_t offset_table_flag = oasis_read_unsigned_integer(in);
    if (offset_table_flag == 0) {
        // Skip offset table
        for (uint8_t i = 12; i > 0; i--) oasis_read_unsigned_integer(in);
    }

    // State variables
    bool modal_absolute_pos = true;
    uint64_t modal_layer = 0;
    uint64_t modal_datatype = 0;
    uint64_t modal_textlayer = 0;
    uint64_t modal_texttype = 0;
    Vec2 modal_placement_pos = {0, 0};
    Vec2 modal_text_pos = {0, 0};
    Vec2 modal_geom_pos = {0, 0};
    Vec2 modal_geom_dim = {0, 0};
    Repetition modal_repetition = {RepetitionType::None};
    Label* modal_text_string = NULL;
    Reference* modal_placement_cell = NULL;
    Array<Vec2> modal_polygon_points = {0};
    Array<Vec2> modal_path_points = {0};
    double modal_path_halfwidth = 0;
    Vec2 modal_path_extensions = {0, 0};
    uint8_t modal_ctrapezoid_type = 0;
    double modal_circle_radius = 0;
    Property* modal_property = NULL;
    PropertyValue* modal_property_value_list = NULL;

    Property** next_property = &library.properties;

    Array<Property*> unfinished_property_name = {0};
    Array<PropertyValue*> unfinished_property_value = {0};
    bool modal_property_unfinished = false;

    // Name tables
    Array<ByteArray> cell_name_table = {0};
    Array<ByteArray> label_text_table = {0};
    Array<ByteArray> property_name_table = {0};
    Array<ByteArray> property_value_table = {0};

    // Elements
    Cell* cell = NULL;

    OasisRecord record;
    while (oasis_read(&record, 1, 1, in) > 0) {
        switch (record) {
            case OasisRecord::PAD:
                break;
            case OasisRecord::START:
                // START is parsed before this loop
                fputs("[GDSTK] Unexpected START record out of position in file.\n", stderr);
                break;
            case OasisRecord::END: {
                fseek(in.file, 0, SEEK_END);
                library.name = (char*)allocate(4);
                library.name[0] = 'L';
                library.name[1] = 'I';
                library.name[2] = 'B';
                library.name[3] = 0;

                uint64_t c_size = library.cell_array.size;
                Map<Cell*> map = {0};
                map.resize((uint64_t)(2.0 + 10.0 / MAP_CAPACITY_THRESHOLD * c_size));

                Cell** cell_p = library.cell_array.items;
                for (uint64_t i = c_size; i > 0; i--) {
                    cell = *cell_p++;
                    if (cell->name == NULL) {
                        ByteArray* cell_name = cell_name_table.items + (uint64_t)cell->owner;
                        cell->owner = NULL;
                        cell->name = copy_string((char*)cell_name->bytes, len);
                        if (cell_name->properties) {
                            Property* last = cell_name->properties;
                            while (last->next) last = last->next;
                            last->next = cell->properties;
                            cell->properties = cell_name->properties;
                            cell_name->properties = NULL;
                        }
                    }
                    map.set(cell->name, cell);

                    Label** label_p = cell->label_array.items;
                    for (uint64_t j = cell->label_array.size; j > 0; j--) {
                        Label* label = *label_p++;
                        if (label->text == NULL) {
                            ByteArray* label_text = label_text_table.items + (uint64_t)label->owner;
                            label->owner = NULL;
                            label->text = copy_string((char*)label_text->bytes, len);
                            if (label_text->properties) {
                                Property* copy = properties_copy(label_text->properties);
                                Property* last = copy;
                                while (last->next) last = last->next;
                                last->next = label->properties;
                                label->properties = copy;
                            }
                        }
                    }
                }

                cell_p = library.cell_array.items;
                for (uint64_t i = c_size; i > 0; i--, cell_p++) {
                    Reference** ref_p = (*cell_p)->reference_array.items;
                    for (uint64_t j = (*cell_p)->reference_array.size; j > 0; j--, ref_p++) {
                        Reference* ref = *ref_p;
                        if (ref->type == ReferenceType::Cell) {
                            // Using reference number
                            ByteArray* cell_name = cell_name_table.items + (uint64_t)ref->cell;
                            ref->cell = map.get((char*)cell_name->bytes);
                        } else {
                            // Using name
                            cell = map.get(ref->name);
                            free_allocation(ref->name);
                            ref->cell = cell;
                            ref->type = ReferenceType::Cell;
                        }
                    }
                }
                map.clear();

                Property** prop_p = unfinished_property_name.items;
                for (uint64_t i = unfinished_property_name.size; i > 0; i--) {
                    Property* property = *prop_p++;
                    ByteArray* prop_name = property_name_table.items + (uint64_t)property->name;
                    property->name = copy_string((char*)prop_name->bytes, len);
                }
                PropertyValue** prop_value_p = unfinished_property_value.items;
                for (uint64_t i = unfinished_property_value.size; i > 0; i--) {
                    PropertyValue* property_value = *prop_value_p++;
                    ByteArray* prop_string =
                        property_value_table.items + (uint64_t)property_value->unsigned_integer;
                    property_value->type = PropertyType::String;
                    property_value->size = prop_string->size;
                    property_value->bytes = (uint8_t*)allocate(prop_string->size);
                    memcpy(property_value->bytes, prop_string->bytes, prop_string->size);
                }
            } break;
            case OasisRecord::CELLNAME_IMPLICIT: {
                uint8_t* bytes = oasis_read_string(in, true, len);
                cell_name_table.append(ByteArray{len, bytes, NULL});
                next_property = &cell_name_table[cell_name_table.size - 1].properties;
            } break;
            case OasisRecord::CELLNAME: {
                uint8_t* bytes = oasis_read_string(in, true, len);
                uint64_t ref_number = oasis_read_unsigned_integer(in);
                if (ref_number >= cell_name_table.size) {
                    cell_name_table.ensure_slots(ref_number + 1 - cell_name_table.size);
                    for (uint64_t i = cell_name_table.size; i < ref_number; i++) {
                        cell_name_table[i] = ByteArray{0, NULL, NULL};
                    }
                    cell_name_table.size = ref_number + 1;
                }
                cell_name_table[ref_number] = ByteArray{len, bytes, NULL};
                next_property = &cell_name_table[ref_number].properties;
            } break;
            case OasisRecord::TEXTSTRING_IMPLICIT: {
                uint8_t* bytes = oasis_read_string(in, true, len);
                label_text_table.append(ByteArray{len, bytes, NULL});
                next_property = &label_text_table[label_text_table.size - 1].properties;
            } break;
            case OasisRecord::TEXTSTRING: {
                uint8_t* bytes = oasis_read_string(in, true, len);
                uint64_t ref_number = oasis_read_unsigned_integer(in);
                if (ref_number >= label_text_table.size) {
                    label_text_table.ensure_slots(ref_number + 1 - label_text_table.size);
                    for (uint64_t i = label_text_table.size; i < ref_number; i++) {
                        label_text_table[i] = ByteArray{0, NULL, NULL};
                    }
                    label_text_table.size = ref_number + 1;
                }
                label_text_table[ref_number] = ByteArray{len, bytes, NULL};
                next_property = &label_text_table[ref_number].properties;
            } break;
            case OasisRecord::PROPNAME_IMPLICIT: {
                uint8_t* bytes = oasis_read_string(in, true, len);
                property_name_table.append(ByteArray{len, bytes, NULL});
                next_property = &property_name_table[property_name_table.size - 1].properties;
            } break;
            case OasisRecord::PROPNAME: {
                uint8_t* bytes = oasis_read_string(in, true, len);
                uint64_t ref_number = oasis_read_unsigned_integer(in);
                if (ref_number >= property_name_table.size) {
                    property_name_table.ensure_slots(ref_number + 1 - property_name_table.size);
                    for (uint64_t i = property_name_table.size; i < ref_number; i++) {
                        property_name_table[i] = ByteArray{0, NULL, NULL};
                    }
                    property_name_table.size = ref_number + 1;
                }
                property_name_table[ref_number] = ByteArray{len, bytes, NULL};
                next_property = &property_name_table[ref_number].properties;
            } break;
            case OasisRecord::PROPSTRING_IMPLICIT: {
                uint8_t* bytes = oasis_read_string(in, false, len);
                property_value_table.append(ByteArray{len, bytes, NULL});
                next_property = &property_value_table[property_value_table.size - 1].properties;
            } break;
            case OasisRecord::PROPSTRING: {
                uint8_t* bytes = oasis_read_string(in, false, len);
                uint64_t ref_number = oasis_read_unsigned_integer(in);
                if (ref_number >= property_value_table.size) {
                    property_value_table.ensure_slots(ref_number + 1 - property_value_table.size);
                    for (uint64_t i = property_value_table.size; i < ref_number; i++) {
                        property_value_table[i] = ByteArray{0, NULL, NULL};
                    }
                    property_value_table.size = ref_number + 1;
                }
                property_value_table[ref_number] = ByteArray{len, bytes, NULL};
                next_property = &property_value_table[ref_number].properties;
            } break;
            case OasisRecord::LAYERNAME_DATA:
            case OasisRecord::LAYERNAME_TEXT:
                // Unused record
                free_allocation(oasis_read_string(in, false, len));
                for (uint32_t i = 2; i > 0; i--) {
                    uint64_t type = oasis_read_unsigned_integer(in);
                    if (type > 0) {
                        if (type == 4) oasis_read_unsigned_integer(in);
                        oasis_read_unsigned_integer(in);
                    }
                }
                break;
            case OasisRecord::CELL_REF_NUM:
            case OasisRecord::CELL: {
                cell = (Cell*)allocate_clear(sizeof(Cell));
                library.cell_array.append(cell);
                next_property = &cell->properties;
                if (record == OasisRecord::CELL_REF_NUM) {
                    // Use owner as temporary storage for the reference number
                    cell->owner = (void*)oasis_read_unsigned_integer(in);
                } else {
                    cell->name = (char*)oasis_read_string(in, true, len);
                }
                modal_absolute_pos = true;
                modal_placement_pos = {0, 0};
                modal_geom_pos = {0, 0};
                modal_text_pos = {0, 0};
            } break;
            case OasisRecord::XYABSOLUTE:
                modal_absolute_pos = true;
                break;
            case OasisRecord::XYRELATIVE:
                modal_absolute_pos = false;
                break;
            case OasisRecord::PLACEMENT:
            case OasisRecord::PLACEMENT_TRANSFORM: {
                Reference* reference = (Reference*)allocate_clear(sizeof(Reference));
                cell->reference_array.append(reference);
                next_property = &reference->properties;
                uint8_t info;
                oasis_read(&info, 1, 1, in);
                if (info & 0x80) {
                    // Explicit reference
                    if (info & 0x40) {
                        // Reference number
                        reference->type = ReferenceType::Cell;
                        reference->cell = (Cell*)oasis_read_unsigned_integer(in);
                    } else {
                        // Cell name
                        reference->type = ReferenceType::Name;
                        reference->name = (char*)oasis_read_string(in, true, len);
                    }
                    modal_placement_cell = reference;
                } else {
                    // Use modal_placement_cell
                    if (modal_placement_cell->type == ReferenceType::Cell) {
                        reference->type = ReferenceType::Cell;
                        reference->cell = modal_placement_cell->cell;
                    } else {
                        reference->type = ReferenceType::Name;
                        reference->name = copy_string(modal_placement_cell->name, len);
                    }
                }
                if (record == OasisRecord::PLACEMENT) {
                    reference->magnification = 1;
                    switch (info & 0x06) {
                        case 0x02:
                            reference->rotation = M_PI * 0.5;
                            break;
                        case 0x04:
                            reference->rotation = M_PI;
                            break;
                        case 0x06:
                            reference->rotation = M_PI * 1.5;
                    }
                } else {
                    if (info & 0x04) {
                        reference->magnification = oasis_read_real(in);
                    } else {
                        reference->magnification = 1;
                    }
                    if (info & 0x02) {
                        reference->rotation = oasis_read_real(in) * (M_PI / 180.0);
                    }
                }
                reference->x_reflection = info & 0x01;
                if (info & 0x20) {
                    double x = factor * oasis_read_integer(in);
                    if (modal_absolute_pos) {
                        modal_placement_pos.x = x;
                    } else {
                        modal_placement_pos.x += x;
                    }
                }
                if (info & 0x10) {
                    double y = factor * oasis_read_integer(in);
                    if (modal_absolute_pos) {
                        modal_placement_pos.y = y;
                    } else {
                        modal_placement_pos.y += y;
                    }
                }
                reference->origin = modal_placement_pos;
                if (info & 0x08) {
                    oasis_read_repetition(in, factor, modal_repetition);
                    reference->repetition.copy_from(modal_repetition);
                }
            } break;
            case OasisRecord::TEXT: {
                Label* label = (Label*)allocate_clear(sizeof(Label));
                label->magnification = 1;
                label->anchor = Anchor::SW;
                cell->label_array.append(label);
                next_property = &label->properties;
                uint8_t info;
                oasis_read(&info, 1, 1, in);
                if (info & 0x40) {
                    // Explicit text
                    if (info & 0x20) {
                        // Reference number: use owner to temporarily store it
                        label->owner = (void*)oasis_read_unsigned_integer(in);
                    } else {
                        label->text = (char*)oasis_read_string(in, true, len);
                    }
                    modal_text_string = label;
                } else {
                    // Use modal_text_string
                    if (modal_text_string->text == NULL) {
                        label->owner = modal_text_string->owner;
                    } else {
                        label->text = copy_string(modal_text_string->text, len);
                    }
                }
                if (info & 0x01) {
                    modal_textlayer = oasis_read_unsigned_integer(in);
                }
                label->layer = modal_textlayer;
                if (info & 0x02) {
                    modal_texttype = oasis_read_unsigned_integer(in);
                }
                label->texttype = modal_texttype;
                if (info & 0x10) {
                    double x = factor * oasis_read_integer(in);
                    if (modal_absolute_pos) {
                        modal_text_pos.x = x;
                    } else {
                        modal_text_pos.x += x;
                    }
                }
                if (info & 0x08) {
                    double y = factor * oasis_read_integer(in);
                    if (modal_absolute_pos) {
                        modal_text_pos.y = y;
                    } else {
                        modal_text_pos.y += y;
                    }
                }
                label->origin = modal_text_pos;
                if (info & 0x04) {
                    oasis_read_repetition(in, factor, modal_repetition);
                    label->repetition.copy_from(modal_repetition);
                }
            } break;
            case OasisRecord::RECTANGLE: {
                Polygon* polygon = (Polygon*)allocate_clear(sizeof(Polygon));
                cell->polygon_array.append(polygon);
                next_property = &polygon->properties;
                uint8_t info;
                oasis_read(&info, 1, 1, in);
                if (info & 0x01) {
                    modal_layer = oasis_read_unsigned_integer(in);
                }
                if (info & 0x02) {
                    modal_datatype = oasis_read_unsigned_integer(in);
                }
                if (info & 0x40) {
                    modal_geom_dim.x = factor * oasis_read_unsigned_integer(in);
                }
                if (info & 0x20) {
                    modal_geom_dim.y = factor * oasis_read_unsigned_integer(in);
                }
                if (info & 0x10) {
                    double x = factor * oasis_read_integer(in);
                    if (modal_absolute_pos) {
                        modal_geom_pos.x = x;
                    } else {
                        modal_geom_pos.x += x;
                    }
                }
                if (info & 0x08) {
                    double y = factor * oasis_read_integer(in);
                    if (modal_absolute_pos) {
                        modal_geom_pos.y = y;
                    } else {
                        modal_geom_pos.y += y;
                    }
                }
                Vec2 corner2 = {
                    modal_geom_pos.x + modal_geom_dim.x,
                    modal_geom_pos.y + ((info & 0x80) ? modal_geom_dim.x : modal_geom_dim.y)};
                *polygon = rectangle(modal_geom_pos, corner2, modal_layer, modal_datatype);
                if (info & 0x04) {
                    oasis_read_repetition(in, factor, modal_repetition);
                    polygon->repetition.copy_from(modal_repetition);
                }
            } break;
            case OasisRecord::POLYGON: {
                Polygon* polygon = (Polygon*)allocate_clear(sizeof(Polygon));
                cell->polygon_array.append(polygon);
                next_property = &polygon->properties;
                uint8_t info;
                oasis_read(&info, 1, 1, in);
                if (info & 0x01) {
                    modal_layer = oasis_read_unsigned_integer(in);
                }
                polygon->layer = modal_layer;
                if (info & 0x02) {
                    modal_datatype = oasis_read_unsigned_integer(in);
                }
                polygon->datatype = modal_datatype;
                if (info & 0x20) {
                    modal_polygon_points.size = 0;
                    oasis_read_point_list(in, factor, true, modal_polygon_points);
                }
                polygon->point_array.ensure_slots(1 + modal_polygon_points.size);
                polygon->point_array.append_unsafe(Vec2{0, 0});
                polygon->point_array.extend(modal_polygon_points);
                if (info & 0x10) {
                    double x = factor * oasis_read_integer(in);
                    if (modal_absolute_pos) {
                        modal_geom_pos.x = x;
                    } else {
                        modal_geom_pos.x += x;
                    }
                }
                if (info & 0x08) {
                    double y = factor * oasis_read_integer(in);
                    if (modal_absolute_pos) {
                        modal_geom_pos.y = y;
                    } else {
                        modal_geom_pos.y += y;
                    }
                }
                Vec2* v = polygon->point_array.items;
                for (uint64_t i = polygon->point_array.size; i > 0; i--) {
                    *v++ += modal_geom_pos;
                }
                if (info & 0x04) {
                    oasis_read_repetition(in, factor, modal_repetition);
                    polygon->repetition.copy_from(modal_repetition);
                }
            } break;
            case OasisRecord::PATH: {
                FlexPath* path = (FlexPath*)allocate_clear(sizeof(FlexPath));
                FlexPathElement* element =
                    (FlexPathElement*)allocate_clear(sizeof(FlexPathElement));
                cell->flexpath_array.append(path);
                next_property = &path->properties;
                path->spine.tolerance = tolerance;
                path->elements = element;
                path->num_elements = 1;
                path->gdsii_path = true;
                path->scale_width = true;
                uint8_t info;
                oasis_read(&info, 1, 1, in);
                if (info & 0x01) {
                    modal_layer = oasis_read_unsigned_integer(in);
                }
                element->layer = modal_layer;
                if (info & 0x02) {
                    modal_datatype = oasis_read_unsigned_integer(in);
                }
                element->datatype = modal_datatype;
                if (info & 0x40) {
                    modal_path_halfwidth = factor * oasis_read_unsigned_integer(in);
                }
                element->half_width_and_offset.append(Vec2{modal_path_halfwidth, 0});
                if (info & 0x80) {
                    uint8_t extension_scheme;
                    oasis_read(&extension_scheme, 1, 1, in);
                    switch (extension_scheme & 0x03) {
                        case 0x01:
                            modal_path_extensions.x = 0;
                            break;
                        case 0x02:
                            modal_path_extensions.x = modal_path_halfwidth;
                            break;
                        case 0x03:
                            modal_path_extensions.x = factor * oasis_read_integer(in);
                    }
                    switch (extension_scheme & 0x0c) {
                        case 0x04:
                            modal_path_extensions.y = 0;
                            break;
                        case 0x08:
                            modal_path_extensions.y = modal_path_halfwidth;
                            break;
                        case 0x0c:
                            modal_path_extensions.y = factor * oasis_read_integer(in);
                    }
                }
                if (modal_path_extensions.x == 0 && modal_path_extensions.y == 0) {
                    element->end_type = EndType::Flush;
                } else if (modal_path_extensions.x == modal_path_halfwidth &&
                           modal_path_extensions.y == modal_path_halfwidth) {
                    element->end_type = EndType::HalfWidth;
                } else {
                    element->end_type = EndType::Extended;
                    element->end_extensions = modal_path_extensions;
                }
                if (info & 0x20) {
                    modal_path_points.size = 0;
                    oasis_read_point_list(in, factor, false, modal_path_points);
                }
                if (info & 0x10) {
                    double x = factor * oasis_read_integer(in);
                    if (modal_absolute_pos) {
                        modal_geom_pos.x = x;
                    } else {
                        modal_geom_pos.x += x;
                    }
                }
                if (info & 0x08) {
                    double y = factor * oasis_read_integer(in);
                    if (modal_absolute_pos) {
                        modal_geom_pos.y = y;
                    } else {
                        modal_geom_pos.y += y;
                    }
                }
                path->spine.append(modal_geom_pos);
                path->segment(modal_path_points, NULL, NULL, true);
                if (info & 0x04) {
                    oasis_read_repetition(in, factor, modal_repetition);
                    path->repetition.copy_from(modal_repetition);
                }
            } break;
            case OasisRecord::TRAPEZOID_AB:
            case OasisRecord::TRAPEZOID_A:
            case OasisRecord::TRAPEZOID_B: {
                Polygon* polygon = (Polygon*)allocate_clear(sizeof(Polygon));
                cell->polygon_array.append(polygon);
                next_property = &polygon->properties;
                uint8_t info;
                oasis_read(&info, 1, 1, in);
                if (info & 0x01) {
                    modal_layer = oasis_read_unsigned_integer(in);
                }
                polygon->layer = modal_layer;
                if (info & 0x02) {
                    modal_datatype = oasis_read_unsigned_integer(in);
                }
                polygon->datatype = modal_datatype;
                if (info & 0x40) {
                    modal_geom_dim.x = factor * oasis_read_unsigned_integer(in);
                }
                if (info & 0x20) {
                    modal_geom_dim.y = factor * oasis_read_unsigned_integer(in);
                }
                double delta_a, delta_b;
                if (record == OasisRecord::TRAPEZOID_AB) {
                    delta_a = factor * oasis_read_1delta(in);
                    delta_b = factor * oasis_read_1delta(in);
                } else if (record == OasisRecord::TRAPEZOID_A) {
                    delta_a = factor * oasis_read_1delta(in);
                    delta_b = 0;
                } else {
                    delta_a = 0;
                    delta_b = factor * oasis_read_1delta(in);
                }
                if (info & 0x10) {
                    double x = factor * oasis_read_integer(in);
                    if (modal_absolute_pos) {
                        modal_geom_pos.x = x;
                    } else {
                        modal_geom_pos.x += x;
                    }
                }
                if (info & 0x08) {
                    double y = factor * oasis_read_integer(in);
                    if (modal_absolute_pos) {
                        modal_geom_pos.y = y;
                    } else {
                        modal_geom_pos.y += y;
                    }
                }
                Array<Vec2>* point_array = &polygon->point_array;
                point_array->ensure_slots(4);
                point_array->size = 4;
                if (info & 0x80) {
                    point_array->items[0] = modal_geom_pos;
                    point_array->items[1] = modal_geom_pos + Vec2{modal_geom_dim.x, -delta_a};
                    point_array->items[2] = modal_geom_pos + modal_geom_dim + Vec2{0, -delta_b};
                    point_array->items[3] = modal_geom_pos + Vec2{0, modal_geom_dim.y};
                } else {
                    point_array->items[0] = modal_geom_pos + Vec2{0, modal_geom_dim.y};
                    point_array->items[1] = modal_geom_pos + Vec2{-delta_a, 0};
                    point_array->items[2] = modal_geom_pos + Vec2{modal_geom_dim.x - delta_b, 0};
                    point_array->items[3] = modal_geom_pos + modal_geom_dim;
                }
                if (info & 0x04) {
                    oasis_read_repetition(in, factor, modal_repetition);
                    polygon->repetition.copy_from(modal_repetition);
                }
            } break;
            case OasisRecord::CTRAPEZOID: {
                Polygon* polygon = (Polygon*)allocate_clear(sizeof(Polygon));
                cell->polygon_array.append(polygon);
                next_property = &polygon->properties;
                uint8_t info;
                oasis_read(&info, 1, 1, in);
                if (info & 0x01) {
                    modal_layer = oasis_read_unsigned_integer(in);
                }
                polygon->layer = modal_layer;
                if (info & 0x02) {
                    modal_datatype = oasis_read_unsigned_integer(in);
                }
                polygon->datatype = modal_datatype;
                if (info & 0x80) {
                    oasis_read(&modal_ctrapezoid_type, 1, 1, in);
                }
                if (info & 0x40) {
                    modal_geom_dim.x = factor * oasis_read_unsigned_integer(in);
                }
                if (info & 0x20) {
                    modal_geom_dim.y = factor * oasis_read_unsigned_integer(in);
                }
                if (info & 0x10) {
                    double x = factor * oasis_read_integer(in);
                    if (modal_absolute_pos) {
                        modal_geom_pos.x = x;
                    } else {
                        modal_geom_pos.x += x;
                    }
                }
                if (info & 0x08) {
                    double y = factor * oasis_read_integer(in);
                    if (modal_absolute_pos) {
                        modal_geom_pos.y = y;
                    } else {
                        modal_geom_pos.y += y;
                    }
                }
                Array<Vec2>* point_array = &polygon->point_array;
                Vec2* v;
                if (modal_ctrapezoid_type > 15 && modal_ctrapezoid_type < 24) {
                    point_array->ensure_slots(3);
                    v = point_array->items;
                    v[0] = modal_geom_pos;
                    v[1] = modal_geom_pos;
                    v[2] = modal_geom_pos;
                    point_array->size = 3;
                } else {
                    point_array->ensure_slots(4);
                    v = point_array->items;
                    v[0] = modal_geom_pos;
                    v[1] = modal_geom_pos + Vec2{modal_geom_dim.x, 0};
                    v[2] = modal_geom_pos + modal_geom_dim;
                    v[3] = modal_geom_pos + Vec2{0, modal_geom_dim.y};
                    point_array->size = 4;
                }
                switch (modal_ctrapezoid_type) {
                    case 0:
                        v[2].x -= modal_geom_dim.y;
                        break;
                    case 1:
                        v[1].x -= modal_geom_dim.y;
                        break;
                    case 2:
                        v[3].x += modal_geom_dim.y;
                        break;
                    case 3:
                        v[0].x += modal_geom_dim.y;
                        break;
                    case 4:
                        v[2].x -= modal_geom_dim.y;
                        v[3].x += modal_geom_dim.y;
                        break;
                    case 5:
                        v[0].x += modal_geom_dim.y;
                        v[1].x -= modal_geom_dim.y;
                        break;
                    case 6:
                        v[1].x -= modal_geom_dim.y;
                        v[3].x += modal_geom_dim.y;
                        break;
                    case 7:
                        v[0].x += modal_geom_dim.y;
                        v[2].x -= modal_geom_dim.y;
                        break;
                    case 8:
                        v[2].y -= modal_geom_dim.x;
                        break;
                    case 9:
                        v[3].y -= modal_geom_dim.x;
                        break;
                    case 10:
                        v[1].y += modal_geom_dim.x;
                        break;
                    case 11:
                        v[0].y += modal_geom_dim.x;
                        break;
                    case 12:
                        v[1].x += modal_geom_dim.x;
                        v[2].x -= modal_geom_dim.x;
                        break;
                    case 13:
                        v[0].x += modal_geom_dim.x;
                        v[3].x -= modal_geom_dim.x;
                        break;
                    case 14:
                        v[1].x += modal_geom_dim.x;
                        v[3].x -= modal_geom_dim.x;
                        break;
                    case 15:
                        v[0].x += modal_geom_dim.x;
                        v[2].x -= modal_geom_dim.x;
                        break;
                    case 16:
                        v[1].x += modal_geom_dim.x;
                        v[2].y += modal_geom_dim.x;
                        break;
                    case 17:
                        v[1] += modal_geom_dim.x;
                        v[2].y += modal_geom_dim.x;
                        break;
                    case 18:
                        v[1].x += modal_geom_dim.x;
                        v[2] += modal_geom_dim.x;
                        break;
                    case 19:
                        v[0].x += modal_geom_dim.x;
                        v[1] += modal_geom_dim.x;
                        v[2].y += modal_geom_dim.x;
                        break;
                    case 20:
                        v[1].x += 2 * modal_geom_dim.y;
                        v[2] += modal_geom_dim.y;
                        break;
                    case 21:
                        v[0].x += modal_geom_dim.y;
                        v[1].x += 2 * modal_geom_dim.y;
                        v[1].y += modal_geom_dim.y;
                        v[2].y += modal_geom_dim.y;
                        break;
                    case 22:
                        v[1] += modal_geom_dim.x;
                        v[2].y += 2 * modal_geom_dim.x;
                        break;
                    case 23:
                        v[0].x += modal_geom_dim.x;
                        v[1].x += modal_geom_dim.x;
                        v[1].y += 2 * modal_geom_dim.x;
                        v[2].y += modal_geom_dim.x;
                        break;
                    case 25:
                        v[2].y = v[3].y = modal_geom_pos.y + modal_geom_dim.x;
                        break;
                }
                if (info & 0x04) {
                    oasis_read_repetition(in, factor, modal_repetition);
                    polygon->repetition.copy_from(modal_repetition);
                }
            } break;
            case OasisRecord::CIRCLE: {
                Polygon* polygon = (Polygon*)allocate_clear(sizeof(Polygon));
                cell->polygon_array.append(polygon);
                next_property = &polygon->properties;
                uint8_t info;
                oasis_read(&info, 1, 1, in);
                if (info & 0x01) {
                    modal_layer = oasis_read_unsigned_integer(in);
                }
                if (info & 0x02) {
                    modal_datatype = oasis_read_unsigned_integer(in);
                }
                if (info & 0x20) {
                    modal_circle_radius = factor * oasis_read_unsigned_integer(in);
                }
                if (info & 0x10) {
                    double x = factor * oasis_read_integer(in);
                    if (modal_absolute_pos) {
                        modal_geom_pos.x = x;
                    } else {
                        modal_geom_pos.x += x;
                    }
                }
                if (info & 0x08) {
                    double y = factor * oasis_read_integer(in);
                    if (modal_absolute_pos) {
                        modal_geom_pos.y = y;
                    } else {
                        modal_geom_pos.y += y;
                    }
                }
                *polygon = ellipse(modal_geom_pos, modal_circle_radius, modal_circle_radius, 0, 0,
                                   0, 0, tolerance, modal_layer, modal_datatype);
                if (info & 0x04) {
                    oasis_read_repetition(in, factor, modal_repetition);
                    polygon->repetition.copy_from(modal_repetition);
                }
            } break;
            case OasisRecord::PROPERTY:
            case OasisRecord::LAST_PROPERTY: {
                Property* property = (Property*)allocate_clear(sizeof(Property));
                *next_property = property;
                next_property = &property->next;
                uint8_t info;
                if (record == OasisRecord::LAST_PROPERTY) {
                    info = 0x08;
                } else {
                    oasis_read(&info, 1, 1, in);
                }
                if (info & 0x04) {
                    // Explicit name
                    if (info & 0x02) {
                        // Reference number
                        property->name = (char*)oasis_read_unsigned_integer(in);
                        unfinished_property_name.append(property);
                        modal_property_unfinished = true;
                    } else {
                        property->name = (char*)oasis_read_string(in, true, len);
                        modal_property_unfinished = false;
                    }
                    modal_property = property;
                } else {
                    // Use modal variable
                    if (modal_property_unfinished) {
                        property->name = modal_property->name;
                        unfinished_property_name.append(property);
                    } else {
                        property->name = copy_string(modal_property->name, len);
                    }
                }
                if (info & 0x08) {
                    // Use modal value list
                    property->value = property_values_copy(modal_property_value_list);
                    PropertyValue* src = modal_property_value_list;
                    PropertyValue* dst = property->value;
                    while (src) {
                        if (src->type == PropertyType::UnsignedInteger &&
                            unfinished_property_value.contains(src)) {
                            unfinished_property_value.append(dst);
                        }
                        src = src->next;
                        dst = dst->next;
                    }
                } else {
                    // Explicit value list
                    uint64_t count = info >> 4;
                    if (count == 15) {
                        count = oasis_read_unsigned_integer(in);
                    }
                    PropertyValue** next = &property->value;
                    for (; count > 0; count--) {
                        PropertyValue* property_value =
                            (PropertyValue*)allocate_clear(sizeof(PropertyValue));
                        *next = property_value;
                        next = &property_value->next;
                        OasisDataType data_type;
                        oasis_read(&data_type, 1, 1, in);
                        switch (data_type) {
                            case OasisDataType::RealPositiveInteger:
                            case OasisDataType::RealNegativeInteger:
                            case OasisDataType::RealPositiveReciprocal:
                            case OasisDataType::RealNegativeReciprocal:
                            case OasisDataType::RealPositiveRatio:
                            case OasisDataType::RealNegativeRatio:
                            case OasisDataType::RealFloat:
                            case OasisDataType::RealDouble: {
                                property_value->type = PropertyType::Real;
                                property_value->real = oasis_read_real_by_type(in, data_type);
                            } break;
                            case OasisDataType::UnsignedInteger: {
                                property_value->type = PropertyType::UnsignedInteger;
                                property_value->unsigned_integer = oasis_read_unsigned_integer(in);
                            } break;
                            case OasisDataType::SignedInteger: {
                                property_value->type = PropertyType::Integer;
                                property_value->integer = oasis_read_integer(in);
                            } break;
                            case OasisDataType::AString:
                            case OasisDataType::BString:
                            case OasisDataType::NString: {
                                property_value->type = PropertyType::String;
                                property_value->bytes =
                                    oasis_read_string(in, false, property_value->size);
                            } break;
                            case OasisDataType::ReferenceA:
                            case OasisDataType::ReferenceB:
                            case OasisDataType::ReferenceN: {
                                property_value->type = PropertyType::UnsignedInteger;
                                property_value->unsigned_integer = oasis_read_unsigned_integer(in);
                                unfinished_property_value.append(property_value);
                            } break;
                        }
                    }
                    modal_property_value_list = property->value;
                }
            } break;
            case OasisRecord::XNAME_IMPLICIT: {
                oasis_read_unsigned_integer(in);
                free_allocation(oasis_read_string(in, false, len));
                fputs("[GDSTK] Record type XNAME ignored.\n", stderr);
            } break;
            case OasisRecord::XNAME: {
                oasis_read_unsigned_integer(in);
                free_allocation(oasis_read_string(in, false, len));
                oasis_read_unsigned_integer(in);
                fputs("[GDSTK] Record type XNAME ignored.\n", stderr);
            } break;
            case OasisRecord::XELEMENT: {
                oasis_read_unsigned_integer(in);
                free_allocation(oasis_read_string(in, false, len));
                fputs("[GDSTK] Record type XELEMENT ignored.\n", stderr);
            } break;
            case OasisRecord::XGEOMETRY: {
                uint8_t info;
                oasis_read(&info, 1, 1, in);
                oasis_read_unsigned_integer(in);
                if (info & 0x01) {
                    modal_layer = oasis_read_unsigned_integer(in);
                }
                if (info & 0x02) {
                    modal_datatype = oasis_read_unsigned_integer(in);
                }
                free_allocation(oasis_read_string(in, false, len));
                if (info & 0x10) {
                    double x = factor * oasis_read_integer(in);
                    if (modal_absolute_pos) {
                        modal_geom_pos.x = x;
                    } else {
                        modal_geom_pos.x += x;
                    }
                }
                if (info & 0x08) {
                    double y = factor * oasis_read_integer(in);
                    if (modal_absolute_pos) {
                        modal_geom_pos.y = y;
                    } else {
                        modal_geom_pos.y += y;
                    }
                }
                if (info & 0x04) {
                    oasis_read_repetition(in, factor, modal_repetition);
                }
                fputs("[GDSTK] Record type XGEOMETRY ignored.\n", stderr);
            } break;
            case OasisRecord::CBLOCK: {
                if (oasis_read_unsigned_integer(in) != 0) {
                    fputs("[GDSTK] CBLOCK compression method not supported.\n", stderr);
                    oasis_read_unsigned_integer(in);
                    len = oasis_read_unsigned_integer(in);
                    fseek(in.file, len, SEEK_SET);
                } else {
                    z_stream s = {0};
                    // TODO: use custom allocator
                    in.data_size = oasis_read_unsigned_integer(in);
                    s.avail_out = in.data_size;
                    s.avail_in = oasis_read_unsigned_integer(in);
                    in.data = (uint8_t*)allocate(in.data_size);
                    in.cursor = in.data;
                    s.next_out = in.data;
                    uint8_t* data = (uint8_t*)allocate(s.avail_in);
                    s.next_in = (Bytef*)data;
                    if (fread(s.next_in, 1, s.avail_in, in.file) != s.avail_in) {
                        fputs("[GDSTK] Unable to read full CBLOCK.\n", stderr);
                    }
                    if (inflateInit2(&s, -15) != Z_OK) {
                        fputs("[GDSTK] Unable to initialize zlib.\n", stderr);
                    }
                    int ret = inflate(&s, Z_FINISH);
                    if (ret != Z_STREAM_END) {
                        fputs("[GDSTK] Unable to decompress CBLOCK.\n", stderr);
                    }
                    free_allocation(data);
                    inflateEnd(&s);
                }
            } break;
            default:
                fprintf(stderr, "[GDSTK] Unknown record type <0x%02X>.\n", (uint8_t)record);
        }
    }
    fclose(in.file);

    ByteArray* ba = cell_name_table.items;
    for (uint64_t i = cell_name_table.size; i > 0; i--, ba++) {
        if (ba->bytes) free_allocation(ba->bytes);
        properties_clear(ba->properties);
    }
    cell_name_table.clear();

    ba = label_text_table.items;
    for (uint64_t i = label_text_table.size; i > 0; i--, ba++) {
        if (ba->bytes) free_allocation(ba->bytes);
        properties_clear(ba->properties);
    }
    label_text_table.clear();

    ba = property_name_table.items;
    for (uint64_t i = property_name_table.size; i > 0; i--, ba++) {
        if (ba->bytes) free_allocation(ba->bytes);
        properties_clear(ba->properties);
    }
    property_name_table.clear();

    ba = property_value_table.items;
    for (uint64_t i = property_value_table.size; i > 0; i--, ba++) {
        if (ba->bytes) free_allocation(ba->bytes);
        properties_clear(ba->properties);
    }
    property_value_table.clear();

    modal_repetition.clear();
    modal_polygon_points.clear();
    modal_path_points.clear();

    unfinished_property_name.clear();
    unfinished_property_value.clear();

    return library;
}

int gds_units(const char* filename, double& unit, double& precision) {
    uint8_t buffer[65537];
    uint64_t* data64 = (uint64_t*)(buffer + 4);
    FILE* in = fopen(filename, "rb");
    if (in == NULL) {
        fputs("[GDSTK] Unable to open GDSII file for input.\n", stderr);
        return -1;
    }

    while (gdsii_read_record(in, buffer) > 0) {
        if (buffer[2] == 0x03) {  // UNITS
            big_endian_swap64(data64, 2);
            precision = gdsii_real_to_double(data64[1]);
            unit = precision / gdsii_real_to_double(data64[0]);
            fclose(in);
            return 0;
        }
    }
    fputs("[GDSTK] GDSII file missing units definition.\n", stderr);
    fclose(in);
    return -1;
}

int oas_precision(const char* filename, double& precision) {
    FILE* in = fopen(filename, "rb");
    if (in == NULL) {
        fputs("[GDSTK] Unable to open OASIS file for input.\n", stderr);
        fclose(in);
        return -1;
    }

    // Check header bytes and START record
    char header[14];
    if (fread(header, 1, 14, in) < 14 || memcmp(header, "%SEMI-OASIS\r\n\x01", 14) != 0) {
        fputs("[GDSTK] Invalid OASIS header found.\n", stderr);
        fclose(in);
        return -1;
    }

    // Process START record
    OasisStream s = {in, NULL, NULL, 0};
    uint64_t len;
    uint8_t* version = oasis_read_string(s, false, len);
    if (memcmp(version, "1.0", 3) != 0) {
        fputs("[GDSTK] Unsupported OASIS file version.\n", stderr);
    }
    free_allocation(version);

    precision = 1e-6 / oasis_read_real(s);
    fclose(in);
    return 0;
}

}  // namespace gdstk
