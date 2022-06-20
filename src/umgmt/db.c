#include "db.h"
#include "user.h"
#include "group.h"

#include <gshadow.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <utlist.h>
#include <stdbool.h>

struct um_db_s
{
    um_user_element_t *user_head;
    um_group_element_t *group_head;
};

static int um_user_element_cmp_fn(void *d1, void *d2);
static int um_group_element_cmp_fn(void *d1, void *d2);

/**
 * Allocate new database.
 *
 * @return New allocated database.
 *
 */
um_db_t *um_db_new(void)
{
    um_db_t *new_db = (um_db_t *)malloc(sizeof(um_db_t));

    if (!new_db)
    {
        return NULL;
    }

    *new_db = (um_db_t){0};

    return new_db;
}

/**
 * Load database from the system.
 *
 * @param db Database to load.
 *
 * @return Error code - 0 on success.
 *
 */
int um_db_load(um_db_t *db)
{
    int error = 0;

    // temp data
    um_user_t *tmp_user = NULL;
    um_user_element_t *tmp_user_element = NULL, user_search_element = {0}, *user_found_element = NULL;
    um_group_t *tmp_group = NULL;
    um_group_element_t *tmp_group_element = NULL, group_search_element = {0}, *group_found_element = NULL;

    // make sure no memory leak occurs
    bool user_set = false;
    bool group_set = false;

    // passwd and shadow
    struct passwd *pwd = NULL;
    struct spwd *spwd = NULL;
    struct group *grp = NULL;
    struct sgrp *sgrp = NULL;

    // files closed
    bool passwd_closed = false, shadow_closed = false, gpasswd_closed = false, gshadow_closed = false;

    // allocate search element data for later usage
    user_search_element.user = um_user_new();
    if (!user_search_element.user)
    {
        goto error_out;
    }

    group_search_element.group = um_group_new();
    if (!group_search_element.group)
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
        LL_APPEND(db->user_head, tmp_user_element);
    }

    passwd_closed = true;
    endpwent();

    setspent();

    // /etc/shadow
    while ((spwd = getspent()) != NULL)
    {
        // get user from the list
        error = um_user_set_name(user_search_element.user, spwd->sp_namp);
        if (error)
        {
            goto error_out;
        }

        user_found_element = NULL;
        LL_SEARCH(db->user_head, user_found_element, &user_search_element, um_user_element_cmp_fn);

        // set shadow data
        if (user_found_element)
        {
            um_user_t *user = user_found_element->user;

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

    setgrent();

    // /etc/group
    while ((grp = getgrent()) != NULL)
    {
        tmp_group = um_group_new();

        if (!tmp_group)
            goto error_out;

        group_set = false;

        // set passwd data
        if (um_group_set_name(tmp_group, grp->gr_name))
            goto error_out;
        if (um_group_set_password(tmp_group, grp->gr_passwd))
            goto error_out;

        um_group_set_gid(tmp_group, grp->gr_gid);

        // create element
        tmp_group_element = (um_group_element_t *)malloc(sizeof(um_group_element_t));
        if (!tmp_group_element)
            goto error_out;

        tmp_group_element->group = tmp_group;
        group_set = true;

        // add the group to the list
        LL_APPEND(db->group_head, tmp_group_element);
    }

    gpasswd_closed = true;
    endgrent();

    setsgent();

    // /etc/gshadow
    while ((sgrp = getsgent()) != NULL)
    {
        // get group from the list
        error = um_group_set_name(group_search_element.group, sgrp->sg_namp);
        if (error)
        {
            goto error_out;
        }

        group_found_element = NULL;
        LL_SEARCH(db->group_head, group_found_element, &group_search_element, um_group_element_cmp_fn);

        // set shadow data
        if (group_found_element)
        {
            um_group_t *group = group_found_element->group;

            if (um_group_set_password_hash(group, sgrp->sg_passwd))
                goto error_out;

            // add admin and member lists

            for (int i = 0; sgrp->sg_mem[i] != NULL; i++)
            {
                const char *member = sgrp->sg_mem[i];

                error = um_user_set_name(user_search_element.user, member);
                if (error)
                {
                    goto error_out;
                }

                LL_SEARCH(db->user_head, user_found_element, &user_search_element, um_user_element_cmp_fn);

                if (user_found_element)
                {
                    error = um_group_add_member(group, user_found_element->user);
                    if (error)
                    {
                        goto error_out;
                    }
                }
            }

            for (int i = 0; sgrp->sg_adm[i] != NULL; i++)
            {
                const char *admin = sgrp->sg_adm[i];

                error = um_user_set_name(user_search_element.user, admin);
                if (error)
                {
                    goto error_out;
                }

                user_found_element = NULL;
                LL_SEARCH(db->user_head, user_found_element, &user_search_element, um_user_element_cmp_fn);

                if (user_found_element)
                {
                    error = um_group_add_admin(group, user_found_element->user);
                    if (error)
                    {
                        goto error_out;
                    }
                }
            }
        }
    }

    gshadow_closed = true;
    endsgent();

    goto out;

error_out:
    error = -1;
    if (tmp_user && !user_set)
    {
        um_user_free(tmp_user);
    }
    if (tmp_group && !group_set)
    {
        um_group_free(tmp_group);
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

    if (!gpasswd_closed)
    {
        endgrent();
    }

    if (!gshadow_closed)
    {
        endsgent();
    }

    if (user_search_element.user)
    {
        um_user_free(user_search_element.user);
    }
    if (group_search_element.group)
    {
        um_group_free(group_search_element.group);
    }

    return error;
}

/**
 * Store database to the system.
 *
 * @param db Database to store.
 *
 * @return Error code - 0 on success.
 *
 */
int um_db_store(um_db_t *db)
{
    int error = 0;
    FILE *passwd_file = NULL;
    FILE *shadow_file = NULL;
    FILE *gpasswd_file = NULL;
    FILE *gshadow_file = NULL;

    um_user_element_t *user_iter = NULL;
    um_group_element_t *group_iter = NULL;
    const um_group_user_element_t *tmp_group_user = NULL;

    const char **members = NULL;
    const char **admins = NULL;
    bool members_allocated = false;
    bool admins_allocated = false;

    int members_count = 0;
    int admins_count = 0;

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

    gpasswd_file = fopen("/etc/group", "w");
    if (!gpasswd_file)
    {
        goto error_out;
    }

    gshadow_file = fopen("/etc/gshadow", "w");
    if (!gpasswd_file)
    {
        goto error_out;
    }

    LL_FOREACH(db->user_head, user_iter)
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

    LL_FOREACH(db->group_head, group_iter)
    {
        const um_group_t *group = group_iter->group;

        // get admins and members count
        LL_COUNT(um_group_get_members_head(group), tmp_group_user, members_count);
        LL_COUNT(um_group_get_admin_head(group), tmp_group_user, admins_count);

        // allocate string[] for members and admins
        members_allocated = false;
        admins_allocated = false;

        members = malloc(sizeof(const char *) * (members_count + 1));
        if (!members)
        {
            goto error_out;
        }
        members_allocated = true;

        admins = malloc(sizeof(const char *) * (admins_count + 1));
        if (!admins)
        {
            goto error_out;
        }
        admins_allocated = true;

        // setup members and admins
        int i = 0;

        LL_FOREACH(um_group_get_members_head(group), tmp_group_user)
        {
            members[i] = um_user_get_name(tmp_group_user->user);
            ++i;
        }

        // end list
        members[i] = 0;

        i = 0;
        LL_FOREACH(um_group_get_admin_head(group), tmp_group_user)
        {
            admins[i] = um_user_get_name(tmp_group_user->user);
            ++i;
        }

        // end list
        admins[i] = 0;

        const struct group tmp_group = (const struct group){
            .gr_name = (char *)um_group_get_name(group),
            .gr_passwd = (char *)um_group_get_password(group),
            .gr_gid = um_group_get_gid(group),
            .gr_mem = (char **)members,
        };

        const struct sgrp tmp_sgrp = (const struct sgrp){
            .sg_namp = (char *)um_group_get_name(group),
            .sg_passwd = (char *)um_group_get_password_hash(group),
            .sg_adm = (char **)admins,
            .sg_mem = (char **)members,
        };

        if (putgrent(&tmp_group, gpasswd_file))
        {
            goto error_out;
        }

        if (putsgent(&tmp_sgrp, gshadow_file))
        {
            goto error_out;
        }

        // free members and admins string[]
        free(members);
        free(admins);
        members_allocated = false;
        admins_allocated = false;
    }

    goto out;

error_out:
    error = -1;

out:
    if (members && members_allocated)
    {
        free(members);
    }

    if (admins && admins_allocated)
    {
        free(admins);
    }

    if (passwd_file)
    {
        fclose(passwd_file);
    }

    if (shadow_file)
    {
        fclose(shadow_file);
    }

    if (gpasswd_file)
    {
        fclose(gpasswd_file);
    }

    if (gshadow_file)
    {
        fclose(gshadow_file);
    }

    return error;
}

/**
 * Return the new UID which can be used for a new user.
 *
 * @param db Database to use.
 *
 * @return New UID.
 *
 */
uid_t um_db_get_new_uid(um_db_t *db)
{
    um_user_element_t *user_iter = NULL;
    gid_t max_uid = 0;

    LL_FOREACH(db->user_head, user_iter)
    {
        if (um_user_get_uid(user_iter->user) >= 1000 && um_user_get_uid(user_iter->user) < 65534 &&
            um_user_get_uid(user_iter->user) > max_uid)
        {
            max_uid = um_user_get_uid(user_iter->user);
        }
    }

    return max_uid + 1;
}

/**
 * Return the new GID which can be used for a new user/group.
 *
 * @param db Database to use.
 *
 * @return New GID.
 *
 */
gid_t um_db_get_new_gid(um_db_t *db)
{
    um_user_element_t *user_iter = NULL;
    gid_t max_gid = 0;

    LL_FOREACH(db->user_head, user_iter)
    {
        if (um_user_get_uid(user_iter->user) >= 1000 && um_user_get_uid(user_iter->user) < 65534 &&
            um_user_get_gid(user_iter->user) > max_gid)
        {
            max_gid = um_user_get_gid(user_iter->user);
        }
    }

    return max_gid + 1;
}

/**
 * Add a new user to the database.
 * User will be handled by the database from this point on - do not free user data after adding user to the database.
 *
 * @param db Database to use.
 * @param name User to search for.
 *
 * @return Error code - 0 on success.
 *
 */
int um_db_add_user(um_db_t *db, um_user_t *user)
{
    um_user_element_t *new_user = NULL;

    new_user = (um_user_element_t *)malloc(sizeof(um_user_element_t));
    if (!new_user)
    {
        // free user data immediately
        um_user_free(user);
        return -1;
    }

    new_user->user = user;
    new_user->next = NULL;

    LL_APPEND(db->user_head, new_user);

    return 0;
}

/**
 * Add a new group to the database.
 * Group will be handled by the database from this point on - do not free group data after adding user to the database.
 *
 * @param db Database to use.
 * @param name Group to add.
 *
 * @return Error code - 0 on success.
 *
 */
int um_db_add_group(um_db_t *db, um_group_t *group)
{
    um_group_element_t *new_group = NULL;

    new_group = (um_group_element_t *)malloc(sizeof(um_group_element_t));
    if (!new_group)
    {
        // free user data immediately
        um_group_free(group);
        return -1;
    }

    new_group->group = group;
    new_group->next = NULL;

    LL_APPEND(db->group_head, new_group);

    return 0;
}

/**
 * Get the user from the database.
 *
 * @param db Database to use.
 * @param name User to search for.
 *
 * @return Abstract user type - NULL if not found.
 *
 */
um_user_t *um_db_get_user(um_db_t *db, const char *name)
{
    um_user_element_t search_element = {0}, *found_element = NULL;
    um_user_t *user = NULL;

    search_element.user = um_user_new();
    if (!search_element.user)
    {
        return NULL;
    }

    if (um_user_set_name(search_element.user, name))
    {
        goto out;
    }

    LL_SEARCH(db->user_head, found_element, &search_element, um_user_element_cmp_fn);

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
 * Get the group from the database.
 *
 * @param db Database to use.
 * @param name Group to search for.
 *
 * @return Abstract group type - NULL if not found.
 *
 */
um_group_t *um_db_get_group(um_db_t *db, const char *name)
{
    um_group_element_t search_element = {0}, *found_element = NULL;
    um_group_t *group = NULL;

    search_element.group = um_group_new();
    if (!search_element.group)
    {
        return NULL;
    }

    if (um_group_set_name(search_element.group, name))
    {
        goto out;
    }

    LL_SEARCH(db->group_head, found_element, &search_element, um_group_element_cmp_fn);

    if (found_element)
    {
        group = found_element->group;
    }

out:
    // free temp user
    um_group_free(search_element.group);

    return group;
}

/**
 * Delete user from the database.
 *
 * @param db Database to use.
 * @param name User to search for.
 *
 * @return Error code - 0 on success.
 *
 */
int um_db_delete_user(um_db_t *db, const char *name)
{
    int error = 0;
    um_user_element_t search_element = {0}, *found_element = NULL;

    search_element.user = um_user_new();
    if (!search_element.user)
    {
        goto error_out;
    }

    if (um_user_set_name(search_element.user, name))
    {
        goto error_out;
    }

    LL_SEARCH(db->user_head, found_element, &search_element, um_user_element_cmp_fn);

    if (found_element)
    {
        // free data
        um_user_free(found_element->user);

        // remove from list
        LL_DELETE(db->user_head, found_element);
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
 * Delete group from the database.
 *
 * @param db Database to use.
 * @param name Group to search for.
 *
 * @return Error code - 0 on success.
 *
 */
int um_db_delete_group(um_db_t *db, const char *name)
{
    int error = 0;
    um_group_element_t search_element = {0}, *found_element = NULL;

    search_element.group = um_group_new();
    if (!search_element.group)
    {
        goto error_out;
    }

    if (um_group_set_name(search_element.group, name))
    {
        goto error_out;
    }

    LL_SEARCH(db->group_head, found_element, &search_element, um_group_element_cmp_fn);

    if (found_element)
    {
        // free data
        um_group_free(found_element->group);

        // remove from list
        LL_DELETE(db->group_head, found_element);
    }

    goto out;

error_out:
    error = -1;

out:
    // free temp user
    um_group_free(search_element.group);

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
const um_user_element_t *um_db_get_user_list_head(const um_db_t *db)
{
    return db->user_head;
}

/**
 * Get group list head.
 *
 * @param db Database to use.
 *
 * @return Group list head.
 *
 */
const um_group_element_t *um_db_get_group_list_head(const um_db_t *db)
{
    return db->group_head;
}

/**
 * Free database data.
 *
 * @param db Database to free.
 *
 */
void um_db_free(um_db_t *db)
{
    um_user_element_t *user_iter = NULL, *temp_user = NULL;
    um_group_element_t *group_iter = NULL, *temp_group = NULL;

    LL_FOREACH_SAFE(db->user_head, user_iter, temp_user)
    {
        um_user_free(user_iter->user);
        LL_DELETE(db->user_head, user_iter);
        free(user_iter);
    }

    LL_FOREACH_SAFE(db->group_head, group_iter, temp_group)
    {
        um_group_free(group_iter->group);
        LL_DELETE(db->group_head, group_iter);
        free(group_iter);
    }

    free(db);
}

static int um_user_element_cmp_fn(void *d1, void *d2)
{
    um_user_element_t *u1 = d1;
    um_user_element_t *u2 = d2;

    return strcmp(um_user_get_name(u1->user), um_user_get_name(u2->user));
}

static int um_group_element_cmp_fn(void *d1, void *d2)
{
    um_group_element_t *g1 = d1;
    um_group_element_t *g2 = d2;

    return strcmp(um_group_get_name(g1->group), um_group_get_name(g2->group));
}