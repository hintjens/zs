/*  =========================================================================
    zs_pipe - ZeroScript data pipe

    Copyright (c) the Contributors as noted in the AUTHORS file.
    This file is part of the ZeroScript language, http://zeroscript.org.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

/*
@header
    A pipe is an ordered list of strings and wholes.
@discuss
@end
*/

#include "zs_classes.h"
#include "zs_strtod.c"


//  This holds an item in our value queue
typedef struct {
    int64_t whole;
    double real;
    char *string;               //  Points to byte after 'type'
    char type;                  //  Keep this at the end of the structure
} value_t;

//  Structure of our class
struct _zs_pipe_t {
    zlistx_t *values;           //  Simple stupid implementation
    value_t *value;             //  Register value, if any
    char string_value [30];     //  Register value as string
};

static value_t *
s_value_new (const char *string)
{
    value_t *self = (value_t *) zmalloc (sizeof (value_t) + (string? strlen (string) + 1: 0));
    return self;
}

static void
s_value_destroy (value_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        value_t *self = *self_p;
        free (self);
        *self_p = NULL;
    }
}


//  ---------------------------------------------------------------------------
//  Create a new zs_pipe, return the reference if successful, or NULL
//  if construction failed due to lack of available memory.

zs_pipe_t *
zs_pipe_new (void)
{
    zs_pipe_t *self = (zs_pipe_t *) zmalloc (sizeof (zs_pipe_t));
    if (self) {
        self->values = zlistx_new ();
        zlistx_set_destructor (self->values, (czmq_destructor *) s_value_destroy);
    }
    return self;
}


//  ---------------------------------------------------------------------------
//  Destroy the zs_pipe and free all memory used by the object.

void
zs_pipe_destroy (zs_pipe_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        zs_pipe_t *self = *self_p;
        zlistx_destroy (&self->values);
        s_value_destroy (&self->value);
        free (self);
        *self_p = NULL;
    }
}


//  ---------------------------------------------------------------------------
//  Sets pipe register to contain a specified whole number; any previous
//  value in the register is lost.

void
zs_pipe_set_whole (zs_pipe_t *self, int64_t whole)
{
    if (!self->value)
        self->value = s_value_new (NULL);
    self->value->type = 'w';
    self->value->whole = whole;
}


//  ---------------------------------------------------------------------------
//  Sets pipe register to contain a specified real number; any previous
//  value in the register is lost.

void
zs_pipe_set_real (zs_pipe_t *self, double real)
{
    if (!self->value)
        self->value = s_value_new (NULL);

    self->value->type = 'r';
    self->value->real = real;
}


//  ---------------------------------------------------------------------------
//  Sets pipe register to contain a specified string; any previous value
//  in the register is lost.

void
zs_pipe_set_string (zs_pipe_t *self, const char *string)
{
    if (!self->value
    ||  self->value->type != 's'
    ||  strlen (self->value->string) < strlen (string)) {
        s_value_destroy (&self->value);
        self->value = s_value_new (string);
        self->value->type = 's';
        self->value->string = &self->value->type + 1;
    }
    strcpy (self->value->string, string);
}


//  ---------------------------------------------------------------------------
//  Sends current pipe register to the pipe; returns 0 if successful, or
//  -1 if the pipe register was empty. Clears the register.

int
zs_pipe_send (zs_pipe_t *self)
{
    if (self->value) {
        zlistx_add_end (self->values, self->value);
        self->value = NULL;
        return 0;
    }
    else
        return -1;
}


//  ---------------------------------------------------------------------------
//  Send whole number to pipe; this wipes the current pipe register.

void
zs_pipe_send_whole (zs_pipe_t *self, int64_t whole)
{
    zs_pipe_set_whole (self, whole);
    zs_pipe_send (self);
}


//  ---------------------------------------------------------------------------
//  Send real number to pipe; this wipes the current pipe register.

void
zs_pipe_send_real (zs_pipe_t *self, double real)
{
    zs_pipe_set_real (self, real);
    zs_pipe_send (self);
}


//  ---------------------------------------------------------------------------
//  Send string to pipe; this wipes the current pipe register.

void
zs_pipe_send_string (zs_pipe_t *self, const char *string)
{
    zs_pipe_set_string (self, string);
    zs_pipe_send (self);
}


//  ---------------------------------------------------------------------------
//  Receives the next value off the pipe, into the register. Any previous
//  value in the register is lost. Returns 0 if a value was successfully
//  received. If the pipe was empty, returns -1. This method does not block.

int
zs_pipe_recv (zs_pipe_t *self)
{
    //  Skip any marks
    while (true) {
        s_value_destroy (&self->value);
        self->value = (value_t *) zlistx_detach (self->values, NULL);
        if (!self->value)
            return -1;          //  Pipe is empty
        else
        if (isalpha (self->value->type))
            return 0;           //  We had a normal value
    }
}


//  ---------------------------------------------------------------------------
//  Returns the type of the register, 'w' for whole, 'r' for real, or 's' for
//  string. Returns -1 if the register is empty.

