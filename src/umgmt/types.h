/**
 * @file types.h
 * @brief Abstract data type definitions.
 *
 * Copyright (c) 2022 Sartura Ltd.
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */
#ifndef UMGMT_TYPES_H
#define UMGMT_TYPES_H

/**
 * Abstract user type - containing information from /etc/passwd and /etc/shadow.
 */
typedef struct um_user_s um_user_t;

/**
 * Abstract group type - containing information from /etc/group and /etc/gshadow.
 */
typedef struct um_group_s um_group_t;

/**
 * Group list element.
 */
typedef struct um_group_element_s um_group_element_t;

/**
 * Group member/admin list element.
 */
typedef struct um_group_user_element_s um_group_user_element_t;

/**
 * User list element.
 */
typedef struct um_user_element_s um_user_element_t;

/**
 * Abstract user database type.
 */
typedef struct um_user_db_s um_user_db_t;

/**
 * Abstract group database type.
 */
typedef struct um_group_db_s um_group_db_t;

/**
 * Abstract database type - contains all users and groups found on the system.
 */
typedef struct um_db_s um_db_t;

/**
 * User list element.
 */
struct um_user_element_s
{
    um_user_t *user;         ///< Allocated abstract user data type.
    um_user_element_t *next; ///< Link to the next list node.
};

/**
 * Group list element.
 */
struct um_group_element_s
{
    um_group_t *group;        ///< Allocated abstract group data type.
    um_group_element_t *next; ///< Link to the next list node.
};

/**
 * Group member/admin list element.
 */
struct um_group_user_element_s
{
    const um_user_t *user;         ///< Allocated abstract user data type - single member.
    um_group_user_element_t *next; ///< Link to the next list node.
};

#endif // UMGMT_TYPES_H