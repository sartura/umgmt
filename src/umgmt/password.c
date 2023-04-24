/**
 * @file password.c
 * @brief API for the password handling.
 *
 * Copyright (c) 2022 Sartura Ltd.
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */
#include "password.h"
#include "types.h"
#include "dyn_buffer.h"

#include <string.h>
#include <stdio.h>

/**
 * Create a new shadow password data structure.
 *
 * @return New shadow password data structure with NULL initialized fields.
 */
um_shadow_password_t um_shadow_password_new(void)
{
    return (um_shadow_password_t){
        .algorithm = NULL,
        .salt = um_dyn_byte_buffer_new(),
        .hash = um_dyn_byte_buffer_new(),
    };
}

/**
 * Parse the given string into the shadow password data type.
 *
 * @param password_hash Password hash data to parse.
 * @param shp Shadow password structure to store data.
 *
 * @return Error code (0 on success).
 */
int um_shadow_password_parse_hash(const char *password_hash, um_shadow_password_t *shp);

/**
 * Create the shadow password from the plaintext password.
 *
 * @param password Plaintext password.
 * @param algorithm Hashing algorithm to use.
 * @param use_salt Flag for salt usage.
 * @param shp Shadow password to store the data in.
 *
 * @return Error code(0 on success).
 */
int um_shadow_password_from_plaintext(const char *password, const char *algorithm, bool_t use_salt,
                                      um_shadow_password_t *shp);

/**
 * Convert the algorithm name to the um_hash_algorithm_t enum value.
 *
 * @param algorithm Lowercase algorithm name (md5, sha256, sha512, blowfish etc.)
 *
 * @return Hash algorithm.
 */
um_hash_algorithm_t um_shadow_password_algorithm_to_id(const char *algorithm)
{
    um_hash_algorithm_t id = um_hash_algorithm_unknown;

    struct
    {
        const char *algorithm;
        um_hash_algorithm_t id;
    } id_pairs[] = {
        {
            .algorithm = "unknown",
            .id = um_hash_algorithm_unknown,
        },
        {
            .algorithm = "md5",
            .id = um_hash_algorithm_md5,
        },
        {
            .algorithm = "blowfish",
            .id = um_hash_algorithm_blowfish,
        },
        {
            .algorithm = "bcrypt",
            .id = um_hash_algorithm_bcrypt,
        },
        {
            .algorithm = "sha256",
            .id = um_hash_algorithm_sha256,
        },
        {
            .algorithm = "sha512",
            .id = um_hash_algorithm_sha512,
        },
    };

    for (size_t i = 0; i < sizeof(id_pairs) / sizeof(id_pairs[0]); i++)
    {
        if (!strcmp(id_pairs[i].algorithm, algorithm))
        {
            id = id_pairs[i].id;
            break;
        }
    }

    return id;
}

/**
 * Convert the um_hash_algorithm_t enum to string.
 *
 * @param id Algorithm id.
 *
 * @return Algorithm id value (for example, $1$ is the id for md5. um_hash_algorithm_md5 will be converted to "1").
 */
const char *um_shadow_password_algorithm_id2str(um_hash_algorithm_t id)
{
    const char *id_str = NULL;

    struct
    {
        um_hash_algorithm_t id;
        const char *id_str;
    } id_pairs[] = {
        {
            .id = um_hash_algorithm_unknown,
            .id_str = NULL,
        },
        {
            .id = um_hash_algorithm_md5,
            .id_str = "1",
        },
        {
            .id = um_hash_algorithm_blowfish,
            .id_str = "2a",
        },
        {
            .id = um_hash_algorithm_bcrypt,
            .id_str = "2b",
        },
        {
            .id = um_hash_algorithm_sha256,
            .id_str = "5",
        },
        {
            .id = um_hash_algorithm_sha512,
            .id_str = "6",
        },
    };

    for (size_t i = 0; i < sizeof(id_pairs) / sizeof(id_pairs[0]); i++)
    {
        if (id_pairs[i].id == id)
        {
            id_str = id_pairs[i].id_str;
            break;
        }
    }

    return id_str;
}

/**
 * Set the algorithm ID of the given shadow password.
 *
 * @param shp Shadow password to use.
 * @param alg_id Algorithm ID to set.
 *
 * @return Error code (0 on success).
 */
int um_shadow_password_set_algorithm_id(um_shadow_password_t *shp, const char *alg_id)
{
    int error = 0;

    const size_t len = strlen(alg_id);

    if (shp->algorithm)
    {
        // [TODO]: decide what to do if this is the case
    }

    shp->algorithm = (char *)malloc(sizeof(char) * len);
    if (shp->algorithm == NULL)
    {
        goto error_out;
    }

    memcpy(shp->algorithm, alg_id, len);

    // successful end
    goto out;

error_out:
    error = -1;

out:
    return error;
}

/**
 * Set the salt of the given shadow password.
 *
 * @param shp Shadow password to use.
 * @param salt Salt to set.
 * @param n Salt bytes number.
 *
 * @return Error code (0 on success).
 */
int um_shadow_password_set_salt(um_shadow_password_t *shp, const byte_t *salt, size_t n)
{
    return um_dyn_byte_buffer_copy(
        &(um_dyn_byte_buffer_t){
            .buffer = (byte_t *)salt,
            .size = n,
        },
        &shp->salt);
}

/**
 * Set the hash of the given shadow password.
 *
 * @param shp Shadow password to use.
 * @param hash Hash to set.
 * @param n Hash bytes number.
 *
 * @return Error code (0 on success).
 */
int um_shadow_password_set_hash(um_shadow_password_t *shp, const byte_t *hash, size_t n)
{
    return um_dyn_byte_buffer_copy(
        &(um_dyn_byte_buffer_t){
            .buffer = (byte_t *)hash,
            .size = n,
        },
        &shp->hash);
}

/**
 * Free the given shadow password.
 *
 * @param shp Shadow password to free.
 *
 */
void um_shadow_password_free(um_shadow_password_t *shp)
{
    if (shp->algorithm)
    {
        free(shp->algorithm);
    }

    um_dyn_byte_buffer_free(&shp->salt);
    um_dyn_byte_buffer_free(&shp->hash);

    *shp = um_shadow_password_new();
}