char
zs_pipe_type (zs_pipe_t *self)
{
    return self->value? self->value->type: -1;
}


//  ---------------------------------------------------------------------------
//  Returns the value of the register, coerced to a whole number. This can
//  cause loss of precision. If no conversion was possible, or the register
//  is empty, returns zero.

int64_t
zs_pipe_whole (zs_pipe_t *self)
{
    if (self->value) {
        if (self->value->type == 'w')
            return self->value->whole;
        else
        if (self->value->type == 'r')
            return self->value->real > 0? (int64_t) (self->value->real + 0.5): (int64_t) (self->value->real - 0.5);
        else
        if (self->value->type == 's') {
            errno = 0;
            int64_t whole = (int64_t) strtoll (self->value->string, NULL, 10);
            return errno == 0? whole: 0;
        }
    }
    return 0;
}


//  ---------------------------------------------------------------------------
//  Returns the value of the register, coerced to a real number. This can
//  cause loss of precision. If no conversion was possible, or the register
//  is empty, returns zero.

double
zs_pipe_real (zs_pipe_t *self)
{
    if (self->value) {
        if (self->value->type == 'w')
            return (double) self->value->whole;
        else
        if (self->value->type == 'r')
            return self->value->real;
        else
        if (self->value->type == 's') {
            char *end = self->value->string;
            double real = zs_strtod (self->value->string, &end);
            return end > self->value->string? real: 0;
        }
    }
    return 0;
}


//  ---------------------------------------------------------------------------
//  Returns the value of the register, coerced to a string if needed. If the
//  register is empty, returns an empty string "". The caller must not modify
//  or free the string.

char *
zs_pipe_string (zs_pipe_t *self)
{
    if (self->value) {
        if (self->value->type == 'w') {
            snprintf (self->string_value, sizeof (self->string_value),
                      "%" PRId64, self->value->whole);
            return self->string_value;
        }
        else
        if (self->value->type == 'r') {
            snprintf (self->string_value, sizeof (self->string_value),
                      "%g", self->value->real);
            return self->string_value;
        }
        else
        if (self->value->type == 's')
            return self->value->string;
    }
    return "";
}


//  ---------------------------------------------------------------------------
//  Receives the next value off the pipe, into the register, and coerces it
//  to a whole if needed. If there is no value to receive, returns 0.

int64_t
zs_pipe_recv_whole (zs_pipe_t *self)
{
    if (zs_pipe_recv (self) == 0)
        return zs_pipe_whole (self);
    else
        return 0;
}


//  ---------------------------------------------------------------------------
//  Receives the next value off the pipe, into the register, and coerces it
//  to a real if needed. If there is no value to receive, returns 0.

double
zs_pipe_recv_real (zs_pipe_t *self)
{
    if (zs_pipe_recv (self) == 0)
        return zs_pipe_real (self);
    else
        return 0;
}


//  ---------------------------------------------------------------------------
//  Receives the next value off the pipe, into the register, and coerces it
//  to a string if needed. If there is no value to receive, returns "". The
//  The caller must not modify or free the string.

char *
zs_pipe_recv_string (zs_pipe_t *self)
{
    if (zs_pipe_recv (self) == 0)
        return zs_pipe_string (self);
    else
        return 0;
}


//  ---------------------------------------------------------------------------
//  Marks an end of phrase in the pipe. This is used to delimit the pipe
//  as input for later function calls. Marks are ignored when receiving
//  values off a pipe.

void
zs_pipe_mark (zs_pipe_t *self)
{
    value_t *value = s_value_new (NULL);
    value->type = '|';          //  Non-alphabetic
    zlistx_add_end (self->values, value);
}


//  ---------------------------------------------------------------------------
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
zs_pipe_pull (zs_pipe_t *self, zs_pipe_t *source, bool greedy)
{
    zlistx_purge (self->values);

    value_t *value = (value_t *) zlistx_last (source->values);
    if (!value)
        return;                 //  Nothing to do

    if (value->type == '|') {
        if (greedy)
            //  Pull entire pipe
            value = (value_t *) zlistx_first (source->values);
        else {
            //  Pull last phrase; skip back until we hit the start of the
            //  pipe or a mark (before the current mark)
            value = (value_t *) zlistx_prev (source->values);
            while (value && value->type != '|')
                value = (value_t *) zlistx_prev (source->values);
        }
    }
    else
    if (greedy) {
        //  Pull current phrase; skip back until we hit the start of
        //  pipe or a mark
        while (value && value->type != '|')
            value = (value_t *) zlistx_prev (source->values);
    }
    //  Ensure cursor points to first valid value, if any
    if (!value)
        value = (value_t *) zlistx_first (source->values);
    else
    if (value->type == '|')
        value = (value_t *) zlistx_next (source->values);

    //  Now pull values from cursor onwards. List cursor holds first value,
    //  if any, for us to move
    while (value) {
        value = (value_t *) zlistx_detach (source->values, zlistx_cursor (source->values));
        if (!value)
            break;
        if (isalpha (value->type))
            zlistx_add_end (self->values, value);
        value = (value_t *) zlistx_next (source->values);
    }
}


