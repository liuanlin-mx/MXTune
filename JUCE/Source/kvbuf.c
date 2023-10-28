/*****************************************************************************
*                                                                            *
*  Copyright (C) 2019 Liu An Lin.                                            *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include "kvbuf.h"


typedef struct kvbuf_buffer
{
	char *buffer;
	int offset;
	int size;
}kvbuf_buffer;




static const unsigned char *get_arr_len(const unsigned char *begin, const unsigned char *end, unsigned int *arr_len);
static unsigned char *set_arr_len(unsigned char *begin, unsigned char *end, unsigned int arr_len);

static const unsigned char *parse_value(kvbuf_hooks *hooks, kvbuf *item, const unsigned char *begin, const unsigned char *end);
static const unsigned char *parse_key(kvbuf *item, const unsigned char *begin, const unsigned char *end, int key_len);
static const unsigned char *parse_object(kvbuf_hooks *hooks, kvbuf *item, const unsigned char *begin, const unsigned char *end);
static const unsigned char *parse_number(kvbuf *item, const unsigned char *begin, const unsigned char *end);
static const unsigned char *parse_ext_type(kvbuf *item, const unsigned char *begin, const unsigned char *end);
static const unsigned char *parse_array(kvbuf_hooks *hooks, kvbuf *item, const unsigned char *begin, const unsigned char *end);

static void print_value(kvbuf_hooks *hooks, kvbuf *item, kvbuf_buffer *p, int depth, int fmt, int print_key);
static void print_object(kvbuf_hooks *hooks, kvbuf *item, kvbuf_buffer *p, int depth, int fmt, int print_key);
static void print_array(kvbuf_hooks *hooks, kvbuf *item, kvbuf_buffer *p, int depth, int fmt, int print_key);
static void print_number(kvbuf_hooks *hooks, kvbuf *item, kvbuf_buffer *p, int depth, int fmt, int print_key);


static void buffer_write_string(kvbuf_hooks *hooks, kvbuf_buffer *p, const char *string);
static void buffer_write_int32(kvbuf_hooks *hooks, kvbuf_buffer *p, int number);
static void buffer_write_uint32(kvbuf_hooks *hooks, kvbuf_buffer *p, unsigned int number);
static void buffer_write_int64(kvbuf_hooks *hooks, kvbuf_buffer *p, long long number);
static void buffer_write_uint64(kvbuf_hooks *hooks, kvbuf_buffer *p, unsigned long long number);
static void buffer_write_float32(kvbuf_hooks *hooks, kvbuf_buffer *p, float number);
static void buffer_write_float64(kvbuf_hooks *hooks, kvbuf_buffer *p, double number);
static void buffer_write_float128(kvbuf_hooks *hooks, kvbuf_buffer *p, long double number);
static void buffer_write_table(kvbuf_hooks *hooks, kvbuf_buffer *p, int n);
static void buffer_write_zero(kvbuf_hooks *hooks, kvbuf_buffer *p);

static int max(int x, int y) {return (x > y)? x: y;}


static inline unsigned int get_v(const unsigned char *data, int len)
{
    unsigned int v = 0;
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
    switch (len)
    {
        case 4:
            v |= (data[3] << 24);
        case 3:
            v |= (data[2] << 16);
        case 2:
            v |= (data[1] << 8);
        case 1:
            v |= data[0];
            break;
        default:
            v = 0;
            break;
    }
#pragma GCC diagnostic warning "-Wimplicit-fallthrough"
 
    return v;
}

static inline void set_v(unsigned char *data, unsigned int v, int len)
{
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
    switch (len)
    {
        case 4:
            data[3] = 0xff & (v >> 24);
        case 3:
            data[2] = 0xff & (v >> 16);
        case 2:
            data[1] = 0xff & (v >> 8);
        case 1:
            data[0] = v & 0xff;
            break;
        default:
            break;
    }
#pragma GCC diagnostic warning "-Wimplicit-fallthrough"
}

static inline short get_int16(const unsigned char *data)
{
    return (((unsigned short)data[0]) | (((unsigned short)data[1]) << 8));
}

static inline unsigned short get_uint16(const unsigned char *data)
{
    return (((unsigned short)data[0]) | (((unsigned )data[1]) << 8));
}


static inline int get_int32(const unsigned char *data)
{
    return (((unsigned int)data[0]) | (((unsigned int)data[1]) << 8)
        | (((unsigned int)data[2]) << 16) | (((unsigned int)data[3]) << 24));
}

static inline unsigned int get_uint32(const unsigned char *data)
{
    return (((unsigned int)data[0]) | (((unsigned int)data[1]) << 8)
        | (((unsigned int)data[2]) << 16) | (((unsigned int)data[3]) << 24));
}


static inline long long get_int64(const unsigned char *data)
{
    return (((unsigned long long)data[0]) | (((unsigned long long)data[1]) << 8)
        | (((unsigned long long)data[2]) << 16) | (((unsigned long long)data[3]) << 24)
        | (((unsigned long long)data[4]) << 32) | (((unsigned long long)data[5]) << 40)
        | (((unsigned long long)data[6]) << 48) | (((unsigned long long)data[7]) << 56));
}

static inline unsigned long long get_uint64(const unsigned char *data)
{
    return (((unsigned long long)data[0]) | (((unsigned long long)data[1]) << 8)
        | (((unsigned long long)data[2]) << 16) | (((unsigned long long)data[3]) << 24)
        | (((unsigned long long)data[4]) << 32) | (((unsigned long long)data[5]) << 40)
        | (((unsigned long long)data[6]) << 48) | (((unsigned long long)data[7]) << 56));
}

static inline float get_float16(const unsigned char *data)
{
    return 0;
}

static inline float get_float32(const unsigned char *data)
{
    float tmp;
    unsigned char *p = (unsigned char*)&tmp;
    p[0] = data[0];
    p[1] = data[1];
    p[2] = data[2];
    p[3] = data[3];
    return tmp;
}

static inline double get_float64(const unsigned char *data)
{
    double tmp;
    unsigned char *p = (unsigned char*)&tmp;
    p[0] = data[0];
    p[1] = data[1];
    p[2] = data[2];
    p[3] = data[3];
    p[4] = data[4];
    p[5] = data[5];
    p[6] = data[6];
    p[7] = data[7];
    return tmp;
}

static inline long double get_float128(const unsigned char *data)
{
    long double tmp;
    unsigned char *p = (unsigned char*)&tmp;
    p[0] = data[0];
    p[1] = data[1];
    p[2] = data[2];
    p[3] = data[3];
    p[4] = data[4];
    p[5] = data[5];
    p[6] = data[6];
    p[7] = data[7];
    
    p[8] = data[8];
    p[9] = data[9];
    p[10] = data[10];
    p[11] = data[11];
    p[12] = data[12];
    p[13] = data[13];
    p[14] = data[14];
    p[15] = data[15];
    return tmp;
}

static inline void set_uint16(unsigned char *data, unsigned short v)
{
    data[0] = 0xff & (v);
    data[1] = 0xff & (v >> 8);
}

static inline void set_uint32(unsigned char *data, unsigned int v)
{
    data[0] = 0xff & (v);
    data[1] = 0xff & (v >> 8);
    data[2] = 0xff & (v >> 16);
    data[3] = 0xff & (v >> 24);
}

static inline void set_uint64(unsigned char *data, unsigned long long v)
{
    data[0] = 0xff & (v);
    data[1] = 0xff & (v >> 8);
    data[2] = 0xff & (v >> 16);
    data[3] = 0xff & (v >> 24);
    data[4] = 0xff & (v >> 32);
    data[5] = 0xff & (v >> 40);
    data[6] = 0xff & (v >> 48);
    data[7] = 0xff & (v >> 56);
}


static inline void set_float32(unsigned char *data, float v)
{
    unsigned char *p = (unsigned char *)&v;
    data[0] = p[0];
    data[1] = p[1];
    data[2] = p[2];
    data[3] = p[3];
}

static inline void set_float64(unsigned char *data, double v)
{
    unsigned char *p = (unsigned char *)&v;
    data[0] = p[0];
    data[1] = p[1];
    data[2] = p[2];
    data[3] = p[3];
    data[4] = p[4];
    data[5] = p[5];
    data[6] = p[6];
    data[7] = p[7];
}

static inline void set_float128(unsigned char *data, long double v)
{
    unsigned char *p = (unsigned char *)&v;
    data[0] = p[0];
    data[1] = p[1];
    data[2] = p[2];
    data[3] = p[3];
    data[4] = p[4];
    data[5] = p[5];
    data[6] = p[6];
    data[7] = p[7];
    data[8] = p[8];
    data[9] = p[9];
    data[10] = p[10];
    data[11] = p[11];
    data[12] = p[12];
    data[13] = p[13];
    data[14] = p[14];
    data[15] = p[15];
}

static const unsigned char *get_arr_len(const unsigned char *begin, const unsigned char *end, unsigned int *arr_len)
{
    unsigned int move = 0;
    *arr_len = 0;
    while (1)
    {
        if (begin >= end) {return end + 1;}
        *arr_len |= (*begin & 0x7f) << move;
        if ((*begin & 0x80) == 0)
        {
            return begin + 1;
        }
        ++begin;
        move += 7;
    }
    
#if 0
    if (begin >= end) {return end;}
    *arr_len = (*begin & 0x7f);
    if ((*begin & 0x80) == 0) {return begin + 1;}
    
    if (++begin >= end) {return end;}
    *arr_len |= (*begin & 0x7f) << 7;
    if ((*begin & 0x80) == 0) {return begin + 1;}

    if (++begin >= end) {return end;}
    *arr_len |= (*begin & 0x7f) << 14;
    if ((*begin & 0x80) == 0) {return begin + 1;}

    if (++begin >= end) {return end;}
    *arr_len |= (*begin & 0x7f) << 21;
    if ((*begin & 0x80) == 0) {return begin + 1;}
    
    return begin + 1;
#endif
}


static unsigned char *set_arr_len(unsigned char *begin, unsigned char *end, unsigned int arr_len)
{
    do
    {
        if (begin >= end) {return end + 1;}
        *begin = (arr_len &  0x7f);
        arr_len >>= 7;
        *begin |= (arr_len)? 0x80: 0x00;
        ++begin;
    }
    while (arr_len > 0);
    return begin;
    
#if 0
    if (begin >= end) {return end;}
    *begin = (arr_len &  0x7f); arr_len >>= 7; *begin |= (arr_len)? 0x80: 0x00; ++begin;
    if (arr_len == 0) {return begin;}
    
    if (begin >= end) {return end;}
    *begin = (arr_len &  0x7f); arr_len >>= 7; *begin |= (arr_len)? 0x80: 0x00; ++begin;
    if (arr_len == 0) {return begin;}
    
    if (begin >= end) {return end;}
    *begin = (arr_len &  0x7f); arr_len >>= 7; *begin |= (arr_len)? 0x80: 0x00; ++begin;
    if (arr_len == 0) {return begin;}
    
    if (begin >= end) {return end;}
    *begin = (arr_len &  0x7f); arr_len >>= 7; ++begin;
    if (arr_len == 0) {return begin;}
    return begin;
#endif
}


static unsigned int get_key_len(unsigned int key)
{
    if (key < 256)
    {
        return 1;
    }
    else if (key < 65536)
    {
        return 2;
    }
    else if (key < 0x1000000)
    {
        return 3;
    }
    return 4;
}



static kvbuf *kvbuf_new_item(kvbuf_hooks *hooks)
{
	kvbuf *item = hooks->malloc(sizeof(kvbuf));
	if (item)
    {
		memset(item, 0, sizeof(kvbuf));
	}
	return item;
}

void kvbuf_delete(kvbuf_hooks *hooks, kvbuf *item)
{
	kvbuf *next;
	while (item)
    {
		next = item->next;
		if (item->child) kvbuf_delete(hooks, item->child);
        hooks->free(item);
		item = next;
	}
}

void kvbuf_free(kvbuf_hooks *hooks, void *p)
{
    hooks->free(p);
}

kvbuf *kvbuf_parse(kvbuf_hooks *hooks, const unsigned char *begin, const unsigned char *end)
{
	kvbuf *c = kvbuf_new_item(hooks);
	if (!c) {return 0;}
	
	const unsigned char *r = parse_value(hooks, c, begin, end);
	if (r > end) {kvbuf_delete(hooks, c);return 0;}
	return c;
}

static const unsigned char *parse_value(kvbuf_hooks *hooks, kvbuf *item, const unsigned char *begin, const unsigned char *end)
{
    if (begin == end)
    {
        return end;
    }
    else if (begin > end)
    {
        return end + 1;
    }
    
    if ((*begin & KVBUF_VTYPE_ARRAY) != 0) {return parse_array(hooks, item, begin, end);}
    if ((*begin & KVBUF_VTYPE_MASK) == KVBUF_VTYPE_OBJECT_BEGIN) {return parse_object(hooks, item, begin, end);}
    if ((*begin & KVBUF_VTYPE_MASK) == KVBUF_VTYPE_EXT) {return parse_ext_type(item, begin, end);}
    if ((*begin & KVBUF_VTYPE_MASK) <= KVBUF_VTYPE_NUMBER) {return parse_number(item, begin, end);}
    return end + 1;
}

static const unsigned char *parse_object(kvbuf_hooks *hooks, kvbuf *item, const unsigned char *begin, const unsigned char *end)
{
	kvbuf *child;
    int key_len = ((*begin & KVBUF_KEY_SIZE_EMASK) >> 6) + 1;
	if ((*begin & KVBUF_VTYPE_MASK) != KVBUF_VTYPE_OBJECT_BEGIN) {return end + 1;}
    
	item->type = KV_BUF_OBJECT;
	if (++begin == end) {return end + 1;}
	
	begin = parse_key(item, begin, end, key_len);
	if (begin >= end) {return end + 1;}
    
	if ((*begin & KVBUF_VTYPE_MASK) == KVBUF_VTYPE_OBJECT_END) {return begin + 1;}
    
	item->child = child = kvbuf_new_item(hooks);
	if (begin >= end) {return end + 1;}
    
	begin = parse_value(hooks, child, begin, end);
	if (!begin) {return end + 1;}
    
	while (begin < end)
    {
		kvbuf *new_item;
        if ((*begin & KVBUF_VTYPE_MASK) == KVBUF_VTYPE_OBJECT_END) {break;}
        
        if (!(new_item = kvbuf_new_item(hooks))) {return end + 1;}
		child->next = new_item; new_item->prev = child; child = new_item;
        
		begin = parse_value(hooks, child, begin, end);
        if (begin >= end) {return end + 1;}
	}
    
    if ((*begin & KVBUF_VTYPE_MASK) == KVBUF_VTYPE_OBJECT_END) {return begin + 1;}
    
	return end + 1;
}

static const unsigned char *parse_key(kvbuf *item, const unsigned char *begin, const unsigned char *end, int key_len)
{
    if (begin + key_len >= end) {return end + 1;}
    
    item->key = get_v(begin, key_len);
	
	return begin + key_len;
}

static const unsigned char *parse_number(kvbuf *item, const unsigned char *begin, const unsigned char *end)
{
    int key_len = ((*begin & KVBUF_KEY_SIZE_EMASK) >> 6) + 1;
	if ((*begin & KVBUF_VTYPE_MASK) > KVBUF_VTYPE_NUMBER) {return end + 1;}
    
	item->type = *begin & KVBUF_VTYPE_MASK;
	if (++begin == end) {return end + 1;}
	
	begin = parse_key(item, begin, end, key_len);
	if (begin == end) {return end + 1;}
    
    switch (item->type)
    {
        case KV_BUF_INT8: {item->number.i8 = (char)*begin; ++begin; break;}
        case KV_BUF_INT16: {if (begin + 2 > end) {return end + 1;} item->number.i16 = get_int16(begin); begin += 2; break;}
        case KV_BUF_INT32: {if (begin + 4 > end) {return end + 1;} item->number.i32 = get_int32(begin); begin += 4; break;}
        case KV_BUF_INT64: {if (begin + 8 > end) {return end + 1;} item->number.i64 = get_int64(begin); begin += 8; break;}
        case KV_BUF_INT128: {if (begin + 16 > end) {return end + 1;} /*item->number.i128 = get_int128(begin);*/ begin += 16; break;}
        
        case KV_BUF_UINT8: {item->number.u8 = *begin; ++begin; break;}
        case KV_BUF_UINT16: {if (begin + 2 > end) {return end + 1;} item->number.u16 = get_uint16(begin); begin += 2; break;}
        case KV_BUF_UINT32: {if (begin + 4 > end) {return end + 1;} item->number.u32 = get_uint32(begin); begin += 4; break;}
        case KV_BUF_UINT64: {if (begin + 8 > end) {return end + 1;} item->number.u64 = get_uint64(begin); begin += 8; break;}
        case KV_BUF_UINT128: {if (begin + 16 > end) {return end + 1;} /*item->number.u128 = get_uint128(begin);*/ begin += 16; break;}
        
        case KV_BUF_FLOAT16: {if (begin + 2 > end) {return end + 1;} /*item->number.f16 = get_float16(begin);*/ begin += 2; break;}
        case KV_BUF_FLOAT32: {if (begin + 4 > end) {return end + 1;} item->number.f32 = get_float32(begin); begin += 4; break;}
        case KV_BUF_FLOAT64: {if (begin + 8 > end) {return end + 1;} item->number.f64 = get_float64(begin); begin += 8; break;}
        case KV_BUF_FLOAT128: {if (begin + 16 > end) {return end + 1;} item->number.f128 = get_float128(begin); begin += 16; break;}
        default: {return end + 1;}
    }
    
    return begin;
}

