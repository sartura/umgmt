/**
 *
 * Copyright (c) 2022 Sartura Ltd.
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */
#include "user.h"

#include "user.h"

#include <dirent.h>
#include <linux/limits.h>
#include <pwd.h>
#include <shadow.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

typedef struct um_shadow_data_s um_shadow_data_t;

struct um_shadow_data_s
{
    char *password_hash;
    long int last_change;
    long int change_min;
    long int change_max;
    long int warn_days;
    long int inactive_days;
    long int expiration;
    long int flags;
};

struct um_user_s
{
    char *name;
    char *password;
    uid_t uid;
    gid_t gid;
    char *gecos;
    char *home_path;
    char *shell_path;
    um_shadow_data_t shadow;
};

static int user_processes(const um_user_t *user, process_operations pop);

/**
 * Allocate new user.
 *
 * @return New allocated user.
 */
um_user_t *um_user_new(void)
{
    um_user_t *new_user = (um_user_t *)malloc(sizeof(um_user_t));

    if (!new_user)
    {
        return NULL;
    }

    // set all to 0
    *new_user = (um_user_t){0};

    return new_user;
}

/**
 * Set the name for the user.
 *
 * @param user User to use.
 * @param name Name to set.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_set_name(um_user_t *user, const char *name)
{
    if (user->name)
    {
        free(user->name);
        user->name = 0;
    }

    if (name)
    {
        user->name = strdup(name);
        if (!user->name)
        {
            return -1;
        }
    }

    return 0;
}

/**
 * Set the password value from /etc/passwd for the user.
 *
 * @param user User to use.
 * @param password Password value to set.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_set_password(um_user_t *user, const char *password)
{
    if (user->password)
    {
        free(user->password);
        user->password = 0;
    }

    if (password)
    {
        user->password = strdup(password);
        if (!user->password)
        {
            return -1;
        }
    }

    return 0;
}

/**
 * Set UID for the user.
 *
 * @param user User to use.
 * @param uid UID to set.
 *
 */
void um_user_set_uid(um_user_t *user, uid_t uid)
{
    user->uid = uid;
}

/**
 * Set GID for the user.
 *
 * @param user User to use.
 * @param gid GID to set.
 *
 */
void um_user_set_gid(um_user_t *user, gid_t gid)
{
    user->gid = gid;
}

/**
 * Set user gecos value.
 *
 * @param user User to use.
 * @param gecos Gecos value.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_set_gecos(um_user_t *user, const char *gecos)
{
    if (user->gecos)
    {
        free(user->gecos);
        user->gecos = 0;
    }

    if (gecos)
    {
        user->gecos = strdup(gecos);
        if (!user->gecos)
        {
            return -1;
        }
    }

    return 0;
}

/**
 * Set home path for the user.
 *
 * @param user User to use.
 * @param path Home path to set.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_set_home_path(um_user_t *user, const char *path)
{
    if (user->home_path)
    {
        free(user->home_path);
        user->home_path = 0;
    }

    if (path)
    {
        user->home_path = strdup(path);
        if (!user->home_path)
        {
            return -1;
        }
    }

    return 0;
}

/**
 * Set shell path for the user.
 *
 * @param user User to use.
 * @param path Shell path to set.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_set_shell_path(um_user_t *user, const char *path)
{
    if (user->shell_path)
    {
        free(user->shell_path);
        user->shell_path = 0;
    }

    if (path)
    {
        user->shell_path = strdup(path);
        if (!user->shell_path)
        {
            return -1;
        }
    }

    return 0;
}

/**
 * Set the password hash for the user.
 *
 * @param user User to use.
 * @param password_hash Password hash to set.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_set_password_hash(um_user_t *user, const char *password_hash)
{
    if (user->shadow.password_hash)
    {
        free(user->shadow.password_hash);
        user->shadow.password_hash = 0;
    }

    if (password_hash)
    {
        user->shadow.password_hash = strdup(password_hash);
        if (!user->shadow.password_hash)
        {
            return -1;
        }
    }

    return 0;
}

/**
 * Set date of the last change for the user.
 *
 * @param user User to use.
 * @param last_change Date of the last change to set.
 *
 */
