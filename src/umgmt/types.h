/**
 * @file types.h
 * @brief Abstract data type definitions.
 */
#ifndef UMGMT_TYPES_H
#define UMGMT_TYPES_H

/**
 * Abstract user type - containing information from /etc/passwd and /etc/shadow.
 */
typedef struct um_user_s um_user_t;

/**
 * User list element.
 */
typedef struct um_user_element_s um_user_element_t;

/**
 * Abstract user database type.
 */
typedef struct um_user_db_s um_user_db_t;

/**
 * User list element.
 */
struct um_user_element_s
{
    um_user_t *user;         ///< Allocated abstract user data type.
    um_user_element_t *next; ///< Link to the next list node.
};

#endif // UMGMT_TYPES_H