static const unsigned char *parse_ext_type(kvbuf *item, const unsigned char *begin, const unsigned char *end)
{
    if (begin + 2 >= end) { return end + 1; }
    int key_len = ((*begin & KVBUF_KEY_SIZE_EMASK) >> 6) + 1;
    item->type = KV_BUF_UNKNOWN;

    unsigned char value_len = *begin;
    ++begin;
    
    begin = parse_key(item, begin, end, key_len);
    
    if (begin + value_len > end) {return end + 1;}
    begin += value_len;
    
    return begin;
}

static const unsigned char *parse_number_in_array(kvbuf *item, int type, const unsigned char *begin, const unsigned char *end)
{
    item->type = type & KVBUF_VTYPE_MASK;
    if (begin >= end) {return end + 1;}
    switch (type)
    {
        case KV_BUF_INT8: {item->number.i8 = (char)*begin; ++begin; break;}
        case KV_BUF_INT16: {if (begin + 2 > end) {return end + 1;} item->number.i16 = get_int16(begin); begin += 2; break;}
        case KV_BUF_INT32: {if (begin + 4 > end) {return end + 1;} item->number.i32 = get_int32(begin); begin += 4; break;}
        case KV_BUF_INT64: {if (begin + 8 > end) {return end + 1;} item->number.i64 = get_int64(begin); begin += 8; break;}
        case KV_BUF_INT128: {if (begin + 16 > end) {return end + 1;} /*item->number.i128 = get_int128(begin);*/ begin += 16; break;}
        
        case KV_BUF_UINT8: {item->number.u8 = *begin; ++begin; break;}
        case KV_BUF_UINT16: {if (begin + 2 > end) {return end + 1;} item->number.u16 = get_uint16(begin); begin += 2; break;}
        case KV_BUF_UINT32: {if (begin + 4 > end) {return end + 1;} item->number.u32 = get_uint32(begin); begin += 4; break;}
        case KV_BUF_UINT64: {if (begin + 8 > end) {return end + 1;} item->number.u64 = get_uint64(begin); begin += 8; break;}
        case KV_BUF_UINT128: {if (begin + 16 > end) {return end + 1;} /*item->number.u128 = get_uint128(begin);*/ begin += 16; break;}
        
        case KV_BUF_FLOAT16: {if (begin + 2 > end) {return end + 1;} /*item->number.f32 = get_float16(begin);*/ begin += 2; break;}
        case KV_BUF_FLOAT32: {if (begin + 4 > end) {return end + 1;} item->number.f32 = get_float32(begin); begin += 4; break;}
        case KV_BUF_FLOAT64: {if (begin + 8 > end) {return end + 1;} item->number.f64 = get_float64(begin); begin += 8; break;}
        case KV_BUF_FLOAT128: {if (begin + 16 > end) {return end + 1;} item->number.f128 = get_float128(begin); begin += 16; break;}
        default: {return end + 1;}
    }
    
    return begin;
}


