#include "user_db.h"
#include "umgmt/types.h"
#include "user.h"

#include <string.h>
#include <stdlib.h>
#include <utlist.h>
#include <pwd.h>
#include <shadow.h>
#include <stdbool.h>
#include <stdio.h>

struct um_user_db_s
{
    um_user_element_t *users_head;
};

static int um_user_element_cmp_fn(void *d1, void *d2);

/**
 * Allocate new user database.
 *
 * @return New allocated database.
 *
 */
um_user_db_t *um_user_db_new(void)
{
    um_user_db_t *new_db = (um_user_db_t *)malloc(sizeof(um_user_db_t));

    if (!new_db)
    {
        return NULL;
    }

    *new_db = (um_user_db_t){0};

    return new_db;
}

/**
 * Load user database.
 *
 * @param db Database to load.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_db_load(um_user_db_t *db)
{
    int error = 0;

    // temp data
    um_user_t *tmp_user = NULL;
    um_user_element_t *tmp_user_element = NULL, search_element = {0}, *found_element = NULL;

    // make sure no memory leak occurs
    bool user_set = false;

    // passwd and shadow
    struct passwd *pwd = NULL;
    struct spwd *spwd = NULL;
    bool passwd_closed = false, shadow_closed = false;

    // allocate search element data for later usage
    search_element.user = um_user_new();
    if (!search_element.user)
    {
        goto error_out;
    }

    // load /etc/passwd data and after that load /etc/shadow data
    setpwent();

    // /etc/passwd
    while ((pwd = getpwent()) != NULL)
    {
        tmp_user = um_user_new();

        if (!tmp_user)
            goto error_out;

        user_set = false;

        // set passwd data
        if (um_user_set_name(tmp_user, pwd->pw_name))
            goto error_out;
        if (um_user_set_password(tmp_user, pwd->pw_passwd))
            goto error_out;
        if (um_user_set_gecos(tmp_user, pwd->pw_gecos))
            goto error_out;
        if (um_user_set_home_path(tmp_user, pwd->pw_dir))
            goto error_out;
        if (um_user_set_shell_path(tmp_user, pwd->pw_shell))
            goto error_out;

        um_user_set_uid(tmp_user, pwd->pw_uid);
        um_user_set_gid(tmp_user, pwd->pw_gid);

        // create element
        tmp_user_element = (um_user_element_t *)malloc(sizeof(um_user_element_t));
        if (!tmp_user_element)
            goto error_out;

        tmp_user_element->user = tmp_user;
        user_set = true;

        // add the user to the list
        LL_APPEND(db->users_head, tmp_user_element);
    }

    passwd_closed = true;
    endpwent();

    setspent();

    // /etc/shadow
    while ((spwd = getspent()) != NULL)
    {
        // get user from the list
        LL_SEARCH(db->users_head, found_element, &search_element, um_user_element_cmp_fn);

        // set shadow data
        if (found_element)
        {
            um_user_t *user = found_element->user;

            if (um_user_set_password_hash(user, spwd->sp_pwdp))
                goto error_out;

            um_user_set_last_change(user, spwd->sp_lstchg);
            um_user_set_change_min(user, spwd->sp_min);
            um_user_set_change_max(user, spwd->sp_max);
            um_user_set_warn_days(user, spwd->sp_warn);
            um_user_set_inactive_days(user, spwd->sp_inact);
            um_user_set_expiration(user, spwd->sp_expire);
            um_user_set_flags(user, spwd->sp_flag);
        }
    }

    shadow_closed = true;
    endspent();

    goto out;

error_out:
    error = -1;
    if (tmp_user && !user_set)
    {
        um_user_free(tmp_user);
    }

out:
    if (!passwd_closed)
    {
        endpwent();
    }

    if (!shadow_closed)
    {
        endspent();
    }

    if (search_element.user)
    {
        um_user_free(search_element.user);
    }

    return error;
}

/**
 * Store user database to the system.
 *
 * @param db Database to store.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_db_store(um_user_db_t *db)
{
    int error = 0;
    FILE *passwd_file = NULL;
    FILE *shadow_file = NULL;

    um_user_element_t *user_iter = NULL;

    passwd_file = fopen("/etc/passwd", "w");
    if (!passwd_file)
    {
        goto error_out;
    }

    shadow_file = fopen("/etc/shadow", "w");
    if (!passwd_file)
    {
        goto error_out;
    }

    LL_FOREACH(db->users_head, user_iter)
    {
        const um_user_t *user = user_iter->user;

        // setup passwd data
        const struct passwd tmp_passwd = (const struct passwd){
            .pw_name = (char *)um_user_get_name(user),
            .pw_passwd = (char *)um_user_get_password(user),
            .pw_dir = (char *)um_user_get_home_path(user),
            .pw_gecos = (char *)um_user_get_gecos(user),
            .pw_shell = (char *)um_user_get_shell_path(user),
            .pw_uid = um_user_get_uid(user),
            .pw_gid = um_user_get_gid(user),
        };

        // setup shadow data
        const struct spwd tmp_spwd = (const struct spwd){
            .sp_namp = (char *)um_user_get_name(user),
            .sp_pwdp = (char *)um_user_get_password_hash(user),
            .sp_lstchg = um_user_get_last_change(user),
            .sp_min = um_user_get_change_min(user),
            .sp_max = um_user_get_change_max(user),
            .sp_expire = um_user_get_expiration(user),
            .sp_flag = um_user_get_flags(user),
            .sp_inact = um_user_get_inactive_days(user),
            .sp_warn = um_user_get_warn_days(user),
        };

        // passwd
        if (putpwent(&tmp_passwd, passwd_file))
        {
            goto error_out;
        }

        // shadow
        if (putspent(&tmp_spwd, shadow_file))
        {
            goto error_out;
        }
    }

error_out:
    error = -1;

out:
    if (passwd_file)
    {
        fclose(passwd_file);
    }

    if (shadow_file)
    {
        fclose(shadow_file);
    }

    return error;
}

/**
 * Get the user from the database.
 *
 * @param db Database to use.
 * @param user_name User to search for.
 *
 * @return Abstract user type - NULL if not found.
 *
 */
