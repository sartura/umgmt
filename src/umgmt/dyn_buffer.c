/**
 * @file dyn_buffer.h
 * @brief Dynamically allocated buffer API.
 *
 * Copyright (c) 2022 Sartura Ltd.
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */
#include "dyn_buffer.h"

#include <string.h>

/**
 * Create a new dynamic byte buffer.
 */
um_dyn_byte_buffer_t um_dyn_byte_buffer_new(void)
{
    return (um_dyn_byte_buffer_t){
        .buffer = NULL,
        .size = 0,
    };
}

/**
 * Allocate space for the dynamic buffer.
 *
 * @param buff Buffer to allocate.
 * @param size Size of the data to allocate.
 */
int um_dyn_byte_buffer_alloc(um_dyn_byte_buffer_t *buff, size_t size)
{
    buff->buffer = (byte_t *)malloc(sizeof(byte_t) * size);
    if (!buff->buffer)
    {
        return -1;
    }

    buff->size = size;

    return 0;
}

/**
 * Copy the given buffer into the destination.
 *
 * @param src Source buffer.
 * @param dst Destination buffer.
 *
 * @return Error code (0 on success).
 */
int um_dyn_byte_buffer_copy(um_dyn_byte_buffer_t *src, um_dyn_byte_buffer_t *dst)
{
    dst->buffer = (byte_t *)malloc(sizeof(byte_t) * src->size);
    if (!dst->buffer)
    {
        return -1;
    }

    // adjust the buffer size
    const size_t n = dst->size = src->size;

    // copy the buffer into the destination
    memcpy(src->buffer, dst->buffer, n);

    return 0;
}

/**
 * Free the given byte buffer.
 *
 * @param buff Dynamic byte buffer.
 *
 */
void um_dyn_byte_buffer_free(um_dyn_byte_buffer_t *buff)
{
    if (buff->size)
    {
        free(buff->buffer);
        *buff = um_dyn_byte_buffer_new();
    }
}