const unsigned char *parse_object_in_array(kvbuf_hooks *hooks, kvbuf *item, const unsigned char *begin, const unsigned char *end)
{
	kvbuf *child;
	if ((*begin & KVBUF_VTYPE_MASK) != KVBUF_VTYPE_OBJECT_BEGIN) {return end + 1;}
    
	item->type = KV_BUF_OBJECT;
	if (++begin == end) {return end + 1;}
    
	if ((*begin & KVBUF_VTYPE_MASK) == KVBUF_VTYPE_OBJECT_END) {return begin + 1;}
    
	item->child = child = kvbuf_new_item(hooks);
	if (begin >= end) {return end + 1;}
    
	begin = parse_value(hooks, child, begin, end);
	if (!begin) {return end + 1;}
    
	while (begin < end)
    {
		kvbuf *new_item;
        if ((*begin & KVBUF_VTYPE_MASK) == KVBUF_VTYPE_OBJECT_END) {break;}
        
        if (!(new_item = kvbuf_new_item(hooks))) {return end + 1;}
		child->next = new_item; new_item->prev = child; child = new_item;
        
		begin = parse_value(hooks, child, begin, end);
        if (begin >= end) {return end + 1;}
	}
    
    if ((*begin & KVBUF_VTYPE_MASK) == KVBUF_VTYPE_OBJECT_END) {return begin + 1;}
    
	return end + 1;
}