//  ---------------------------------------------------------------------------
//  Return pipe contents, as string. Caller must free it when done. Values are
//  separated by spaces. This empties the pipe.
//  TODO: make this an atomic (paste)

char *
zs_pipe_paste (zs_pipe_t *self)
{
    //  We use an extensible CZMQ chunk
    zchunk_t *chunk = zchunk_new (NULL, 256);
    while (zs_pipe_recv (self) == 0) {
        char *string = zs_pipe_string (self);
        if (zchunk_size (chunk))
            zchunk_extend (chunk, " ", 1);
        zchunk_extend (chunk, string, strlen (string));
    }
    size_t result_size = zchunk_size (chunk);
    char *result = (char *) malloc (result_size + 1);
    memcpy (result, (char *) zchunk_data (chunk), result_size);
    result [result_size] = 0;
    zchunk_destroy (&chunk);
    return result;
}


//  ---------------------------------------------------------------------------
//  Empty the pipe of any values it might contain.

void
zs_pipe_purge (zs_pipe_t *self)
{
    zlistx_purge (self->values);
}


//  ---------------------------------------------------------------------------
//  Selftest

void
zs_pipe_test (bool verbose)
{
    printf (" * zs_pipe: ");
    if (verbose)
        printf ("\n");

    //  @selftest
    zs_pipe_t *pipe = zs_pipe_new ();

    zs_pipe_send_whole (pipe, 12345);
    zs_pipe_send_string (pipe, "Hello World");

    int rc = zs_pipe_recv (pipe);
    assert (rc == 0);
    int64_t whole = zs_pipe_whole (pipe);
    assert (whole == 12345);
    rc = zs_pipe_recv (pipe);
    assert (rc == 0);
    const char *string = zs_pipe_string (pipe);
    assert (streq (string, "Hello World"));
    assert (zs_pipe_recv (pipe) == -1);

    char *results = zs_pipe_paste (pipe);
    assert (streq (results, ""));

    zs_pipe_send_whole (pipe, 4);
    zs_pipe_send_whole (pipe, 5);
    zs_pipe_send_whole (pipe, 6);
    whole = zs_pipe_recv_whole (pipe);
    assert (whole == 4);
    zs_pipe_purge (pipe);
    whole = zs_pipe_recv_whole (pipe);
    assert (whole == 0);

    //  Test phrases
    zs_pipe_mark (pipe);
    zs_pipe_send_whole (pipe, 1);
    zs_pipe_send_whole (pipe, 2);
    zs_pipe_send_whole (pipe, 3);
    zs_pipe_mark (pipe);
    zs_pipe_send_whole (pipe, 4);
    zs_pipe_send_whole (pipe, 5);
    zs_pipe_send_whole (pipe, 6);
    zs_pipe_mark (pipe);
    zs_pipe_send_whole (pipe, 7);
    zs_pipe_send_whole (pipe, 8);
    zs_pipe_send_whole (pipe, 9);
    zs_pipe_mark (pipe);
    zs_pipe_send_whole (pipe, 10);

    zs_pipe_t *copy = zs_pipe_new ();

    //  Modest pull should take single last value
    zs_pipe_pull (copy, pipe, false);
    whole = zs_pipe_recv_whole (copy);
    assert (whole == 10);
    assert (zs_pipe_recv (copy) == -1);

    //  Modest pull should take last phrase
    zs_pipe_pull (copy, pipe, false);
    whole = zs_pipe_recv_whole (copy);
    assert (whole == 7);
    whole = zs_pipe_recv_whole (copy);
    assert (whole == 8);
    whole = zs_pipe_recv_whole (copy);
    assert (whole == 9);
    assert (zs_pipe_recv (copy) == -1);

    //  Add some more to the pipe...
    zs_pipe_mark (pipe);
    zs_pipe_send_whole (pipe, 7);
    zs_pipe_send_whole (pipe, 8);

    //  Greedy pull should take just those two values now
    zs_pipe_pull (copy, pipe, true);
    whole = zs_pipe_recv_whole (copy);
    assert (whole == 7);
    whole = zs_pipe_recv_whole (copy);
    assert (whole == 8);
    assert (zs_pipe_recv (copy) == -1);

    //  Greedy pull should take all six remaining values
    zs_pipe_pull (copy, pipe, true);
    whole = zs_pipe_recv_whole (copy);
    assert (whole == 1);
    whole = zs_pipe_recv_whole (copy);
    assert (whole == 2);
    whole = zs_pipe_recv_whole (copy);
    assert (whole == 3);
    whole = zs_pipe_recv_whole (copy);
    assert (whole == 4);
    whole = zs_pipe_recv_whole (copy);
    assert (whole == 5);
    whole = zs_pipe_recv_whole (copy);
    assert (whole == 6);
    assert (zs_pipe_recv (copy) == -1);

    //  Check pipe is empty
    assert (zs_pipe_recv (pipe) == -1);

    zs_pipe_destroy (&copy);
    zs_pipe_destroy (&pipe);
    //  @end
    printf ("OK\n");
}