um_user_t *um_user_db_get_user(um_user_db_t *db, const char *user_name)
{
    um_user_element_t search_element = {0}, *found_element = NULL;
    um_user_t *user = NULL;

    search_element.user = um_user_new();
    if (!search_element.user)
    {
        return NULL;
    }

    if (um_user_set_name(search_element.user, user_name))
    {
        goto out;
    }

    LL_SEARCH(db->users_head, found_element, &search_element, um_user_element_cmp_fn);

    if (found_element)
    {
        user = found_element->user;
    }

out:
    // free temp user
    um_user_free(search_element.user);

    return user;
}

/**
 * Delete user from the database.
 *
 * @param db Database to use.
 * @param user_name User to search for.
 *
 * @return Error code - 0 on success.
 *
 */
int um_user_db_delete_user(um_user_db_t *db, const char *user_name)
{
    int error = 0;
    um_user_element_t search_element = {0}, *found_element = NULL;

    search_element.user = um_user_new();
    if (!search_element.user)
    {
        goto error_out;
    }

    if (um_user_set_name(search_element.user, user_name))
    {
        goto error_out;
    }

    LL_SEARCH(db->users_head, found_element, &search_element, um_user_element_cmp_fn);

    if (found_element)
    {
        // free data
        um_user_free(found_element->user);

        // remove from list
        LL_DELETE(db->users_head, found_element);
    }

    goto out;

error_out:
    error = -1;

out:
    // free temp user
    um_user_free(search_element.user);

    return error;
}

/**
 * Get users list head.
 *
 * @param db Database to use.
 *
 * @return Users list head.
 *
 */
const um_user_element_t *um_user_db_get_user_list_head(const um_user_db_t *db)
{
    return db->users_head;
}

/**
 * Free user database data.
 *
 * @param db User database to free.
 *
 */
void um_user_db_free(um_user_db_t *db)
{
    um_user_element_t *user_iter = NULL, *temp = NULL;

    LL_FOREACH_SAFE(db->users_head, user_iter, temp)
    {
        um_user_free(user_iter->user);
        LL_DELETE(db->users_head, user_iter);
        free(user_iter);
    }

    free(db);
}

static int um_user_element_cmp_fn(void *d1, void *d2)
{
    um_user_element_t *u1 = d1;
    um_user_element_t *u2 = d2;

    return strcmp(um_user_get_name(u1->user), um_user_get_name(u2->user));
}