const unsigned char *parse_array_in_array(kvbuf_hooks *hooks, kvbuf *item, const unsigned char *begin, const unsigned char *end)
{
    kvbuf *child;
    unsigned int arr_len = 0;
    unsigned char value_len = 0;
    item->type = KV_BUF_ARRAY;
    
    if (begin + 2 >= end) { return end + 1; }
    if ((*begin & KVBUF_VTYPE_ARRAY) == 0) {return end + 1;}
    
    int type = *begin & KVBUF_VTYPE_MASK;
    
    if (type == KVBUF_VTYPE_EXT)
    {
        ++begin;
        value_len = *begin;
    }
    
    ++begin;
    begin = get_arr_len(begin, end, &arr_len);
    if (begin >= end) {return end + 1;}
    if (arr_len == 0) {return begin;}
	
    item->child = child = kvbuf_new_item(hooks);
    if (begin >= end) {return end + 1;}
    
    if (type <= KVBUF_VTYPE_NUMBER)
    {
        begin = parse_number_in_array(child, type, begin, end);
        if (begin >= end) {return end + 1;}
        --arr_len;
        for (unsigned int i = 0; i < arr_len; ++i)
        {
            kvbuf *new_item;
            if (!(new_item = kvbuf_new_item(hooks))) {return end + 1;}
            child->next = new_item; new_item->prev = child; child = new_item;
        
            begin = parse_number_in_array(child, type, begin, end);
            if (begin >= end) {return end + 1;}
        }
    }
    else if (type == KVBUF_VTYPE_OBJECT_BEGIN)
    {
        begin = parse_object_in_array(hooks, child, begin, end);
        if (begin >= end) {return end + 1;}
        --arr_len;
        for (unsigned int i = 0; i < arr_len; ++i)
        {
            kvbuf *new_item;
            if (!(new_item = kvbuf_new_item(hooks))) {return end + 1;}
            child->next = new_item; new_item->prev = child; child = new_item;
        
            begin = parse_object_in_array(hooks, child, begin, end);
            if (begin >= end) {return end + 1;}
        }
    }
    else if (type == KVBUF_VTYPE_NEST_ARRAY)
    {
        begin = parse_array_in_array(hooks, child, begin, end);
        if (begin >= end) {return end + 1;}
        --arr_len;
        for (unsigned int i = 0; i < arr_len; ++i)
        {
            kvbuf *new_item;
            if (!(new_item = kvbuf_new_item(hooks))) {return end + 1;}
            child->next = new_item; new_item->prev = child; child = new_item;
        
            begin = parse_array_in_array(hooks, child, begin, end);
            if (begin >= end) {return end + 1;}
        }
    }
    else if (type == KVBUF_VTYPE_EXT)
    {
        child->type = KV_BUF_UNKNOWN;
        begin += value_len;
        if (begin >= end) {return end + 1;}
        --arr_len;
        for (unsigned int i = 0; i < arr_len; ++i)
        {
            begin += value_len;
            if (begin >= end) { return end + 1; }
        }
    }
    else
    {
        return end + 1;
    }
    
    if ((*begin & (KVBUF_VTYPE_ARRAY | KVBUF_VTYPE_MASK)) == (KVBUF_VTYPE_ARRAY | type)) {return begin + 1;}
    
	return end + 1;
}


