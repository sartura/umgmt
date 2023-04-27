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
#include <assert.h>

#include <openssl/sha.h>
#include <openssl/rand.h>

static int hash_with_sha256(const char *password, bool_t use_salt, um_shadow_password_t *shp)
{
    int error = 0;

    // full data buffer
    um_dyn_byte_buffer_t data_buffer = um_dyn_byte_buffer_new();

    // calculate sizes
    const size_t salt_len = 16;
    const size_t password_len = strlen(password);
    const size_t full_size = salt_len + password_len;
    size_t curr_ptr = 0;

    // allocate full size data
    error = um_dyn_byte_buffer_alloc(&data_buffer, full_size);
    if (error)
    {
        return -1;
    }

    // append salt to the start of the data
    if (use_salt)
    {
        um_dyn_byte_buffer_alloc(&shp->salt, salt_len);

        error = RAND_bytes(shp->salt.buffer, (int)salt_len);
        if (!error)
        {
            return -2;
        }

        // copy the salt at the start of the data
        memcpy(shp->salt.buffer, data_buffer.buffer, salt_len);
        curr_ptr += salt_len;
    }

    // copy the plaintext data after the salt
    memcpy((void *)password, &data_buffer.buffer[curr_ptr], password_len);

    // now hash the data and store it to the shp hash buffer
    // allocate the hash buffer:
    error = um_dyn_byte_buffer_alloc(&shp->hash, SHA256_DIGEST_LENGTH);
    if (error)
    {
        return -3;
    }

    // calculate sha256 hash:
    SHA256(data_buffer.buffer, data_buffer.size, shp->hash.buffer);

    return error;
}

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
int um_shadow_password_parse_hash(const char *password_hash, um_shadow_password_t *shp)
{
    // tokenize password hash and store shadow and password data
    int error = 0;
    char *token = NULL;
    size_t count = 0;
    um_shadow_password_t temp = um_shadow_password_new();

    // split string into 3 parts - algorithm, salt and hash
    char *tokens[3] = {0, 0, 0};

    token = strtok((char *)password_hash, "$");
    while (token)
    {
        tokens[count] = strdup(token);
        ++count;
        token = strtok(NULL, "$");
    }

    // assert non null
    for (size_t i = 0; i < 3; i++)
    {
        assert(tokens[i] != NULL);
    }

    // use tokens to create the password data structure
    const char *algorithm = tokens[0];
    const byte_t *salt = (const byte_t *)tokens[1];
    const byte_t *hash = (const byte_t *)tokens[2];

    size_t salt_len = strlen((const char *)salt);
    size_t hash_len = strlen((const char *)hash);

    error = um_shadow_password_set_algorithm_id(&temp, algorithm);
    if (error != 0)
    {
        goto error_out;
    }

    error = um_shadow_password_set_salt(&temp, salt, salt_len);
    if (error != 0)
    {
        goto error_out;
    }

    error = um_shadow_password_set_hash(&temp, hash, hash_len);
    if (error != 0)
    {
        goto error_out;
    }

    goto out;

error_out:
    error = -1;

out:
    // free allocated data
    for (size_t i = 0; i < 3; i++)
    {
        free(tokens[i]);
    }

    return 0;
}

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
                                      um_shadow_password_t *shp)
{
    int error = 0;

    // store temporary data before applying changes to the output
    um_shadow_password_t temp = um_shadow_password_new();

    // get the algorithm id
    um_hash_algorithm_t alg_id = um_shadow_password_algorithm_to_id(algorithm);

    // hash the plaintext password depending on the algorithm
    switch (alg_id)
    {
    case um_hash_algorithm_unknown:
        // [TODO]: return an error
        break;
    case um_hash_algorithm_sha256:
        error = hash_with_sha256(password, use_salt, &temp);
        if (error)
        {
            goto error_out;
        }
        break;
    }

    // copy the temp value to the output
    shp->algorithm = strdup(um_shadow_password_algorithm_id2str(alg_id));

    error = um_shadow_password_set_salt(shp, temp.salt.buffer, temp.salt.size);
    if (error)
    {
        goto error_out;
    }

    error = um_shadow_password_set_hash(shp, temp.hash.buffer, temp.hash.size);
    if (error)
    {
        goto error_out;
    }

    goto out;

error_out:
    error = -1;

out:
    // release the temporary value
    um_shadow_password_free(&temp);

    return error;
}

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

    if (shp->algorithm)
    {
        // [TODO]: decide what to do if this is the case
    }

    shp->algorithm = strdup(alg_id);
    if (shp->algorithm == NULL)
    {
        goto error_out;
    }

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
