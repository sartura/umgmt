/**
 * @file password.h
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
#ifndef UMGMT_PASSWORD_H
#define UMGMT_PASSWORD_H

#include "types.h"

#include <stdint.h>
#include <stdlib.h>

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
 * Convert the algorithm name to the um_hash_algorithm_t enum value.
 *
 * @param algorithm Lowercase algorithm name (md5, sha256, sha512, blowfish etc.)
 *
 * @return Hash algorithm.
 */
um_hash_algorithm_t um_shadow_password_algorithm_to_id(const char *algorithm);

/**
 * Convert the um_hash_algorithm_t enum to string.
 *
 * @param id Algorithm id.
 *
 * @return Algorithm id value (for example, $1$ is the id for md5. um_hash_algorithm_md5 will be converted to "1").
 */
const char *um_shadow_password_algorithm_id2str(um_hash_algorithm_t id);

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

#endif // UMGMT_PASSWORD_H
