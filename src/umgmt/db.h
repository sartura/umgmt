/**
 * @file db.h
 * @brief API for the user/group database abstract data type.
 */
#ifndef UMGMT_DB_H
#define UMGMT_DB_H

#include "types.h"

#include <pwd.h>

/**
 * Allocate new database.
 *
 * @return New allocated database.
 *
 */
um_db_t *um_db_new(void);

/**
 * Load database from the system.
 *
 * @param db Database to load.
 *
 * @return Error code - 0 on success.
 *
 */
int um_db_load(um_db_t *db);

/**
 * Store database to the system.
 *
 * @param db Database to store.
 *
 * @return Error code - 0 on success.
 *
 */
int um_db_store(um_db_t *db);

/**
 * Return the new UID which can be used for a new user.
 *
 * @param db Database to use.
 *
 * @return New UID.
 *
 */
uid_t um_db_get_new_uid(um_db_t *db);

/**
 * Return the new GID which can be used for a new user/group.
 *
 * @param db Database to use.
 *
 * @return New GID.
 *
 */
gid_t um_db_get_new_gid(um_db_t *db);

/**
 * Add a new user to the database.
 * User will be handled by the database from this point on - do not free user data after adding user to the database.
 *
 * @param db Database to use.
 * @param user User to search for.
 *
 * @return Error code - 0 on success.
 *
 */
int um_db_add_user(um_db_t *db, um_user_t *user);

/**
 * Add a new group to the database.
 * Group will be handled by the database from this point on - do not free group data after adding user to the database.
 *
 * @param db Database to use.
 * @param group Group to add.
 *
 * @return Error code - 0 on success.
 *
 */
int um_db_add_group(um_db_t *db, um_group_t *group);

/**
 * Get the user from the database.
 *
 * @param db Database to use.
 * @param name User to search for.
 *
 * @return Abstract user type - NULL if not found.
 *
 */
um_user_t *um_db_get_user(um_db_t *db, const char *name);

/**
 * Get the group from the database.
 *
 * @param db Database to use.
 * @param name Group to search for.
 *
 * @return Abstract group type - NULL if not found.
 *
 */
um_group_t *um_db_get_group(um_db_t *db, const char *name);

/**
 * Delete user from the database.
 *
 * @param db Database to use.
 * @param name User to search for.
 *
 * @return Error code - 0 on success.
 *
 */
int um_db_delete_user(um_db_t *db, const char *name);

/**
 * Delete group from the database.
 *
 * @param db Database to use.
 * @param name Group to search for.
 *
 * @return Error code - 0 on success.
 *
 */
int um_db_delete_group(um_db_t *db, const char *name);

/**
 * Get users list head.
 *
 * @param db Database to use.
 *
 * @return Users list head.
 *
 */
const um_user_element_t *um_db_get_user_list_head(const um_db_t *db);

/**
 * Get group list head.
 *
 * @param db Database to use.
 *
 * @return Group list head.
 *
 */
const um_group_element_t *um_db_get_group_list_head(const um_db_t *db);

/**
 * Free database data.
 *
 * @param db Database to free.
 *
 */
void um_db_free(um_db_t *db);

#endif // UMGMT_DB_H