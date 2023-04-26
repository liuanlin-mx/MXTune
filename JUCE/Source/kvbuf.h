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

#ifndef __MTOOL_KV_BUF_H__
#define __MTOOL_KV_BUF_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define KVBUF_KEY_SIZE_1BYTE (0)
#define KVBUF_KEY_SIZE_2BYTE (0x01 << 6)
#define KVBUF_KEY_SIZE_3BYTE (0x02 << 6)
#define KVBUF_KEY_SIZE_4BYTE (0x03 << 6)
#define KVBUF_KEY_SIZE_EMASK (0xc0)

#define KVBUF_VTYPE_INT8 (0)
#define KVBUF_VTYPE_INT16 (1)
#define KVBUF_VTYPE_INT32 (2)
#define KVBUF_VTYPE_INT64 (3)
#define KVBUF_VTYPE_INT128 (4)
#define KVBUF_VTYPE_UINT8 (5)
#define KVBUF_VTYPE_UINT16 (6)
#define KVBUF_VTYPE_UINT32 (7)
#define KVBUF_VTYPE_UINT64 (8)
#define KVBUF_VTYPE_UINT128 (9)
#define KVBUF_VTYPE_FLOAT16 (10)
#define KVBUF_VTYPE_FLOAT32 (11)
#define KVBUF_VTYPE_FLOAT64 (12)
#define KVBUF_VTYPE_FLOAT128 (13)
#define KVBUF_VTYPE_NUMBER (13)


#define KVBUF_VTYPE_OBJECT_BEGIN (14)
#define KVBUF_VTYPE_OBJECT_END (15)
#define KVBUF_VTYPE_NEST_ARRAY (16)

#define KVBUF_VTYPE_EXT (0x1f)

#define KVBUF_VTYPE_MASK (0x1f)
#define KVBUF_VTYPE_ARRAY (0x20)


/* 数字类型 */
/* 头(1字节) + 扩展头(1字节 可选) + key(1-4字节 可变) + 元素 */

/* 对象类型 */
/* 头(1字节) + 扩展头(1字节 可选) + key(1-4字节 可变) + 元素 + 对象结束标志(1字节) */
/* 对象存放在数组中时没有 key字段 */

/* 数组 */
/* 头(1字节) + 扩展头(1字节 可选) + key(1-4字节 可变) + 数组长度(可变长度) + 元素0 元素1...元素n + 数组结束标志 (1字节)*/
/* 数组长度 当bit7为1时表示需要扩展1字节 依次递推, 长度的低7bit在第一个字节 */


/* 类型头 */
/* bit0-bit4 元素类型 */
/* bit5 1: 表示是数组，0: 普通元素或对象 */
/* bit0-bit4为KVBUF_VTYPE_EXT 表示有扩展头，扩展头大小1字节紧跟头部 描述类型大小 单位是字节 0: 没有扩展头 */
/* 有扩展头时元素类型字段含义跟无扩展头时不同 无扩展头时元素类型已经包含了元素大小 没必要用扩展头描述长度信息了*/
/* 扩展头只是为了以后扩展可能用到的字段 估计不会用 */
/* bit6-bit7 表示key 占用的字节数 范围1-4字节 */

enum
{
    KV_BUF_INT8 = 0,
    KV_BUF_INT16,
    KV_BUF_INT32,
    KV_BUF_INT64,
    KV_BUF_INT128,
    KV_BUF_UINT8,
    KV_BUF_UINT16,
    KV_BUF_UINT32,
    KV_BUF_UINT64,
    KV_BUF_UINT128,
    KV_BUF_FLOAT16,
    KV_BUF_FLOAT32,
    KV_BUF_FLOAT64,
    KV_BUF_FLOAT128,
    KV_BUF_NUMBER = KVBUF_VTYPE_NUMBER,
	KV_BUF_OBJECT = KVBUF_VTYPE_OBJECT_BEGIN,
    KV_BUF_ARRAY = KVBUF_VTYPE_NEST_ARRAY,
    KV_BUF_EXT = KVBUF_VTYPE_EXT,
    KV_BUF_UNKNOWN,
};

typedef struct kvbuf
{
	struct kvbuf *next;
    struct kvbuf *prev;
    struct kvbuf *child;
	int type;
    unsigned int key;
    union
    {
        signed char i8;
        signed short i16;
        signed int i32;
        signed long long i64;
        unsigned char u8;
        unsigned short u16;
        unsigned int u32;
        unsigned long long u64;
        float f32;
        double f64;
        long double f128;
    }number;
    
}kvbuf;

typedef struct kvbuf_hooks
{
    void* (*malloc)(size_t sz);
    void (*free)(void* ptr);
}kvbuf_hooks;



