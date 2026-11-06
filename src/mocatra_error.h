#ifndef _MOCATRA_ERROR_H
#define _MOCATRA_ERROR_H

typedef enum {
        MOCATRA_OK,
        MOCATRA_ERROR,
        MOCATRA_INVSETTINGS, /**< invalid render settings - new mocatra */
        MOCATRA_INVARG,
        MOCATRA_NOMEM,
} mocatra_error_t;

#endif /* _MOCATRA_ERROR_H */