static const unsigned char *parse_array(kvbuf_hooks *hooks, kvbuf *item, const unsigned char *begin, const unsigned char *end)
{
    kvbuf *child;
    unsigned int arr_len = 0;
    unsigned char value_len = 0;
    item->type = KV_BUF_ARRAY;
    
    if (begin + 2 >= end) { return end + 1; }
    if ((*begin & KVBUF_VTYPE_ARRAY) == 0) {return end + 1;}
    int key_len = ((*begin & KVBUF_KEY_SIZE_EMASK) >> 6) + 1;
    int type = *begin & KVBUF_VTYPE_MASK;
    
    if (type == KVBUF_VTYPE_EXT)
    {
        ++begin;
        value_len = *begin;
    }
    
    ++begin;
    
    begin = parse_key(item, begin, end, key_len);
    if (begin >= end) {return end + 1;}
    
    begin = get_arr_len(begin, end, &arr_len);
    if (begin >= end) {return end + 1;}
    
    if (arr_len > 0)
    {
        if (arr_len == 0) {return begin;}
        
        item->child = child = kvbuf_new_item(hooks);
        if (begin >= end) {return end + 1;}
        
        if (type <= KVBUF_VTYPE_NUMBER)
        {
            begin = parse_number_in_array(child, type, begin, end);
            if (begin >= end) {return end + 1;}
            --arr_len;
            for (unsigned int i = 0; i < arr_len; ++i)
            {
                kvbuf *new_item;
                if (!(new_item = kvbuf_new_item(hooks))) {return end + 1;}
                child->next = new_item; new_item->prev = child; child = new_item;
            
                begin = parse_number_in_array(child, type, begin, end);
                if (begin >= end) {return end + 1;}
            }
        }
        else if (type == KVBUF_VTYPE_OBJECT_BEGIN)
        {
            begin = parse_object_in_array(hooks, child, begin, end);
            if (begin >= end) {return end + 1;}
            --arr_len;
            for (unsigned int i = 0; i < arr_len; ++i)
            {
                kvbuf *new_item;
                if (!(new_item = kvbuf_new_item(hooks))) {return end + 1;}
                child->next = new_item; new_item->prev = child; child = new_item;
            
                begin = parse_object_in_array(hooks, child, begin, end);
                if (begin >= end) {return end + 1;}
            }
        }
        else if (type == KVBUF_VTYPE_NEST_ARRAY)
        {
            begin = parse_array_in_array(hooks, child, begin, end);
            if (begin >= end) {return end + 1;}
            --arr_len;
            for (unsigned int i = 0; i < arr_len; ++i)
            {
                kvbuf *new_item;
                if (!(new_item = kvbuf_new_item(hooks))) {return end + 1;}
                child->next = new_item; new_item->prev = child; child = new_item;
            
                begin = parse_array_in_array(hooks, child, begin, end);
                if (begin >= end) {return end + 1;}
            }
        }
        else if (type == KVBUF_VTYPE_EXT)
        {
            child->type = KV_BUF_UNKNOWN;
            begin += value_len;
            if (begin >= end) {return end + 1;}
            --arr_len;
            for (unsigned int i = 0; i < arr_len; ++i)
            {
                begin += value_len;
                if (begin >= end) { return end + 1; }
            }
        }
        else
        {
            return end + 1;
        }
    }
    if ((*begin & (KVBUF_VTYPE_ARRAY | KVBUF_VTYPE_MASK)) == (KVBUF_VTYPE_ARRAY | type)) {return begin + 1;}
    
	return end + 1;
}


static unsigned char *build_value(kvbuf *item, unsigned char *begin, unsigned char *end);
static unsigned char *build_number_in_array(kvbuf *item, unsigned char *begin, unsigned char *end);

static unsigned char *build_object(kvbuf *item, unsigned char *begin, unsigned char *end)
{
	kvbuf *child;
	if (!item) {return end;}
	if (begin == end) {return end;}
    
    unsigned int key_len = get_key_len(item->key);
    *begin = (((key_len - 1) << 6) & KVBUF_KEY_SIZE_EMASK) | KVBUF_VTYPE_OBJECT_BEGIN;
    ++begin;
    
    if (begin + key_len > end) {return end;}
    set_v(begin, item->key, key_len);
    begin += key_len;
    if (begin >= end) {return end;}
    
    
    child = item->child;
    
	if (child)
    {
        begin = build_value(child, begin, end);
		while ((child = child->next) && begin < end) {begin = build_value(child, begin, end);}
	}
    
    if (begin >= end) {return end;}
    *begin = KVBUF_VTYPE_OBJECT_END;
    return begin + 1;
}


static unsigned char *build_object_in_array(kvbuf *item, unsigned char *begin, unsigned char *end)
{
	kvbuf *child;
	if (!item) {return end;}
	if (begin == end) {return end;}
    
    *begin = KVBUF_VTYPE_OBJECT_BEGIN;
    ++begin;
	if (begin == end) {return end;}
    
    child = item->child;
	if (child)
    {
        begin = build_value(child, begin, end);
		while ((child = child->next) && begin < end) {begin = build_value(child, begin, end);}
	}
    
    if (begin >= end) {return end;}
    *begin = KVBUF_VTYPE_OBJECT_END;
    return begin + 1;
}

static unsigned char *build_array_in_array(kvbuf *item, unsigned char *begin, unsigned char *end)
{
    kvbuf *child;
	unsigned int arr_len = 0;
    
    if (!item) {return end;}
	if (begin == end) {return end;}
    
    *begin = KVBUF_VTYPE_ARRAY;
    *begin |= (item->child->type & KVBUF_VTYPE_MASK);
    ++begin;
    
    if (begin >= end) {return end;}
    
    for (child = item->child; child; child = child->next)
    {
        ++arr_len;
    }
    
    begin = set_arr_len(begin, end, arr_len);
    if (begin >= end) {return end;}
    
    child = item->child;
	if (child)
    {
        if (child->type <= KV_BUF_NUMBER)
        {
            begin = build_number_in_array(child, begin, end);
            while ((child = child->next) && begin < end) {begin = build_number_in_array(child, begin, end);}
        }
        else if (child->type == KV_BUF_OBJECT)
        {
            begin = build_object_in_array(child, begin, end);
            while ((child = child->next) && begin < end) {begin = build_object_in_array(child, begin, end);}
        }
        else if (child->type == KV_BUF_ARRAY)
        {
            begin = build_array_in_array(child, begin, end);
            while ((child = child->next) && begin < end) {begin = build_array_in_array(child, begin, end);}
        }
        
    }
    
    if (begin >= end) {return end;}
    
    *begin = KVBUF_VTYPE_ARRAY | (item->child->type & KVBUF_VTYPE_MASK);
    return ++begin;
}

