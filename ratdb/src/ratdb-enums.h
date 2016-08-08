/* this file is part of RatDB */
/**
 * Copyright (c) 2012 Mike Manilone
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS `AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef __RATDB_ENUMS_H__
#define __RATDB_ENUMS_H__

/**
 * RatdbNewDBFlags:
 * @RATDB_NEWDB_ANYWAY: Create it or load it
 * @RATDB_NEWDB_REPLACE: Replace and create it
 * @RATDB_NEWDB_OPEN: Open it; fails when it doesn't exist
 * @RATDB_NEWDB_CREATE: Create it; failes when it exists
 */
typedef enum {
    RATDB_NEWDB_ANYWAY,
    RATDB_NEWDB_REPLACE,
    RATDB_NEWDB_OPEN,
    RATDB_NEWDB_CREATE
} RatdbNewDBFlags;

/**
 * RatdbMatch:
 * @RATDB_MATCH_SUCCESS: Found
 * @RATDB_MATCH_NOTFOUND: Not found
 * @RATDB_MATCH_TOOMANY: There are too many results, but the functions needs only a few (or only one)
 */
typedef enum {
    RATDB_MATCH_NOTFOUND,
    RATDB_MATCH_SUCCESS,
    RATDB_MATCH_TOOMANY
} RatdbMatch;

#endif 
