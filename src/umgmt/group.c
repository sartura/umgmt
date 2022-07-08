#include "group.h"

#include <string.h>
#include <stdlib.h>

#include <utlist.h>

typedef struct um_gshadow_data_s um_gshadow_data_t;

struct um_gshadow_data_s
{
    char *password_hash;
    um_group_user_element_t *members_head;
    um_group_user_element_t *admin_head;
};

struct um_group_s
{
    char *name;
    char *password;
    gid_t gid;
    um_gshadow_data_t gshadow;
};

/**
 * Allocate new group.
 *
 * @return New allocated group.
 */
um_group_t *um_group_new(void)
{
    um_group_t *new_group = (um_group_t *)malloc(sizeof(um_group_t));

    if (!new_group)
    {
        return NULL;
    }

    *new_group = (um_group_t){0};

    return new_group;
}

/**
 * Set the name for the group.
 *
 * @param group Group to use.
 * @param name Name to set.
 *
 * @return Error code - 0 on success.
 *
 */
int um_group_set_name(um_group_t *group, const char *name)
{
    if (group->name)
    {
        free(group->name);
        group->name = 0;
    }

    if (name)
    {
        group->name = strdup(name);
        if (!group->name)
        {
            return -1;
        }
    }

    return 0;
}

/**
 * Set the password value from /etc/passwd for the group.
 *
 * @param group Group to use.
 * @param password Password value to set.
 *
 * @return Error code - 0 on success.
 *
 */
int um_group_set_password(um_group_t *group, const char *password)
{
    if (group->password)
    {
        free(group->password);
        group->password = 0;
    }

    if (password)
    {
        group->password = strdup(password);
        if (!group->password)
        {
            return -1;
        }
    }

    return 0;
}

/**
 * Set GID for the group.
 *
 * @param group Group to use.
 * @param gid GID to set.
 *
 */
void um_group_set_gid(um_group_t *group, gid_t gid)
{
    group->gid = gid;
}

/**
 * Set the password hash for the group.
 *
 * @param group Group to use.
 * @param password_hash Password hash to set.
 *
 * @return Error code - 0 on success.
 *
 */
int um_group_set_password_hash(um_group_t *group, const char *password_hash)
{
    if (group->gshadow.password_hash)
    {
        free(group->gshadow.password_hash);
        group->gshadow.password_hash = 0;
    }

    if (password_hash)
    {
        group->gshadow.password_hash = strdup(password_hash);
        if (!group->gshadow.password_hash)
        {
            return -1;
        }
    }

    return 0;
}

/**
 * Add user to the group member list.
 *
 * @param group Group to use.
 * @param user User to add as a member.
 *
 * @return Error code - 0 on success.
 *
 */
int um_group_add_member(um_group_t *group, const um_user_t *user)
{
    um_group_user_element_t *new_element = (um_group_user_element_t *)malloc(sizeof(um_group_user_element_t));

    if (!new_element)
    {
        return -1;
    }

    new_element->user = user;
    new_element->next = NULL;

    // add to both lists
    LL_APPEND(group->gshadow.members_head, new_element);

    return 0;
}

/**
 * Add user to the group admin list.
 *
 * @param group Group to use.
 * @param user User to add as a admin.
 *
 * @return Error code - 0 on success.
 *
 */
int um_group_add_admin(um_group_t *group, const um_user_t *user)
{
    um_group_user_element_t *new_element = (um_group_user_element_t *)malloc(sizeof(um_group_user_element_t));

    if (!new_element)
    {
        return -1;
    }

    new_element->user = user;
    new_element->next = NULL;

    LL_APPEND(group->gshadow.admin_head, new_element);

    return 0;
}

/**
 * Get group name.
 *
 * @param group Group to use.
 *
 * @return Group name.
 *
 */
const char *um_group_get_name(const um_group_t *group)
{
    return group->name;
}

/**
 * Get group password.
 *
 * @param group Group to use.
 *
 * @return Group's password value from /etc/passwd.
 *
 */
const char *um_group_get_password(const um_group_t *group)
{
    return group->password;
}

/**
 * Get group GID.
 *
 * @param group Group to use.
 *
 * @return Group GID.
 *
 */
gid_t um_group_get_gid(const um_group_t *group)
{
    return group->gid;
}

/**
 * Get group password hash.
 *
 * @param group Group to use.
 *
 * @return Group's password hash.
 *
 */
const char *um_group_get_password_hash(const um_group_t *group)
{
    return group->gshadow.password_hash;
}

/**
 * Get head of group members list.
 *
 * @param group Group to use.
 *
 * @return Group's members list head.
 *
 */
const um_group_user_element_t *um_group_get_members_head(const um_group_t *group)
{
    return group->gshadow.members_head;
}

/**
 * Get head of group admin list.
 *
 * @param group Group to use.
 *
 * @return Group's admin list head.
 *
 */
const um_group_user_element_t *um_group_get_admin_head(const um_group_t *group)
{
    return group->gshadow.admin_head;
}

/**
 * Free group data.
 *
 * @param group Group to free.
 *
 */
void um_group_free(um_group_t *group)
{
    um_group_user_element_t *iter = NULL, *tmp = NULL;

    if (group->name)
    {
        free(group->name);
    }

    if (group->password)
    {
        free(group->password);
    }

    if (group->gshadow.password_hash)
    {
        free(group->gshadow.password_hash);
    }

    LL_FOREACH_SAFE(group->gshadow.members_head, iter, tmp)
    {
        LL_DELETE(group->gshadow.members_head, iter);
        free(iter);
    }

    LL_FOREACH_SAFE(group->gshadow.admin_head, iter, tmp)
    {
        LL_DELETE(group->gshadow.admin_head, iter);
        free(iter);
    }

    free(group);
}