static unsigned char *build_array(kvbuf *item, unsigned char *begin, unsigned char *end)
{
	kvbuf *child;
	unsigned int arr_len = 0;
    unsigned int key_len;
    if (!item) {return end;}
	if (begin == end) {return end;}
    
    key_len = get_key_len(item->key);
    *begin = (((key_len - 1) << 6) & KVBUF_KEY_SIZE_EMASK) | KVBUF_VTYPE_ARRAY;
    
    if (item->child)
    {
        *begin |= (item->child->type & KVBUF_VTYPE_MASK);
    }
    
    ++begin;
    
    if (begin + key_len > end) {return end;}
    set_v(begin, item->key, key_len);
    begin += key_len;
    if (begin >= end) {return end;}
    
    for (child = item->child; child; child = child->next)
    {
        ++arr_len;
    }
    
    begin = set_arr_len(begin, end, arr_len);
    if (begin >= end) {return end;}
    
    child = item->child;
	if (child)
    {
        if (child->type <= KV_BUF_NUMBER)
        {
            begin = build_number_in_array(child, begin, end);
            while ((child = child->next) && begin < end) {begin = build_number_in_array(child, begin, end);}
        }
        else if (child->type == KV_BUF_OBJECT)
        {
            begin = build_object_in_array(child, begin, end);
            while ((child = child->next) && begin < end) {begin = build_object_in_array(child, begin, end);}
        }
        else if (child->type == KV_BUF_ARRAY)
        {
            begin = build_array_in_array(child, begin, end);
            while ((child = child->next) && begin < end) {begin = build_array_in_array(child, begin, end);}
        }
         
        if (begin >= end) {return end;}
        *begin = KVBUF_VTYPE_ARRAY | (item->child->type & KVBUF_VTYPE_MASK);
    }
    else
    {
        if (begin >= end) {return end;}
        *begin = KVBUF_VTYPE_ARRAY;
    }
    
    
    return ++begin;
}

static unsigned char *build_number(kvbuf *item, unsigned char *begin, unsigned char *end)
{
	if (!item) {return end;}
    if (begin == end) {return end;}
    
    unsigned int key_len = get_key_len(item->key);
    *begin = (((key_len - 1) << 6) & KVBUF_KEY_SIZE_EMASK) | item->type;
    ++begin;
    
    if (begin + key_len > end) {return end;}
    set_v(begin, item->key, key_len);
    begin += key_len;
    if (begin >= end) {return end;}
    
    switch (item->type)
    {
        case KV_BUF_INT8: {*begin = (unsigned char)item->number.i8; ++begin; break;}
        case KV_BUF_INT16: {if (begin + 2 > end) {return end;} set_uint16(begin, item->number.i16); begin += 2; break;}
        case KV_BUF_INT32: {if (begin + 4 > end) {return end;} set_uint32(begin, item->number.i32); begin += 4; break;}
        case KV_BUF_INT64: {if (begin + 8 > end) {return end;} set_uint64(begin, item->number.i64); begin += 8; break;}
        case KV_BUF_INT128: {if (begin + 16 > end) {return end;} /*set_uint128(begin, item->number.i128);*/ begin += 16; break;}
        
        case KV_BUF_UINT8: {*begin = item->number.u8; ++begin; break;}
        case KV_BUF_UINT16: {if (begin + 2 > end) {return end;} set_uint16(begin, item->number.u16); begin += 2; break;}
        case KV_BUF_UINT32: {if (begin + 4 > end) {return end;} set_uint32(begin, item->number.u32); begin += 4; break;}
        case KV_BUF_UINT64: {if (begin + 8 > end) {return end;} set_uint64(begin, item->number.u64); begin += 8; break;}
        case KV_BUF_UINT128: {if (begin + 16 > end) {return end;} /*set_uint64(begin, item->number.u64);*/ begin += 16; break;}
        
        case KV_BUF_FLOAT16: {if (begin + 2 > end) {return end;} /*set_float16(begin, item->number.f16);*/ begin += 2; break;}
        case KV_BUF_FLOAT32: {if (begin + 4 > end) {return end;} set_float32(begin, item->number.f32); begin += 4; break;}
        case KV_BUF_FLOAT64: {if (begin + 8 > end) {return end;} set_float64(begin, item->number.f64); begin += 8; break;}
        case KV_BUF_FLOAT128: {if (begin + 16 > end) {return end;} set_float128(begin, item->number.f128); begin += 16; break;}
        default:
        {
            break;
        }
    }
    return begin;
}



static unsigned char *build_number_in_array(kvbuf *item, unsigned char *begin, unsigned char *end)
{
	if (!item) {return end;}
    if (begin == end) {return end;}
    
    switch (item->type)
    {
        case KV_BUF_INT8: {*begin = (unsigned char)item->number.i8; ++begin; break;}
        case KV_BUF_INT16: {if (begin + 2 > end) {return end;} set_uint16(begin, item->number.i16); begin += 2; break;}
        case KV_BUF_INT32: {if (begin + 4 > end) {return end;} set_uint32(begin, item->number.i32); begin += 4; break;}
        case KV_BUF_INT64: {if (begin + 8 > end) {return end;} set_uint64(begin, item->number.i64); begin += 8; break;}
        case KV_BUF_INT128: {if (begin + 16 > end) {return end;} /*set_uint128(begin, item->number.i128);*/ begin += 16; break;}
        
        case KV_BUF_UINT8: {*begin = item->number.u8; ++begin; break;}
        case KV_BUF_UINT16: {if (begin + 2 > end) {return end;} set_uint16(begin, item->number.u16); begin += 2; break;}
        case KV_BUF_UINT32: {if (begin + 4 > end) {return end;} set_uint32(begin, item->number.u32); begin += 4; break;}
        case KV_BUF_UINT64: {if (begin + 8 > end) {return end;} set_uint64(begin, item->number.u64); begin += 8; break;}
        case KV_BUF_UINT128: {if (begin + 16 > end) {return end;} /*set_uint128(begin, item->number.u64);*/ begin += 16; break;}
        
        case KV_BUF_FLOAT16: {if (begin + 2 > end) {return end;} /*set_float16(begin, item->number.f16);*/ begin += 2; break;}
        case KV_BUF_FLOAT32: {if (begin + 4 > end) {return end;} set_float32(begin, item->number.f32); begin += 4; break;}
        case KV_BUF_FLOAT64: {if (begin + 8 > end) {return end;} set_float64(begin, item->number.f64); begin += 8; break;}
        case KV_BUF_FLOAT128: {if (begin + 16 > end) {return end;} set_float128(begin, item->number.f128); begin += 16; break;}
        default:
        {
            break;
        }
    }
    return begin;
}

static unsigned char *build_value(kvbuf *item, unsigned char *begin, unsigned char *end)
{
	if (!item) {return end;}
    if (item->type <= KV_BUF_NUMBER) {return build_number(item, begin, end);}
    if (item->type == KV_BUF_OBJECT) { return build_object(item, begin, end);}
    if (item->type == KV_BUF_ARRAY) {return build_array(item, begin, end);}
    return end;
}

unsigned int kvbuf_build(kvbuf_hooks *hooks, kvbuf *item, unsigned char *buf, unsigned int size)
{
    const unsigned char *r = build_value(item, buf, buf + size);
    if (r == buf + size)
    {
        return 0;
    }
    return r - buf;
}






