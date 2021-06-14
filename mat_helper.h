/*****************************************************************************
*                                                                            *
*  Copyright (C) 2020, Liu An Lin.                                           *
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

#ifndef __MAT_HELPER_H__
#define __MAT_HELPER_H__
#include <stdio.h>
#include <string.h>


#if (defined(_WIN32) || defined(_WIN64) || defined(__MINGW32__) || defined(__MINGW64__))
	#define MAT_STORAGE_OS_WINDOWS
#else
	#define MAT_STORAGE_OS_LINUX
#endif

#ifdef MAT_HELPER_USE_OPENCV
#include <opencv2/opencv.hpp>
#endif

#ifdef MAT_STORAGE_OS_LINUX
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>


typedef int mat_helper_socket_t;
typedef socklen_t mat_helper_socklen_t;
#define MAT_HELPER_INVALID_SOCKET (-1)

static inline void mat_helper_close_socket(mat_helper_socket_t s)
{
    shutdown(s, 2);
    close(s);
}
        
#else
#include <windows.h>
typedef int mat_helper_socklen_t;
typedef SOCKET mat_helper_socket_t;
#define MAT_HELPER_INVALID_SOCKET INVALID_SOCKET

static inline void mat_helper_close_socket(mat_helper_socket_t s)
{
    shutdown(s, 2);
    closesocket(s);
}
#endif


#define MAT_HELPER_PORT (18597)

enum
{
    MAT_HELPER_INT8 = 0,
    MAT_HELPER_UINT8,
    MAT_HELPER_INT16,
    MAT_HELPER_UINT16,
    MAT_HELPER_INT32,
    MAT_HELPER_UINT32,
    MAT_HELPER_INT64,
    MAT_HELPER_UINT64,
    MAT_HELPER_FLOAT32,
    MAT_HELPER_FLOAT64,
};


#pragma pack(1)
enum
{
    MAT_HELPER_TYPE_WRITE_REQ,
    MAT_HELPER_TYPE_WRITE_RES,
    MAT_HELPER_TYPE_READ_REQ,
    MAT_HELPER_TYPE_READ_RES,
    MAT_HELPER_TYPE_READ_INFO_REQ,
    MAT_HELPER_TYPE_READ_INFO_RES,
    MAT_HELPER_TYPE_LIST_REQ,
    MAT_HELPER_TYPE_LIST_RES,
    MAT_HELPER_TYPE_DEL_REQ,
    MAT_HELPER_TYPE_DEL_RES,
    MAT_HELPER_TYPE_SEM_INIT_REQ,
    MAT_HELPER_TYPE_SEM_INIT_RES,
    MAT_HELPER_TYPE_SEM_DESTROY_REQ,
    MAT_HELPER_TYPE_SEM_DESTROY_RES,
    MAT_HELPER_TYPE_SEM_WAIT_REQ,
    MAT_HELPER_TYPE_SEM_WAIT_RES,
    MAT_HELPER_TYPE_SEM_POST_REQ,
    MAT_HELPER_TYPE_SEM_POST_RES,
};

enum
{
    MAT_HELPER_OK,
    MAT_HELPER_ERR
};

struct mat_helper_mat_info
{
    unsigned char type;
    unsigned char dims;
    int dim_size[8];
};

struct mat_helper_write_req
{
    unsigned char type;
    char name[64];
    struct mat_helper_mat_info info;
};


struct mat_helper_write_res
{
    unsigned char type;
    unsigned char result;
};

struct mat_helper_read_req
{
    unsigned char type;
    char del;
    char name[64];
};

struct mat_helper_read_res
{
    unsigned char type;
    unsigned char result;
    struct mat_helper_mat_info info;
};

struct mat_helper_list_req
{
    unsigned char type;
};

struct mat_helper_list_res
{
    unsigned char type;
    int len;
};


struct mat_helper_del_req
{
    unsigned char type;
    char name[64];
};

struct mat_helper_del_res
{
    unsigned char type;
    unsigned char result;
};


struct mat_helper_sem_init_req
{
    unsigned char type;
    unsigned char max;
    char name[64];
};
struct mat_helper_sem_init_res
{
    unsigned char type;
    unsigned char result;
};

struct mat_helper_sem_destroy_req
{
    unsigned char type;
    char name[64];
};
struct mat_helper_sem_destroy_res
{
    unsigned char type;
    unsigned char result;
};

struct mat_helper_sem_post_req
{
    unsigned char type;
    char name[64];
};
struct mat_helper_sem_post_res
{
    unsigned char type;
    unsigned char result;
};

struct mat_helper_sem_wait_req
{
    unsigned char type;
    char name[64];
};
struct mat_helper_sem_wait_res
{
    unsigned char type;
    unsigned char result;
};
#pragma pack()


static inline int __mat_helper_get_type_size(int type)
{
    switch (type)
    {
        case MAT_HELPER_INT8:
        case MAT_HELPER_UINT8:
            return 1;
        case MAT_HELPER_INT16:
        case MAT_HELPER_UINT16:
            return 2;
        case MAT_HELPER_INT32:
        case MAT_HELPER_UINT32:
            return 4;
        case MAT_HELPER_INT64:
        case MAT_HELPER_UINT64:
            return 8;
        case MAT_HELPER_FLOAT32:
            return 4;
        case MAT_HELPER_FLOAT64:
            return 8;
        default:
            return 1;
    }
    return 1;
}


static inline int mat_helper_str_to_type(const char *type)
{
    if (strcasecmp(type, "int8") == 0)
    {
        return MAT_HELPER_INT8;
    }
    else if (strcasecmp(type, "uint8") == 0)
    {
        return MAT_HELPER_UINT8;
    }
    else if (strcasecmp(type, "int16") == 0)
    {
        return MAT_HELPER_UINT16;
    }
    else if (strcasecmp(type, "uint16") == 0)
    {
        return MAT_HELPER_UINT16;
    }
    else if (strcasecmp(type, "int32") == 0)
    {
        return MAT_HELPER_INT32;
    }
    else if (strcasecmp(type, "uint32") == 0)
    {
        return MAT_HELPER_UINT32;
    }
    else if (strcasecmp(type, "int64") == 0)
    {
        return MAT_HELPER_INT64;
    }
    else if (strcasecmp(type, "uint64") == 0)
    {
        return MAT_HELPER_UINT64;
    }
    else if (strcasecmp(type, "float32") == 0 || strcasecmp(type, "float") == 0)
    {
        return MAT_HELPER_FLOAT32;
    }
    else if (strcasecmp(type, "float64") == 0 || strcasecmp(type, "double") == 0)
    {
        return MAT_HELPER_FLOAT64;
    }
    return MAT_HELPER_UINT8;
}


static const char *mat_helper_type_to_str(int type)
{
    switch (type)
    {
        case MAT_HELPER_INT8:
            return "int8";
        case MAT_HELPER_UINT8:
            return "uint8";
        case MAT_HELPER_INT16:
            return "int16";
        case MAT_HELPER_UINT16:
            return "uint16";
        case MAT_HELPER_INT32:
            return "int32";
        case MAT_HELPER_UINT32:
            return "uint32";
        case MAT_HELPER_INT64:
            return "int64";
        case MAT_HELPER_UINT64:
            return "uint64";
        case MAT_HELPER_FLOAT32:
            return "float32";
        case MAT_HELPER_FLOAT64:
            return "float64";
        default:
            return "int8";
    }
    return "int8";
}

static inline int mat_helper_getsize(int dims, const int *dim_size, int type)
{
    int size = 1;
    for (int i = 0; i < dims; ++i)
    {
        size *= dim_size[i];
    }
    
    return size * __mat_helper_get_type_size(type);
}

static inline int mat_helper_read(mat_helper_socket_t s, char *buf, int size)
{
    int len = 0;
    while (len < size)
    {
        int r = recv(s, buf + len, size - len, 0);
        if (r <= 0)
        {
            return -1;
        }
        len += r;
    }
    return len;
}

static inline int mat_helper_write(mat_helper_socket_t s, char *data, int data_len)
{
    int len = 0;
    while (len < data_len)
    {
        int r = send(s, data + len, data_len - len, 0);
        if (r <= 0)
        {
            return -1;
        }
        len += r;
    }
    return len;
}


static inline int mat_helper_write_mat(const char *ip, const char *name, int dims, int *dim_size, int type, char *data)
{
    mat_helper_socket_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == MAT_HELPER_INVALID_SOCKET)
    {
        return -1;
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MAT_HELPER_PORT);
    addr.sin_addr.s_addr = inet_addr(ip);
    
    if (0 != connect(sock, (struct sockaddr *)&addr, sizeof(addr)))
    {
        mat_helper_close_socket(sock);
        return -1;
    }
            
    struct mat_helper_write_req req;
    memset(&req, 0, sizeof(req));
    req.type = MAT_HELPER_TYPE_WRITE_REQ;
    strcpy(req.name, name);
    req.info.dims = dims;
    for (int i = 0; i < dims; ++i)
    {
        req.info.dim_size[i] = htonl(dim_size[i]);
    }
    req.info.type = type;
    
    if (mat_helper_write(sock, (char *)&req, sizeof(req)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    if (mat_helper_write(sock, (char *)data, mat_helper_getsize(dims, dim_size, type)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    struct mat_helper_write_res res;
    if (mat_helper_read(sock, (char *)&res, sizeof(res)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    mat_helper_close_socket(sock);
    if (res.type != MAT_HELPER_TYPE_WRITE_RES || res.result != MAT_HELPER_OK)
    {
        return -1;
    }
    return 0;
}



static inline int mat_helper_write_mat2_1(const char *ip, const char *name, int rows, int cols, int ch, const char *type, void *data)
{
    int dim[3] = {rows, cols, ch};
    return mat_helper_write_mat(ip, name, 3, dim, mat_helper_str_to_type(type), (char *)data);
}

static inline int mat_helper_write_mat2_2(const char *ip, const char *name, int rows, int cols, const char *type, void *data)
{
    int dim[2] = {rows, cols};
    return mat_helper_write_mat(ip, name, 2, dim, mat_helper_str_to_type(type), (char *)data);
}


static inline int mat_helper_write_mat2_3(const char *name, int rows, int cols, int ch, const char *type, void *data)
{
    int dim[3] = {rows, cols, ch};
    return mat_helper_write_mat("127.0.0.1", name, 3, dim, mat_helper_str_to_type(type), (char *)data);
}

static inline int mat_helper_write_mat2_4(const char *name, int rows, int cols, const char *type, void *data)
{
    int dim[2] = {rows, cols};
    return mat_helper_write_mat("127.0.0.1", name, 2, dim, mat_helper_str_to_type(type), (char *)data);
}



static inline int mat_helper_read_mat_info(const char *ip, const char *name, int *dims, int *dim_size, int *type)
{
    mat_helper_socket_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == MAT_HELPER_INVALID_SOCKET)
    {
        return -1;
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MAT_HELPER_PORT);
    addr.sin_addr.s_addr = inet_addr(ip);
    
    if (0 != connect(sock, (struct sockaddr *)&addr, sizeof(addr)))
    {
        mat_helper_close_socket(sock);
        return -1;
    }
            
    struct mat_helper_read_req req;
    memset(&req, 0, sizeof(req));
    req.type = MAT_HELPER_TYPE_READ_INFO_REQ;
    strcpy(req.name, name);
    if (mat_helper_write(sock, (char *)&req, sizeof(req)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    
    struct mat_helper_read_res res;
    if (mat_helper_read(sock, (char *)&res, sizeof(res)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    if (res.type != MAT_HELPER_TYPE_READ_INFO_RES || res.result != MAT_HELPER_OK)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    *dims = res.info.dims;
    for (int i = 0; i < res.info.dims; ++i)
    {
        res.info.dim_size[i] = ntohl(res.info.dim_size[i]);
        dim_size[i] = res.info.dim_size[i];
    }
    *type = res.info.type;
    mat_helper_close_socket(sock);
    
    return 0;
}


static inline mat_helper_socket_t mat_helper_read_mat_begin(const char *ip, const char *name, struct mat_helper_mat_info *info, int del)
{
    mat_helper_socket_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == MAT_HELPER_INVALID_SOCKET)
    {
        return sock;
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MAT_HELPER_PORT);
    addr.sin_addr.s_addr = inet_addr(ip);
    
    if (0 != connect(sock, (struct sockaddr *)&addr, sizeof(addr)))
    {
        mat_helper_close_socket(sock);
        return MAT_HELPER_INVALID_SOCKET;
    }
            
    struct mat_helper_read_req req;
    memset(&req, 0, sizeof(req));
    req.type = MAT_HELPER_TYPE_READ_REQ;
    strcpy(req.name, name);
    req.del = del? 1: 0;
    if (mat_helper_write(sock, (char *)&req, sizeof(req)) < 0)
    {
        mat_helper_close_socket(sock);
        return MAT_HELPER_INVALID_SOCKET;
    }
    
    
    struct mat_helper_read_res res;
    if (mat_helper_read(sock, (char *)&res, sizeof(res)) < 0)
    {
        mat_helper_close_socket(sock);
        return MAT_HELPER_INVALID_SOCKET;
    }
    
    if (res.type != MAT_HELPER_TYPE_READ_RES || res.result != MAT_HELPER_OK)
    {
        mat_helper_close_socket(sock);
        return MAT_HELPER_INVALID_SOCKET;
    }
    
    for (int i = 0; i < res.info.dims; ++i)
    {
        res.info.dim_size[i] = ntohl(res.info.dim_size[i]);
    }
    *info = res.info;
    
    return sock;
}


static inline int mat_helper_read_mat_end(mat_helper_socket_t sock, const struct mat_helper_mat_info *info, char *buf, int buf_size)
{
    int mat_size = mat_helper_getsize(info->dims, info->dim_size, info->type);
    if (mat_size > buf_size)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    if (mat_helper_read(sock, (char *)buf, mat_size) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    mat_helper_close_socket(sock);
    
    return 0;
}

static inline int __mat_helper_read_mat(const char *ip, const char *name, int *dims, int *dim_size, int *type, char *buf, int buf_size, int del)
{
    mat_helper_socket_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == MAT_HELPER_INVALID_SOCKET)
    {
        return -1;
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MAT_HELPER_PORT);
    addr.sin_addr.s_addr = inet_addr(ip);
    
    if (0 != connect(sock, (struct sockaddr *)&addr, sizeof(addr)))
    {
        mat_helper_close_socket(sock);
        return -1;
    }
            
    struct mat_helper_read_req req;
    memset(&req, 0, sizeof(req));
    req.type = MAT_HELPER_TYPE_READ_REQ;
    strcpy(req.name, name);
    req.del = del? 1: 0;
    if (mat_helper_write(sock, (char *)&req, sizeof(req)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    
    struct mat_helper_read_res res;
    if (mat_helper_read(sock, (char *)&res, sizeof(res)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    if (res.type != MAT_HELPER_TYPE_READ_RES || res.result != MAT_HELPER_OK)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    for (int i = 0; i < res.info.dims; ++i)
    {
        res.info.dim_size[i] = ntohl(res.info.dim_size[i]);
        dim_size[i] = res.info.dim_size[i];
    }
    
    int mat_size = mat_helper_getsize(res.info.dims, res.info.dim_size, res.info.type);
    if (mat_size > buf_size)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    if (mat_helper_read(sock, (char *)buf, mat_size) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    *dims = res.info.dims;
    *type = res.info.type;
    for (int i = 0; i < res.info.dims; ++i)
    {
        dim_size[i] = res.info.dim_size[i];
    }
    mat_helper_close_socket(sock);
    
    return 0;
}


static inline int mat_helper_read_mat(const char *ip, const char *name, int *dims, int *dim_size, int *type, char *buf, int buf_size)
{;
    return __mat_helper_read_mat(ip, name, dims, dim_size, type, buf, buf_size, 0);
}

static inline int mat_helper_read_mat2_1(const char *ip, const char *name, int *rows, int *cols, int *ch, char type[8], char *buf, int buf_size)
{
    int dims = 0;
    int dim[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int type_ = MAT_HELPER_INT8;
    if (-1 == mat_helper_read_mat(ip, name, &dims, dim, &type_, buf, buf_size)) { return -1; }
    if (rows && dims >= 1) { *rows = dim[0]; }
    if (cols && dims >= 2) { *cols = dim[1]; }
    if (ch && dims >= 2) { *ch = dim[1]; }
    if (type) { strcpy(type, mat_helper_type_to_str(type_)); }
    return 0;
}


static inline int mat_helper_read_mat2_2(const char *ip, const char *name, int *rows, int *cols, char type[8], char *buf, int buf_size)
{
    int dims = 0;
    int dim[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int type_ = MAT_HELPER_INT8;
    if (-1 == mat_helper_read_mat(ip, name, &dims, dim, &type_, buf, buf_size)) { return -1; }
    if (rows && dims >= 1) { *rows = dim[0]; }
    if (cols && dims >= 2) { *cols = dim[1]; }
    if (type) { strcpy(type, mat_helper_type_to_str(type_)); }
    return 0;
}

static inline int mat_helper_read_mat2_3(const char *name, int *rows, int *cols, int *ch, char type[8], char *buf, int buf_size)
{
    int dims = 0;
    int dim[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int type_ = MAT_HELPER_INT8;
    if (-1 == mat_helper_read_mat("127.0.0.1", name, &dims, dim, &type_, buf, buf_size)) { return -1; }
    if (rows && dims >= 1) { *rows = dim[0]; }
    if (cols && dims >= 2) { *cols = dim[1]; }
    if (ch && dims >= 2) { *ch = dim[1]; }
    if (type) { strcpy(type, mat_helper_type_to_str(type_)); }
    return 0;
}


static inline int mat_helper_read_mat2_4(const char *name, int *rows, int *cols, char type[8], char *buf, int buf_size)
{
    int dims = 0;
    int dim[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int type_ = MAT_HELPER_INT8;
    if (-1 == mat_helper_read_mat("127.0.0.1", name, &dims, dim, &type_, buf, buf_size)) { return -1; }
    if (rows && dims >= 1) { *rows = dim[0]; }
    if (cols && dims >= 2) { *cols = dim[1]; }
    if (type) { strcpy(type, mat_helper_type_to_str(type_)); }
    return 0;
}

static inline int mat_helper_read_mat3_1(const char *ip, const char *name, int *rows, int *cols, int *ch, char *buf, int buf_size)
{
    int dims = 0;
    int dim[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int type_ = MAT_HELPER_INT8;
    if (-1 == mat_helper_read_mat(ip, name, &dims, dim, &type_, buf, buf_size)) { return -1; }
    if (rows && dims >= 1) { *rows = dim[0]; }
    if (cols && dims >= 2) { *cols = dim[1]; }
    if (ch && dims >= 2) { *ch = dim[1]; }
    
    return 0;
}


static inline int mat_helper_read_mat3_2(const char *name, int *rows, int *cols, int *ch, char *buf, int buf_size)
{
    int dims = 0;
    int dim[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int type_ = MAT_HELPER_INT8;
    if (-1 == mat_helper_read_mat("127.0.0.1", name, &dims, dim, &type_, buf, buf_size)) { return -1; }
    if (rows && dims >= 1) { *rows = dim[0]; }
    if (cols && dims >= 2) { *cols = dim[1]; }
    if (ch && dims >= 2) { *ch = dim[1]; }
    
    return 0;
}

static inline int mat_helper_read_mat3_3(const char *name, int *rows, int *cols, char *buf, int buf_size)
{
    int dims = 0;
    int dim[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int type_ = MAT_HELPER_INT8;
    if (-1 == mat_helper_read_mat("127.0.0.1", name, &dims, dim, &type_, buf, buf_size)) { return -1; }
    if (rows && dims >= 1) { *rows = dim[0]; }
    if (cols && dims >= 2) { *cols = dim[1]; }
    return 0;
}





static inline int mat_helper_read_del_mat(const char *ip, const char *name, int *dims, int *dim_size, int *type, char *buf, int buf_size)
{
    return __mat_helper_read_mat(ip, name, dims, dim_size, type, buf, buf_size, 1);
}




static inline int mat_helper_read_del_mat2_1(const char *ip, const char *name, int *rows, int *cols, int *ch, char type[8], char *buf, int buf_size)
{
    int dims = 0;
    int dim[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int type_ = MAT_HELPER_INT8;
    if (-1 == mat_helper_read_del_mat(ip, name, &dims, dim, &type_, buf, buf_size)) { return -1; }
    if (rows && dims >= 1) { *rows = dim[0]; }
    if (cols && dims >= 2) { *cols = dim[1]; }
    if (ch && dims >= 2) { *ch = dim[1]; }
    if (type) { strcpy(type, mat_helper_type_to_str(type_)); }
    return 0;
}


static inline int mat_helper_read_del_mat2_2(const char *ip, const char *name, int *rows, int *cols, char type[8], char *buf, int buf_size)
{
    int dims = 0;
    int dim[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int type_ = MAT_HELPER_INT8;
    if (-1 == mat_helper_read_del_mat(ip, name, &dims, dim, &type_, buf, buf_size)) { return -1; }
    if (rows && dims >= 1) { *rows = dim[0]; }
    if (cols && dims >= 2) { *cols = dim[1]; }
    if (type) { strcpy(type, mat_helper_type_to_str(type_)); }
    return 0;
}

static inline int mat_helper_read_del_mat2_3(const char *name, int *rows, int *cols, int *ch, char type[8], char *buf, int buf_size)
{
    int dims = 0;
    int dim[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int type_ = MAT_HELPER_INT8;
    if (-1 == mat_helper_read_del_mat("127.0.0.1", name, &dims, dim, &type_, buf, buf_size)) { return -1; }
    if (rows && dims >= 1) { *rows = dim[0]; }
    if (cols && dims >= 2) { *cols = dim[1]; }
    if (ch && dims >= 2) { *ch = dim[1]; }
    if (type) { strcpy(type, mat_helper_type_to_str(type_)); }
    return 0;
}


static inline int mat_helper_read_del_mat2_4(const char *name, int *rows, int *cols, char type[8], char *buf, int buf_size)
{
    int dims = 0;
    int dim[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int type_ = MAT_HELPER_INT8;
    if (-1 == mat_helper_read_del_mat("127.0.0.1", name, &dims, dim, &type_, buf, buf_size)) { return -1; }
    if (rows && dims >= 1) { *rows = dim[0]; }
    if (cols && dims >= 2) { *cols = dim[1]; }
    if (type) { strcpy(type, mat_helper_type_to_str(type_)); }
    return 0;
}

static inline int mat_helper_read_del_mat3_1(const char *ip, const char *name, int *rows, int *cols, int *ch, char *buf, int buf_size)
{
    int dims = 0;
    int dim[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int type_ = MAT_HELPER_INT8;
    if (-1 == mat_helper_read_del_mat(ip, name, &dims, dim, &type_, buf, buf_size)) { return -1; }
    if (rows && dims >= 1) { *rows = dim[0]; }
    if (cols && dims >= 2) { *cols = dim[1]; }
    if (ch && dims >= 2) { *ch = dim[1]; }
    
    return 0;
}


static inline int mat_helper_read_del_mat3_2(const char *name, int *rows, int *cols, int *ch, char *buf, int buf_size)
{
    int dims = 0;
    int dim[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int type_ = MAT_HELPER_INT8;
    if (-1 == mat_helper_read_del_mat("127.0.0.1", name, &dims, dim, &type_, buf, buf_size)) { return -1; }
    if (rows && dims >= 1) { *rows = dim[0]; }
    if (cols && dims >= 2) { *cols = dim[1]; }
    if (ch && dims >= 2) { *ch = dim[1]; }
    
    return 0;
}

static inline int mat_helper_read_del_mat3_3(const char *name, int *rows, int *cols, char *buf, int buf_size)
{
    int dims = 0;
    int dim[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int type_ = MAT_HELPER_INT8;
    if (-1 == mat_helper_read_del_mat("127.0.0.1", name, &dims, dim, &type_, buf, buf_size)) { return -1; }
    if (rows && dims >= 1) { *rows = dim[0]; }
    if (cols && dims >= 2) { *cols = dim[1]; }
    return 0;
}





static inline int mat_helper_read_list(const char *ip, char *buf, int buf_size)
{
    mat_helper_socket_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == MAT_HELPER_INVALID_SOCKET)
    {
        return -1;
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MAT_HELPER_PORT);
    addr.sin_addr.s_addr = inet_addr(ip);
    
    if (0 != connect(sock, (struct sockaddr *)&addr, sizeof(addr)))
    {
        mat_helper_close_socket(sock);
        return -1;
    }
            
    struct mat_helper_list_req req;
    memset(&req, 0, sizeof(req));
    req.type = MAT_HELPER_TYPE_LIST_REQ;
    if (mat_helper_write(sock, (char *)&req, sizeof(req)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    
    struct mat_helper_list_res res;
    if (mat_helper_read(sock, (char *)&res, sizeof(res)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    
    if (res.type != MAT_HELPER_TYPE_LIST_RES)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    int len = ntohl(res.len);
    if (len > buf_size - 1)
    {
        len = buf_size - 1;
    }
    if (len > 0)
    {
        if (mat_helper_read(sock, (char *)buf, len) < 0)
        {
            mat_helper_close_socket(sock);
            return -1;
        }
    }
    buf[len] = 0;
    mat_helper_close_socket(sock);
    
    return 0;
}


static inline int mat_helper_del_mat(const char *ip, const char *name)
{
    mat_helper_socket_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == MAT_HELPER_INVALID_SOCKET)
    {
        return -1;
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MAT_HELPER_PORT);
    addr.sin_addr.s_addr = inet_addr(ip);
    
    if (0 != connect(sock, (struct sockaddr *)&addr, sizeof(addr)))
    {
        mat_helper_close_socket(sock);
        return -1;
    }
            
    struct mat_helper_del_req req;
    memset(&req, 0, sizeof(req));
    req.type = MAT_HELPER_TYPE_DEL_REQ;
    strcpy(req.name, name);
    if (mat_helper_write(sock, (char *)&req, sizeof(req)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    
    struct mat_helper_del_res res;
    if (mat_helper_read(sock, (char *)&res, sizeof(res)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    if (res.type != MAT_HELPER_TYPE_DEL_RES || res.result != MAT_HELPER_OK)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    mat_helper_close_socket(sock);
    
    return 0;
}

static inline int mat_helper_del_mat2(const char *ip, const char *name)
{
	return mat_helper_del_mat("127.0.0.1", name);
}







static inline int mat_helper_sem_init(const char *ip, const char *name, unsigned char max)
{
    mat_helper_socket_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == MAT_HELPER_INVALID_SOCKET)
    {
        return -1;
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MAT_HELPER_PORT);
    addr.sin_addr.s_addr = inet_addr(ip);
    
    if (0 != connect(sock, (struct sockaddr *)&addr, sizeof(addr)))
    {
        mat_helper_close_socket(sock);
        return -1;
    }
            
    struct mat_helper_sem_init_req req;
    memset(&req, 0, sizeof(req));
    req.type = MAT_HELPER_TYPE_SEM_INIT_REQ;
    strcpy(req.name, name);
    req.max = max;
    if (mat_helper_write(sock, (char *)&req, sizeof(req)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    
    struct mat_helper_sem_init_res res;
    if (mat_helper_read(sock, (char *)&res, sizeof(res)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    if (res.type != MAT_HELPER_TYPE_SEM_INIT_RES || res.result != MAT_HELPER_OK)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    mat_helper_close_socket(sock);
    
    return 0;
}

static inline int mat_helper_sem_init2_1(const char *name, unsigned char max)
{
    return mat_helper_sem_init("127.0.0.1", name, max);
}

static inline int mat_helper_sem_init2_2(const char *name)
{
    return mat_helper_sem_init("127.0.0.1", name, 255);
}


static inline int mat_helper_sem_destroy(const char *ip, const char *name)
{
    mat_helper_socket_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == MAT_HELPER_INVALID_SOCKET)
    {
        return -1;
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MAT_HELPER_PORT);
    addr.sin_addr.s_addr = inet_addr(ip);
    
    if (0 != connect(sock, (struct sockaddr *)&addr, sizeof(addr)))
    {
        mat_helper_close_socket(sock);
        return -1;
    }
            
    struct mat_helper_sem_destroy_req req;
    memset(&req, 0, sizeof(req));
    req.type = MAT_HELPER_TYPE_SEM_DESTROY_REQ;
    strcpy(req.name, name);
    if (mat_helper_write(sock, (char *)&req, sizeof(req)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    
    struct mat_helper_sem_destroy_res res;
    if (mat_helper_read(sock, (char *)&res, sizeof(res)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    if (res.type != MAT_HELPER_TYPE_SEM_DESTROY_RES || res.result != MAT_HELPER_OK)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    mat_helper_close_socket(sock);
    
    return 0;
}


static inline int mat_helper_sem_destroy2_1(const char *name)
{
    return mat_helper_sem_destroy("127.0.0.1", name); 
}


static inline int mat_helper_sem_post(const char *ip, const char *name)
{
    mat_helper_socket_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == MAT_HELPER_INVALID_SOCKET)
    {
        return -1;
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MAT_HELPER_PORT);
    addr.sin_addr.s_addr = inet_addr(ip);
    
    if (0 != connect(sock, (struct sockaddr *)&addr, sizeof(addr)))
    {
        mat_helper_close_socket(sock);
        return -1;
    }
            
    struct mat_helper_sem_post_req req;
    memset(&req, 0, sizeof(req));
    req.type = MAT_HELPER_TYPE_SEM_POST_REQ;
    strcpy(req.name, name);
    if (mat_helper_write(sock, (char *)&req, sizeof(req)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    
    struct mat_helper_sem_post_res res;
    if (mat_helper_read(sock, (char *)&res, sizeof(res)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    if (res.type != MAT_HELPER_TYPE_SEM_POST_RES || res.result != MAT_HELPER_OK)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    mat_helper_close_socket(sock);
    
    return 0;
}

static inline int mat_helper_sem_post2_1(const char *name)
{
    return mat_helper_sem_post("127.0.0.1", name);
}


static inline int mat_helper_sem_wait(const char *ip, const char *name)
{
    mat_helper_socket_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == MAT_HELPER_INVALID_SOCKET)
    {
        return -1;
    }
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MAT_HELPER_PORT);
    addr.sin_addr.s_addr = inet_addr(ip);
    
    if (0 != connect(sock, (struct sockaddr *)&addr, sizeof(addr)))
    {
        mat_helper_close_socket(sock);
        return -1;
    }
            
    struct mat_helper_sem_wait_req req;
    memset(&req, 0, sizeof(req));
    req.type = MAT_HELPER_TYPE_SEM_WAIT_REQ;
    strcpy(req.name, name);
    if (mat_helper_write(sock, (char *)&req, sizeof(req)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    
    struct mat_helper_sem_wait_res res;
    if (mat_helper_read(sock, (char *)&res, sizeof(res)) < 0)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    if (res.type != MAT_HELPER_TYPE_SEM_WAIT_RES || res.result != MAT_HELPER_OK)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    
    mat_helper_close_socket(sock);
    
    return 0;
}

static inline int mat_helper_sem_wait2_1(const char *ip, const char *name)
{
    return mat_helper_sem_wait("127.0.0.1", name);
}





#ifdef MAT_HELPER_USE_OPENCV

static inline int mat_helper_cvtype2mat_helper(int depth)
{
    switch (depth)
    {
        case CV_8S:
            return MAT_HELPER_INT8;
        case CV_8U:
            return MAT_HELPER_UINT8;
        case CV_16U:
            return MAT_HELPER_UINT16;
        case CV_16S:
            return MAT_HELPER_INT16;
        case CV_32S:
            return MAT_HELPER_INT32;
        case CV_32F:
            return MAT_HELPER_FLOAT32;
        case CV_64F:
            return MAT_HELPER_FLOAT64;
        default:
            return MAT_HELPER_UINT8;
    }
    return MAT_HELPER_UINT8;
}


static inline int mat_helper_mat_helper2cvtype(int type)
{
    switch (type)
    {
        case MAT_HELPER_INT8:
            return CV_8S;
        case MAT_HELPER_UINT8:
            return CV_8U;
        case MAT_HELPER_UINT16:
            return CV_16U;
        case MAT_HELPER_INT16:
            return CV_16S;
        case MAT_HELPER_INT32:
        case MAT_HELPER_UINT32:
            return CV_32S;
        case MAT_HELPER_FLOAT32:
            return CV_32F;
        case MAT_HELPER_FLOAT64:
            return CV_64F;
        default:
            return CV_8U;
    }
    return CV_8U;
}

static inline int mat_helper_save(const char *name, cv::Mat& mat, const char *ip = "127.0.0.1")
{
    int dim_size[3] = {mat.rows, mat.cols, mat.channels()};
    int dims = 3;
    int type = mat_helper_cvtype2mat_helper(mat.depth());
    int mat_size = mat_helper_getsize(dims, dim_size, type);
    
    char *tmp = (char *)malloc(mat_size);
    if (tmp == NULL)
    {
        return -1;
    }
    
    int pos = 0;
    int line_size = mat.cols * mat.elemSize();
    for (int i = 0; i < mat.rows; ++i)
    {
        void *p = mat.ptr<void *>(i);
        memcpy(tmp + pos, p, line_size);
        pos += line_size;
    }
    
    int r = mat_helper_write_mat(ip, name, dims, dim_size, type, tmp);
    free(tmp);
    return r;
}



static inline int mat_helper_load(const char *name, cv::Mat& mat, const char *ip = "127.0.0.1")
{
    struct mat_helper_mat_info info;
    memset(&info, 0, sizeof(info));
    mat_helper_socket_t sock = mat_helper_read_mat_begin(ip, name, &info, 0);
    if (sock == MAT_HELPER_INVALID_SOCKET)
    {
        return -1;
    }
    int mat_size = mat_helper_getsize(info.dims, info.dim_size, info.type);
    
    char *tmp = (char *)malloc(mat_size);
    if (tmp == NULL)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    int r = mat_helper_read_mat_end(sock, &info, (char *)tmp, mat_size);
    if (r == -1)
    {
        free(tmp);
    }
    
    if (info.dims == 0)
    {
        return -1;
    }
    
    if (info.dims < 2)
    {
        info.dim_size[1] = 1;
    }
    
    if (info.dims < 3)
    {
        info.dim_size[2] = 1;
    }
    
    mat = cv::Mat(info.dim_size[0], info.dim_size[1], CV_MAKETYPE(mat_helper_mat_helper2cvtype(info.type), info.dim_size[2]), tmp).clone();
    free(tmp);
    return 0;
}


static inline int mat_helper_load_del(const char *name, cv::Mat& mat, const char *ip = "127.0.0.1")
{
    struct mat_helper_mat_info info;
    memset(&info, 0, sizeof(info));
    mat_helper_socket_t sock = mat_helper_read_mat_begin(ip, name, &info, 1);
    if (sock == MAT_HELPER_INVALID_SOCKET)
    {
        return -1;
    }
    int mat_size = mat_helper_getsize(info.dims, info.dim_size, info.type);
    
    char *tmp = (char *)malloc(mat_size);
    if (tmp == NULL)
    {
        mat_helper_close_socket(sock);
        return -1;
    }
    int r = mat_helper_read_mat_end(sock, &info, (char *)tmp, mat_size);
    if (r == -1)
    {
        free(tmp);
    }
    
    if (info.dims == 0)
    {
        return -1;
    }
    
    if (info.dims < 2)
    {
        info.dim_size[1] = 1;
    }
    
    if (info.dims < 3)
    {
        info.dim_size[2] = 1;
    }
    
    mat = cv::Mat(info.dim_size[0], info.dim_size[1], CV_MAKETYPE(mat_helper_mat_helper2cvtype(info.type), info.dim_size[2]), tmp).clone();
    free(tmp);
    return 0;
}
#endif

#endif