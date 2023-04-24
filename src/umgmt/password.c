/**
 * @file group.h
 * @brief API for the group abstract data type.
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

/**
 * Create a new shadow password data structure.
 *
 * @return New shadow password data structure with NULL initialized fields.
 */
um_shadow_password_t um_shadow_password_new(void);

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
 * Convert the algorithm name to the id which will be written to the shadow data.
 *
 * @param algorithm Algorithm name (md5, sha256, sha512, blowfish etc.) All lowercase.
 * @param id_buffer Buffer to write the id to.
 * @param buffer_size Size of the given buffer.
 *
 * @return Error code (0 on success).
 */
int um_shadow_password_alg_to_id(const char *algorithm, char *id_buffer, size_t buffer_size);

/**
 * Set the algorithm ID of the given shadow password.
 *
 * @param shp Shadow password to use.
 * @param alg_id Algorithm ID to set.
 *
 * @return Error code (0 on success).
 */
int um_shadow_password_set_algorithm_id(um_shadow_password_t *shp, const char *alg_id);

/**
 * Set the salt of the given shadow password.
 *
 * @param shp Shadow password to use.
 * @param salt Salt to set.
 * @param n Salt bytes number.
 *
 * @return Error code (0 on success).
 */
int um_shadow_password_set_salt(um_shadow_password_t *shp, const byte_t *salt, size_t n);

/**
 * Set the hash of the given shadow password.
 *
 * @param shp Shadow password to use.
 * @param hash Hash to set.
 * @param n Hash bytes number.
 *
 * @return Error code (0 on success).
 */
int um_shadow_password_set_hash(um_shadow_password_t *shp, const byte_t *hash, size_t n);

/**
 * Free the given shadow password.
 *
 * @param shp Shadow password to free.
 *
 */
void um_shadow_password_free(um_shadow_password_t *shp);
