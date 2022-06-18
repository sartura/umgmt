/**
 * @file user_db.h
 * @brief API for the user database abstract data type.
 */
#ifndef UMGMT_USER_DB_H
#define UMGMT_USER_DB_H

#include "types.h"

/**
 * Allocate new user database.
 *
 * @return New allocated database.
 *
 */
um_user_db_t *um_user_db_new(void);

/**
 * Load user database from the system.
 *
 * @param db Database to load.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_db_load(um_user_db_t *db);

/**
 * Store user database to the system.
 *
 * @param db Database to store.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_db_store(um_user_db_t *db);

/**
 * Get the user from the database.
 *
 * @param db Database to use.
 * @param user_name User to search for.
 *
 * @return Abstract user type - NULL if not found.
 *
 */
um_user_t *um_user_db_get_user(um_user_db_t *db, const char *user_name);

/**
 * Delete user from the database.
 *
 * @param db Database to use.
 * @param user_name User to search for.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_db_delete_user(um_user_db_t *db, const char *user_name);

/**
 * Get users list head.
 *
 * @param db Database to use.
 *
 * @return Users list head.
 *
 */
const um_user_element_t *um_user_db_get_user_list_head(const um_user_db_t *db);

/**
 * Free user database data.
 *
 * @param db User database to free.
 *
 */
void um_user_db_free(um_user_db_t *db);

#endif // UMGMT_USER_DB_H