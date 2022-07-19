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
#ifndef UMGMT_GROUP_H
#define UMGMT_GROUP_H

#include "types.h"

#define __USE_GNU
#include <grp.h>
#include <gshadow.h>

/**
 * Allocate new group.
 *
 * @return New allocated group.
 */
um_group_t *um_group_new(void);

/**
 * Set the name for the group.
 *
 * @param group Group to use.
 * @param name Name to set.
 *
 * @return Error code - 0 on success.
 *
 */
int um_group_set_name(um_group_t *group, const char *name);

/**
 * Set the password value from /etc/passwd for the group.
 *
 * @param group Group to use.
 * @param password Password value to set.
 *
 * @return Error code - 0 on success.
 *
 */
int um_group_set_password(um_group_t *group, const char *password);

/**
 * Set GID for the group.
 *
 * @param group Group to use.
 * @param gid GID to set.
 *
 */
void um_group_set_gid(um_group_t *group, gid_t gid);

/**
 * Set the password hash for the group.
 *
 * @param group Group to use.
 * @param password_hash Password hash to set.
 *
 * @return Error code - 0 on success.
 *
 */
int um_group_set_password_hash(um_group_t *group, const char *password_hash);

/**
 * Add user to the group member list.
 *
 * @param group Group to use.
 * @param user User to add as a member.
 *
 * @return Error code - 0 on success.
 *
 */
int um_group_add_member(um_group_t *group, const um_user_t *user);

/**
 * Add user to the group admin list.
 *
 * @param group Group to use.
 * @param user User to add as a admin.
 *
 * @return Error code - 0 on success.
 *
 */
int um_group_add_admin(um_group_t *group, const um_user_t *user);

/**
 * Get group name.
 *
 * @param group Group to use.
 *
 * @return Group name.
 *
 */
const char *um_group_get_name(const um_group_t *group);

/**
 * Get group password.
 *
 * @param group Group to use.
 *
 * @return Group's password value from /etc/passwd.
 *
 */
const char *um_group_get_password(const um_group_t *group);

/**
 * Get group GID.
 *
 * @param group Group to use.
 *
 * @return Group GID.
 *
 */
gid_t um_group_get_gid(const um_group_t *group);

/**
 * Get group password hash.
 *
 * @param group Group to use.
 *
 * @return Group's password hash.
 *
 */
const char *um_group_get_password_hash(const um_group_t *group);

/**
 * Get head of group members list.
 *
 * @param group Group to use.
 *
 * @return Group's members list head.
 *
 */
const um_group_user_element_t *um_group_get_members_head(const um_group_t *group);

/**
 * Get head of group admin list.
 *
 * @param group Group to use.
 *
 * @return Group's admin list head.
 *
 */
const um_group_user_element_t *um_group_get_admin_head(const um_group_t *group);

/**
 * Free group data.
 *
 * @param group Group to free.
 *
 */
void um_group_free(um_group_t *group);

#endif // UMGMT_GROUP_H