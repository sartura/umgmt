/**
 * @file user.h
 * @brief API for the user abstract data type.
 */
#ifndef UMGMT_USER_H
#define UMGMT_USER_H

#include "types.h"

#include <pwd.h>
#include <shadow.h>

/**
 * Allocate new user.
 *
 * @return New allocated user.
 */
um_user_t *um_user_new();

/**
 * Set the name for the user.
 *
 * @param user User to use.
 * @param name Name to set.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_set_name(um_user_t *user, const char *name);

/**
 * Set the password value from /etc/passwd for the user.
 *
 * @param user User to use.
 * @param password Password value to set.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_set_password(um_user_t *user, const char *password);

/**
 * Set UID for the user.
 *
 * @param user User to use.
 * @param uid UID to set.
 *
 */
void um_user_set_uid(um_user_t *user, uid_t uid);

/**
 * Set GID for the user.
 *
 * @param user User to use.
 * @param gid GID to set.
 *
 */
void um_user_set_gid(um_user_t *user, gid_t gid);

/**
 * Set user gecos value.
 *
 * @param user User to use.
 * @param gecos Gecos value.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_set_gecos(um_user_t *user, const char *gecos);

/**
 * Set home path for the user.
 *
 * @param user User to use.
 * @param path Home path to set.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_set_home_path(um_user_t *user, const char *path);

/**
 * Set shell path for the user.
 *
 * @param user User to use.
 * @param path Shell path to set.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_set_shell_path(um_user_t *user, const char *path);

/**
 * Set the password hash for the user.
 *
 * @param user User to use.
 * @param password_hash Password hash to set.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_set_password_hash(um_user_t *user, const char *password_hash);

/**
 * Set date of the last change for the user.
 *
 * @param user User to use.
 * @param last_change Date of the last change to set.
 *
 */
void um_user_set_last_change(um_user_t *user, long int last_change);

/**
 * Set minimum number of days between changes for the user.
 *
 * @param user User to use.
 * @param change_min Minimum number of days between changes to set.
 *
 */
void um_user_set_change_min(um_user_t *user, long int change_min);

/**
 * Set maximum number of days between changes for the user.
 *
 * @param user User to use.
 * @param change_max Maximum number of days between changes to set.
 *
 */
void um_user_set_change_max(um_user_t *user, long int change_max);

/**
 * Set number of days to warn the user to change the password.
 *
 * @param user User to use.
 * @param warn_days Number of days to warn the user to change the password.
 *
 */
void um_user_set_warn_days(um_user_t *user, long int warn_days);

/**
 * Set number of days the account may be inactive.
 *
 * @param user User to use.
 * @param inactive_days Number of days the account may be inactive.
 *
 */
void um_user_set_inactive_days(um_user_t *user, long int inactive_days);

/**
 * Set number of days until the account expires.
 *
 * @param user User to use.
 * @param expiration Number of days until the account expires.
 *
 */
void um_user_set_expiration(um_user_t *user, long int expiration);

/**
 * Set reserved shadow.h flags.
 *
 * @param user User to use.
 * @param flags Reserved flags.
 *
 */
void um_user_set_flags(um_user_t *user, long int flags);

/**
 * Get user name.
 *
 * @param user User to use.
 *
 * @return User name.
 *
 */
const char *um_user_get_name(const um_user_t *user);

/**
 * Get user password.
 *
 * @param user User to use.
 *
 * @return User's password value from /etc/passwd.
 *
 */
const char *um_user_get_password(const um_user_t *user);

/**
 * Get user UID.
 *
 * @param user User to use.
 *
 * @return User UID.
 *
 */
uid_t um_user_get_uid(const um_user_t *user);

/**
 * Get user GID.
 *
 * @param user User to use.
 *
 * @return User GID.
 *
 */
gid_t um_user_get_gid(const um_user_t *user);

/**
 * Get user gecos.
 *
 * @param user User to use.
 *
 * @return User's gecos value.
 *
 */
const char *um_user_get_gecos(const um_user_t *user);

/**
 * Get user home path.
 *
 * @param user User to use.
 *
 * @return User home path.
 *
 */
const char *um_user_get_home_path(const um_user_t *user);

/**
 * Get user shell path.
 *
 * @param user User to use.
 *
 * @return User shell path.
 *
 */
const char *um_user_get_shell_path(const um_user_t *user);

/**
 * Get user password hash.
 *
 * @param user User to use.
 *
 * @return User's password hash.
 *
 */
const char *um_user_get_password_hash(const um_user_t *user);

/**
 * Get user last change.
 *
 * @param user User to use.
 *
 * @return User last change.
 *
 */
long int um_user_get_last_change(const um_user_t *user);

/**
 * Get user minimum number of days between changes.
 *
 * @param user User to use.
 *
 * @return User minimum number of days between changes.
 *
 */
long int um_user_get_change_min(const um_user_t *user);

/**
 * Get user maximum number of days between changes.
 *
 * @param user User to use.
 *
 * @return User maximum number of days between changes.
 *
 */
long int um_user_get_change_max(const um_user_t *user);

/**
 * Get number of days to warn user.
 *
 * @param user User to use.
 *
 * @return User number of days of a warning.
 *
 */
long int um_user_get_warn_days(const um_user_t *user);

/**
 * Get user inactive days.
 *
 * @param user User to use.
 *
 * @return User inactive days.
 *
 */
long int um_user_get_inactive_days(const um_user_t *user);

/**
 * Get user expiration.
 *
 * @param user User to use.
 *
 * @return User expiration.
 *
 */
long int um_user_get_expiration(const um_user_t *user);

/**
 * Get user reserved flags.
 *
 * @param user User to use.
 *
 * @return User reserved flags.
 *
 */
long int um_user_get_flags(const um_user_t *user);

/**
 * Free user data.
 *
 * @param user User to free.
 *
 */
void um_user_free(um_user_t *user);

#endif // UMGMT_USER_H