void kvbuf_delete(kvbuf_hooks *hooks, kvbuf *item);
void kvbuf_free(kvbuf_hooks *hooks, void *p);
kvbuf *kvbuf_parse(kvbuf_hooks *hooks, const unsigned char *begin, const unsigned char *end);
char *kvbuf_print(kvbuf_hooks *hooks, kvbuf *item, int fmt);
unsigned int kvbuf_build(kvbuf_hooks *hooks, kvbuf *item, unsigned char *buf, unsigned int size);


kvbuf *kvbuf_create_object(kvbuf_hooks *hooks);
kvbuf *kvbuf_create_int8(kvbuf_hooks *hooks, char number);
kvbuf *kvbuf_create_int16(kvbuf_hooks *hooks, short number);
kvbuf *kvbuf_create_int32(kvbuf_hooks *hooks, int number);
kvbuf *kvbuf_create_int64(kvbuf_hooks *hooks, long long number);
kvbuf *kvbuf_create_uint8(kvbuf_hooks *hooks, unsigned char number);
kvbuf *kvbuf_create_uint16(kvbuf_hooks *hooks, unsigned short number);
kvbuf *kvbuf_create_uint32(kvbuf_hooks *hooks, unsigned int number);
kvbuf *kvbuf_create_uint64(kvbuf_hooks *hooks, unsigned long long number);
kvbuf *kvbuf_create_float32(kvbuf_hooks *hooks, float number);
kvbuf *kvbuf_create_float64(kvbuf_hooks *hooks, double number);
kvbuf *kvbuf_create_float128(kvbuf_hooks *hooks, long double number);

kvbuf *kvbuf_create_array(kvbuf_hooks *hooks);

void kvbuf_add_item_to_array(kvbuf *array, kvbuf *item);
void kvbuf_add_item_to_object(kvbuf *object, unsigned int key, kvbuf *item);

kvbuf *kvbuf_get_object_item(kvbuf *object, unsigned int key);
kvbuf *kvbuf_get_array_item(kvbuf *array, int item);
kvbuf *kvbuf_get_array_begin(kvbuf *array);
kvbuf *kvbuf_get_array_next(kvbuf *it);
kvbuf *kvbuf_get_array_end(kvbuf *array);

int kvbuf_get_array_size(kvbuf *array);


inline signed char kvbuf_get_int8(kvbuf *item) { return item->number.i8; }
inline signed short kvbuf_get_int16(kvbuf *item) { return item->number.i16; }
inline signed int kvbuf_get_int32(kvbuf *item) { return item->number.i32; }
inline signed long long kvbuf_get_int64(kvbuf *item) { return item->number.i64; }

inline unsigned char kvbuf_get_uint8(kvbuf *item) { return item->number.u8; }
inline unsigned short kvbuf_get_uint16(kvbuf *item) { return item->number.u16; }
inline unsigned int kvbuf_get_uint32(kvbuf *item) { return item->number.u32; }
inline unsigned long long kvbuf_get_uint64(kvbuf *item) { return item->number.u64; }

inline float kvbuf_get_float32(kvbuf *item) { return item->number.f32; }
inline double kvbuf_get_float64(kvbuf *item) { return item->number.f64; }





inline int kvbuf_is_array(kvbuf *item) { return item->type == KV_BUF_ARRAY; }
inline int kvbuf_is_object(kvbuf *item) { return item->type == KV_BUF_OBJECT; }
inline int kvbuf_is_number(kvbuf *item) { return (item->type <= KV_BUF_FLOAT64); }

inline int kvbuf_is_int8(kvbuf *item) { return (item->type == KV_BUF_INT8); }
inline int kvbuf_is_int16(kvbuf *item) { return (item->type == KV_BUF_INT16); }
inline int kvbuf_is_int32(kvbuf *item) { return (item->type == KV_BUF_INT32); }
inline int kvbuf_is_int64(kvbuf *item) { return (item->type == KV_BUF_INT64); }

inline int kvbuf_is_uint8(kvbuf *item) { return (item->type == KV_BUF_UINT8); }
inline int kvbuf_is_uint16(kvbuf *item) { return (item->type == KV_BUF_UINT16); }
inline int kvbuf_is_uint32(kvbuf *item) { return (item->type == KV_BUF_UINT32); }
inline int kvbuf_is_uint64(kvbuf *item) { return (item->type == KV_BUF_UINT64); }

inline int kvbuf_is_float32(kvbuf *item) { return (item->type == KV_BUF_FLOAT32); }
inline int kvbuf_is_float64(kvbuf *item) { return (item->type == KV_BUF_FLOAT64); }

#ifdef __cplusplus
}
#endif

#endif