void um_user_set_last_change(um_user_t *user, long int last_change)
{
    user->shadow.last_change = last_change;
}

/**
 * Set minimum number of days between changes for the user.
 *
 * @param user User to use.
 * @param change_min Minimum number of days between changes to set.
 *
 */
void um_user_set_change_min(um_user_t *user, long int change_min)
{
    user->shadow.change_min = change_min;
}

/**
 * Set maximum number of days between changes for the user.
 *
 * @param user User to use.
 * @param change_max Maximum number of days between changes to set.
 *
 */
void um_user_set_change_max(um_user_t *user, long int change_max)
{
    user->shadow.change_max = change_max;
}

/**
 * Set number of days to warn the user to change the password.
 *
 * @param user User to use.
 * @param warn_days Number of days to warn the user to change the password.
 *
 */
void um_user_set_warn_days(um_user_t *user, long int warn_days)
{
    user->shadow.warn_days = warn_days;
}

/**
 * Set number of days the account may be inactive.
 *
 * @param user User to use.
 * @param inactive_days Number of days the account may be inactive.
 *
 */
void um_user_set_inactive_days(um_user_t *user, long int inactive_days)
{
    user->shadow.inactive_days = inactive_days;
}

/**
 * Set number of days until the account expires.
 *
 * @param user User to use.
 * @param expiration Number of days until the account expires.
 *
 */
void um_user_set_expiration(um_user_t *user, long int expiration)
{
    user->shadow.expiration = expiration;
}

/**
 * Set reserved shadow.h flags.
 *
 * @param user User to use.
 * @param flags Reserved flags.
 *
 */
void um_user_set_flags(um_user_t *user, long int flags)
{
    user->shadow.flags = flags;
}

/**
 * Get user name.
 *
 * @param user User to use.
 *
 * @return User name.
 *
 */
const char *um_user_get_name(const um_user_t *user)
{
    return user->name;
}

/**
 * Get user password.
 *
 * @param user User to use.
 *
 * @return User's password value from /etc/passwd.
 *
 */
const char *um_user_get_password(const um_user_t *user)
{
    return user->password;
}

/**
 * Get user UID.
 *
 * @param user User to use.
 *
 * @return User UID.
 *
 */
uid_t um_user_get_uid(const um_user_t *user)
{
    return user->uid;
}

/**
 * Get user GID.
 *
 * @param user User to use.
 *
 * @return User GID.
 *
 */
gid_t um_user_get_gid(const um_user_t *user)
{
    return user->gid;
}

/**
 * Get user gecos.
 *
 * @param user User to use.
 *
 * @return User's gecos value.
 *
 */
const char *um_user_get_gecos(const um_user_t *user)
{
    return user->gecos;
}

/**
 * Get user home path.
 *
 * @param user User to use.
 *
 * @return User home path.
 *
 */
const char *um_user_get_home_path(const um_user_t *user)
{
    return user->home_path;
}

/**
 * Get user shell path.
 *
 * @param user User to use.
 *
 * @return User shell path.
 *
 */
const char *um_user_get_shell_path(const um_user_t *user)
{
    return user->shell_path;
}

/**
 * Get user password hash.
 *
 * @param user User to use.
 *
 * @return User's password hash.
 *
 */
const char *um_user_get_password_hash(const um_user_t *user)
{
    return user->shadow.password_hash;
}

/**
 * Get user last change.
 *
 * @param user User to use.
 *
 * @return User last change.
 *
 */
long int um_user_get_last_change(const um_user_t *user)
{
    return user->shadow.last_change;
}

/**
 * Get user minimum number of days between changes.
 *
 * @param user User to use.
 *
 * @return User minimum number of days between changes.
 *
 */
long int um_user_get_change_min(const um_user_t *user)
{
    return user->shadow.change_min;
}

