/*  =========================================================================
    zs_pipe - ZeroScript data pipe

    Copyright (c) the Contributors as noted in the AUTHORS file.
    This file is part of the ZeroScript language, http://zeroscript.org.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#ifndef ZS_PIPE_H_INCLUDED
#define ZS_PIPE_H_INCLUDED

#include <czmq.h>

#ifdef __cplusplus
extern "C" {
#endif

//  Opaque class structure
#ifndef ZS_PIPE_T_DEFINED
typedef struct _zs_pipe_t zs_pipe_t;
#endif

//  @interface
//  Create a new zs_pipe, return the reference if successful, or NULL
//  if construction failed due to lack of available memory.
zs_pipe_t *
    zs_pipe_new (void);

//  Destroy the zs_pipe and free all memory used by the object.
void
    zs_pipe_destroy (zs_pipe_t **self_p);

//  Sets pipe register to contain a specified whole number; any previous
//  value in the register is lost.
void
    zs_pipe_set_whole (zs_pipe_t *self, int64_t whole);

//  Sets pipe register to contain a specified real number; any previous
//  value in the register is lost.
void
    zs_pipe_set_real (zs_pipe_t *self, double real);

//  Sets pipe register to contain a specified string; any previous value
//  in the register is lost.
void
    zs_pipe_set_string (zs_pipe_t *self, const char *string);

//  Sends current pipe register to the pipe; returns 0 if successful, or
//  -1 if the pipe register was empty. Clears the register.
int
    zs_pipe_send (zs_pipe_t *self);

//  Send whole number to pipe; this wipes the current pipe register.
void
    zs_pipe_send_whole (zs_pipe_t *self, int64_t whole);

//  Send real number to pipe; this wipes the current pipe register.
void
    zs_pipe_send_real (zs_pipe_t *self, double real);

//  Send string to pipe; this wipes the current pipe register.
void
    zs_pipe_send_string (zs_pipe_t *self, const char *string);

//  Receives the next value off the pipe, into the register. Any previous
//  value in the register is lost. Returns 0 if a value was successfully
//  received. If no values were received, returns -1. This method does not
//  block.
int
    zs_pipe_recv (zs_pipe_t *self);

//  Returns the type of the register, 'w' for whole, 'r' for real, or 's' for
//  string. Returns -1 if the register is empty.
char
    zs_pipe_type (zs_pipe_t *self);

//  Returns the value of the register, coerced to a whole number. This can
//  cause loss of precision. If no conversion was possible, or the register
//  is empty, returns zero.
int64_t
    zs_pipe_whole (zs_pipe_t *self);

//  Returns the value of the register, coerced to a real number. This can
//  cause loss of precision. If no conversion was possible, or the register
//  is empty, returns zero.
double
    zs_pipe_real (zs_pipe_t *self);

//  Returns the value of the register, coerced to a string if needed. If the
//  register is empty, returns an empty string "". The caller must not modify
//  or free the string.
char *
    zs_pipe_string (zs_pipe_t *self);

//  Receives the next value off the pipe, into the register, and coerces it
//  to a whole if needed. If there is no value to receive, returns 0.
int64_t
    zs_pipe_recv_whole (zs_pipe_t *self);

//  Receives the next value off the pipe, into the register, and coerces it
//  to a real if needed. If there is no value to receive, returns 0.
double
    zs_pipe_recv_real (zs_pipe_t *self);

//  Receives the next value off the pipe, into the register, and coerces it
//  to a string if needed. If there is no value to receive, returns "". The
//  The caller must not modify or free the string.
char *
    zs_pipe_recv_string (zs_pipe_t *self);

//  Marks an end of phrase in the pipe. This is used to delimit the pipe
//  as input for later function calls. Marks are ignored when receiving
//  values off a pipe.
void
    zs_pipe_mark (zs_pipe_t *self);

//  Pulls a list of values from the source pipe into the pipe. The pull
//  algorithm works depending on whether the pipe is at the end of a
//  phrase or not, and whether the greedy argument is true or false:
//
//                  End of phrase:      In phrase:
//  Greedy:         pull entire pipe    pull current phrase
//  Not greedy:     pull last phrase    pull last single value
//
//  Any existing values in the pipe are first removed. This implements
//  the necessary pipe mechanics for modest and greedy functions.
void
    zs_pipe_pull (zs_pipe_t *self, zs_pipe_t *source, bool greedy);

//  Return pipe contents, as string. Caller must free it when done. Values are
//  separated by spaces. This empties the pipe.
char *
    zs_pipe_paste (zs_pipe_t *self);

//  Empty the pipe of any values it might contain.
void
    zs_pipe_purge (zs_pipe_t *self);

//  Self test of this class
void
    zs_pipe_test (bool animate);
//  @end

#ifdef __cplusplus
}
#endif

#endif