char *kvbuf_print(kvbuf_hooks *hooks, kvbuf *item, int fmt)
{
	kvbuf_buffer p;
	if (!(p.buffer = (char *)hooks->malloc(512))) {return 0;}
	p.size = 512; p.offset = 0;
	
	print_value(hooks, item, &p, 0, fmt, 1);
	buffer_write_zero(hooks, &p);
	return p.buffer;
}



static void print_value(kvbuf_hooks *hooks, kvbuf *item, kvbuf_buffer *p, int depth, int fmt, int print_key)
{
	if (!item) {return;}
    if (item->type == KV_BUF_OBJECT) {print_object(hooks, item, p, depth, fmt, print_key); return;}
    if (item->type == KV_BUF_ARRAY) {print_array(hooks, item, p, depth, fmt, print_key); return;}
    if (item->type <= KV_BUF_NUMBER) {print_number(hooks, item, p, depth, fmt, print_key); return;}
}

static void print_object(kvbuf_hooks *hooks, kvbuf *item, kvbuf_buffer *p, int depth, int fmt, int print_key)
{
	kvbuf *child;
	if (!item) {return;}
	
	child = item->child;
	
	if (fmt) { buffer_write_table(hooks, p, depth); }
    if (print_key) { buffer_write_uint32(hooks, p, item->key); buffer_write_string(hooks, p, ":"); }
    if (fmt) { buffer_write_string(hooks, p, " "); }
    
	buffer_write_string(hooks, p, "{"); if (fmt) {buffer_write_string(hooks, p, "\n");}
	if (child)
    {
		print_value(hooks, child, p, depth + 1, fmt, 1);
		while ((child = child->next)) {buffer_write_string(hooks, p, ","); if (fmt) {buffer_write_string(hooks, p, "\n");} print_value(hooks, child, p, depth + 1, fmt, 1);}
	}
	if (fmt) {buffer_write_string(hooks, p, "\n"); buffer_write_table(hooks, p, depth);} buffer_write_string(hooks, p, "}");
	
}

static void print_array(kvbuf_hooks *hooks, kvbuf *item, kvbuf_buffer *p, int depth, int fmt, int print_key)
{
	kvbuf *child;
	if (!item) {return;}
	
	child = item->child;
	if (fmt) { buffer_write_table(hooks, p, depth); }
    if (print_key) { buffer_write_uint32(hooks, p, item->key); buffer_write_string(hooks, p, ":"); }
    if (fmt) { buffer_write_string(hooks, p, " "); }
	buffer_write_string(hooks, p, "["); if (fmt) {buffer_write_string(hooks, p, "\n");}
	if (child)
    {
		print_value(hooks, child, p, depth + 1, fmt, 0);
		while ((child = child->next)) {buffer_write_string(hooks, p, ","); if (fmt) {buffer_write_string(hooks, p, "\n");} print_value(hooks, child, p, depth + 1, fmt, 0);}
	}
	if (fmt) {buffer_write_string(hooks, p, "\n"); buffer_write_table(hooks, p, depth);} buffer_write_string(hooks, p, "]");
}

static void print_number(kvbuf_hooks *hooks, kvbuf *item, kvbuf_buffer *p, int depth, int fmt, int print_key)
{
	if (!item) { return; }
	if (fmt) { buffer_write_table(hooks, p, depth); }
    if (print_key) { buffer_write_uint32(hooks, p, item->key); buffer_write_string(hooks, p, ":"); }
    if (fmt) { buffer_write_string(hooks, p, " "); }
    
    switch (item->type)
    {
        case KV_BUF_INT8: {buffer_write_int32(hooks, p, item->number.i8); break;}
        case KV_BUF_INT16: {buffer_write_int32(hooks, p, item->number.i16); break;}
        case KV_BUF_INT32: {buffer_write_int32(hooks, p, item->number.i32); break;}
        case KV_BUF_INT64: {buffer_write_int64(hooks, p, item->number.i64); break;}
        case KV_BUF_UINT8: {buffer_write_uint32(hooks, p, item->number.u8); break;}
        case KV_BUF_UINT16: {buffer_write_uint32(hooks, p, item->number.u16); break;}
        case KV_BUF_UINT32: {buffer_write_uint32(hooks, p, item->number.u32); break;}
        case KV_BUF_UINT64: {buffer_write_uint64(hooks, p, item->number.u64); break;}
        case KV_BUF_FLOAT32: {buffer_write_float32(hooks, p, item->number.f32); break;}
        case KV_BUF_FLOAT64: {buffer_write_float64(hooks, p, item->number.f64); break;}
        case KV_BUF_FLOAT128: {buffer_write_float128(hooks, p, item->number.f128); break;}
        default:
        {
            break;
        }
    }
}



kvbuf *kvbuf_create_object(kvbuf_hooks *hooks)
{
	kvbuf *child = kvbuf_new_item(hooks);
	if (!child) {return 0;}
	child->type = KV_BUF_OBJECT;
	return child;
}



kvbuf *kvbuf_create_int8(kvbuf_hooks *hooks, char number)
{
	kvbuf *child = kvbuf_new_item(hooks);
	if (!child) {return 0;}
	child->type = KV_BUF_INT8;
    child->number.i8 = number;
	return child;
}


kvbuf *kvbuf_create_int16(kvbuf_hooks *hooks, short number)
{
	kvbuf *child = kvbuf_new_item(hooks);
	if (!child) {return 0;}
	child->type = KV_BUF_INT16;
    child->number.i16 = number;
	return child;
}

kvbuf *kvbuf_create_int32(kvbuf_hooks *hooks, int number)
{
	kvbuf *child = kvbuf_new_item(hooks);
	if (!child) {return 0;}
	child->type = KV_BUF_INT32;
    child->number.i32 = number;
	return child;
}

kvbuf *kvbuf_create_int64(kvbuf_hooks *hooks, long long number)
{
	kvbuf *child = kvbuf_new_item(hooks);
	if (!child) {return 0;}
	child->type = KV_BUF_INT64;
    child->number.i64 = number;
	return child;
}



kvbuf *kvbuf_create_uint8(kvbuf_hooks *hooks, unsigned char number)
{
	kvbuf *child = kvbuf_new_item(hooks);
	if (!child) {return 0;}
	child->type = KV_BUF_UINT8;
    child->number.u8 = number;
	return child;
}


kvbuf *kvbuf_create_uint16(kvbuf_hooks *hooks, unsigned short number)
{
	kvbuf *child = kvbuf_new_item(hooks);
	if (!child) {return 0;}
	child->type = KV_BUF_UINT16;
    child->number.u16 = number;
	return child;
}