/**
 * Get user maximum number of days between changes.
 *
 * @param user User to use.
 *
 * @return User maximum number of days between changes.
 *
 */
long int um_user_get_change_max(const um_user_t *user)
{
    return user->shadow.change_max;
}

/**
 * Get number of days to warn user.
 *
 * @param user User to use.
 *
 * @return User number of days of a warning.
 *
 */
long int um_user_get_warn_days(const um_user_t *user)
{
    return user->shadow.warn_days;
}

/**
 * Get user inactive days.
 *
 * @param user User to use.
 *
 * @return User inactive days.
 *
 */
long int um_user_get_inactive_days(const um_user_t *user)
{
    return user->shadow.inactive_days;
}

/**
 * Get user expiration.
 *
 * @param user User to use.
 *
 * @return User expiration.
 *
 */
long int um_user_get_expiration(const um_user_t *user)
{
    return user->shadow.expiration;
}

/**
 * Get user reserved flags.
 *
 * @param user User to use.
 *
 * @return User reserved flags.
 *
 */
long int um_user_get_flags(const um_user_t *user)
{
    return user->shadow.flags;
}

/**
 * Kill all of user's processes
 *
 * @param user User to use.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_kill_all_proc(const um_user_t *user)
{
    return user_processes(user, PROC_TERM);
}

/**
 * Check for users processes and do process_operations when running process(es) found
 *
 * @param user User to use.
 * @param process_operations operation to perform when a process for user is found
 *
 * @return Error code - 0 on success.
 *
 */
static int user_processes(const um_user_t *user, process_operations pop)
{
    int error = 0;
    DIR *proc_d = NULL;
    FILE *status_f = NULL;
    struct dirent *proc = NULL;
    char nums[] = "1234567890";
    char status_path[NAME_MAX] = { 0 };
    char buf[1024] = { 0 };
    size_t proc_ruid = 0;
    size_t proc_id = 0;

    proc_d = opendir("/proc");
    if (proc_d == NULL) {
        goto error_out;
    }

    while ((proc = readdir(proc_d)) != NULL) {
        if (strcmp(proc->d_name, ".") && strcmp(proc->d_name, "..")) {
            // check if directory name has only numbers
            if (proc->d_type == DT_DIR && (strspn(proc->d_name, nums) == strlen(proc->d_name))) {
                if (snprintf(status_path, NAME_MAX, "/proc/%s/status", proc->d_name) < 0) {
                    goto error_out;
                }

                proc_id = strtol(proc->d_name, NULL, 10);
                if (proc_id == 0) {
                    goto error_out;
                }

                status_f = fopen(status_path, "r");
                if (status_f == NULL) {
                    goto error_out;
                }

                while (fgets (buf, sizeof(buf), status_f) == buf) {
                    if (sscanf(buf, "Uid:\t%lu", &proc_ruid) == 1) {
                        if (proc_ruid == user->uid) {
                            if (pop == PROC_CHECK) {
                                error = 0;
                                goto out;
                            } else if (pop == PROC_TERM) {
                                if (kill(proc_id, SIGTERM) != 0) {
                                    if (kill(proc_id, SIGKILL) != 0) {
                                        goto error_out;
                                    }
                                }
                            }
                        }
                    }
                }
                fclose(status_f);
                status_f = NULL;
            }
        }
    }

error_out:
    error = -1;

out:
    if (proc_d) {
        closedir(proc_d);
    }
    if (status_f) {
        fclose(status_f);
    }

    return error;
}

/**
 * Free user data.
 *
 * @param user User to free.
 *
 */
void um_user_free(um_user_t *user)
{
    // free all fields

    if (user->name)
    {
        free(user->name);
    }

    if (user->password)
    {
        free(user->password);
    }

    if (user->gecos)
    {
        free(user->gecos);
    }

    if (user->home_path)
    {
        free(user->home_path);
    }

    if (user->shell_path)
    {
        free(user->shell_path);
    }

    if (user->shadow.password_hash)
    {
        free(user->shadow.password_hash);
    }

    // free allocated struct
    free(user);
}