kvbuf *kvbuf_create_uint32(kvbuf_hooks *hooks, unsigned int number)
{
	kvbuf *child = kvbuf_new_item(hooks);
	if (!child) {return 0;}
	child->type = KV_BUF_UINT32;
    child->number.u32 = number;
	return child;
}

kvbuf *kvbuf_create_uint64(kvbuf_hooks *hooks, unsigned long long number)
{
	kvbuf *child = kvbuf_new_item(hooks);
	if (!child) {return 0;}
	child->type = KV_BUF_UINT64;
    child->number.u64 = number;
	return child;
}

kvbuf *kvbuf_create_float32(kvbuf_hooks *hooks, float number)
{
    kvbuf *child = kvbuf_new_item(hooks);
    if (!child) {return 0;}
    child->type = KV_BUF_FLOAT32;
    child->number.f32 = number;
    return child;
}

kvbuf *kvbuf_create_float64(kvbuf_hooks *hooks, double number)
{
	kvbuf *child = kvbuf_new_item(hooks);
	if (!child) {return 0;}
	child->type = KV_BUF_FLOAT64;
    child->number.f64 = number;
	return child;
}

kvbuf *kvbuf_create_float128(kvbuf_hooks *hooks, long double number)
{
	kvbuf *child = kvbuf_new_item(hooks);
	if (!child) {return 0;}
	child->type = KV_BUF_FLOAT128;
    child->number.f128 = number;
	return child;
}



kvbuf *kvbuf_create_array(kvbuf_hooks *hooks)
{
	kvbuf *child = kvbuf_new_item(hooks);
	if (!child) {return 0;}
	child->type = KV_BUF_ARRAY;
	return child;
}




void kvbuf_add_item_to_array(kvbuf *array, kvbuf *item)
{
    kvbuf *child = array->child;
    if (!item) {return;}
    if (!child) {array->child = item; return;}
    if (child && child->prev) { child = child->prev; }
    child->next = item; item->prev = child; array->child->prev = item;
}

void kvbuf_add_item_to_object(kvbuf *object, unsigned int key, kvbuf *item)
{
	if (!item) {return;}
    item->key = key;
	kvbuf_add_item_to_array(object, item);
}


kvbuf *kvbuf_get_object_item(kvbuf *object, unsigned int key)
{
	kvbuf *c = object->child;
	while (c) {if (key == c->key) {break;} c = c->next;}
	if (c) {return c;}
	return 0;
}


kvbuf *kvbuf_get_array_item(kvbuf *array, int item)
{
	kvbuf *c = array->child;
	while (c && item > 0) {c = c->next; item--;}
	if (c) {return c;}
	return 0;
}

kvbuf *kvbuf_get_array_begin(kvbuf *array)
{
    return array->child;
}

kvbuf *kvbuf_get_array_next(kvbuf *it)
{
    return it->next;
}

kvbuf *kvbuf_get_array_end(kvbuf *array)
{
    return 0;
}

int kvbuf_get_array_size(kvbuf *array)
{
	int size = 0; kvbuf *c = array->child;
	while (c) {c = c->next; size++;}
	return size;
}






static int buffer_resize(kvbuf_hooks *hooks, kvbuf_buffer *p, int need_size)
{
	if (p->offset + need_size > p->size)
    {
		int new_size = max(p->offset + need_size, p->size * 2);
		char *buf = (char *)hooks->malloc(new_size);
		if (!buf) {return -1;}
		memcpy(buf, p->buffer, p->offset); hooks->free(p->buffer);
		p->buffer = buf; p->size = new_size;
	}
	return 0;
}


static void buffer_write_int64(kvbuf_hooks *hooks, kvbuf_buffer *p, long long number)
{
	int len = 10;
	if (buffer_resize(hooks, p, len)) {return;}
    sprintf(p->buffer + p->offset, "%lld", number);
	while (p->buffer[p->offset]) {p->offset++;}
}


static void buffer_write_uint64(kvbuf_hooks *hooks, kvbuf_buffer *p, unsigned long long number)
{
	int len = 10;
	if (buffer_resize(hooks, p, len)) {return;}
    sprintf(p->buffer + p->offset, "%llu", number);
	while (p->buffer[p->offset]) {p->offset++;}
}

static void buffer_write_int32(kvbuf_hooks *hooks, kvbuf_buffer *p, int number)
{
	int len = 10;
	if (buffer_resize(hooks, p, len)) {return;}
    sprintf(p->buffer + p->offset, "%d", number);
	while (p->buffer[p->offset]) {p->offset++;}
}


static void buffer_write_uint32(kvbuf_hooks *hooks, kvbuf_buffer *p, unsigned int number)
{
	int len = 10;
	if (buffer_resize(hooks, p, len)) {return;}
    sprintf(p->buffer + p->offset, "%u", number);
	while (p->buffer[p->offset]) {p->offset++;}
}

static void buffer_write_float32(kvbuf_hooks *hooks, kvbuf_buffer *p, float number)
{
    int len = 20;
    if (buffer_resize(hooks, p, len)) {return;}
    sprintf(p->buffer + p->offset, "%f", number);
    while (p->buffer[p->offset]) {p->offset++;}
}


static void buffer_write_float64(kvbuf_hooks *hooks, kvbuf_buffer *p, double number)
{
	int len = 20;
	if (buffer_resize(hooks, p, len)) {return;}
    sprintf(p->buffer + p->offset, "%f", number);
	while (p->buffer[p->offset]) {p->offset++;}    
}

static void buffer_write_float128(kvbuf_hooks *hooks, kvbuf_buffer *p, long double number)
{
	int len = 20;
	if (buffer_resize(hooks, p, len)) {return;}
    sprintf(p->buffer + p->offset, "%Lf", number);
	while (p->buffer[p->offset]) {p->offset++;}    
}

static void buffer_write_table(kvbuf_hooks *hooks, kvbuf_buffer *p, int n)
{
	while (n--) {buffer_write_string(hooks, p, "    ");}
}

static void buffer_write_zero(kvbuf_hooks *hooks, kvbuf_buffer *p)
{
	if (buffer_resize(hooks, p, 1)) {return;}
	p->buffer[p->offset] = 0;
	p->offset++;
}


static void buffer_write_string(kvbuf_hooks *hooks, kvbuf_buffer *p, const char *string)
{
	int len = strlen(string);
	if (buffer_resize(hooks, p, len)) {return;}
	memcpy(p->buffer + p->offset, string, len);
	p->offset += len;
}
