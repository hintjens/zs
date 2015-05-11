/*  =========================================================================
    zs_vm - ZeroScript virtual machine

    Copyright (c) the Contributors as noted in the AUTHORS file.
    This file is part of the ZeroScript language, http://zeroscript.org.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#ifndef ZS_VM_H_INCLUDED
#define ZS_VM_H_INCLUDED

#include <czmq.h>

#ifdef __cplusplus
extern "C" {
#endif

//  Opaque class structure
#ifndef ZS_VM_T_DEFINED
typedef struct _zs_vm_t zs_vm_t;
#endif

//  Virtual machine atomic function type
typedef int (zs_vm_fn_t) (zs_vm_t *self);

//  @interface
//  Create a new empty virtual machine. Returns the reference if successful,
//  or NULL if construction failed due to lack of available memory.
zs_vm_t *
    zs_vm_new (void);

//  Destroy the virtual machine and free all memory used by it.
void
    zs_vm_destroy (zs_vm_t **self_p);

//  Probe atomic to ask it to register itself; we use a self-registration
//  system where all information about an atomic is encapsulated in its
//  source code, rather than spread throughout the codebase. It's valid to
//  probe dictionary at any time.
void
    zs_vm_probe (zs_vm_t *self, zs_vm_fn_t *atomic);

//  Return true if we're probing dictionary; this tells dictionary to
//  register rather than to run.
bool
    zs_vm_probing (zs_vm_t *self);

//  Primitive registers itself with the execution context. This is only valid
//  if zs_vm_probing () is true. Returns 0 if registration worked, -1 if it
//  failed due to an internal error.
int
    zs_vm_register (zs_vm_t *self, const char *name, const char *hint);

//  Compile a whole number constant into the virtual machine.
//  Whole numbers are stored thus:
//      [VM_WHOLE][8 bytes in host format]
void
    zs_vm_compile_whole (zs_vm_t *self, int64_t whole);

//  Compile a real number constant into the virtual machine.
//  Whole numbers are stored thus:
//      [VM_REAL][8 bytes in host format]
void
    zs_vm_compile_real (zs_vm_t *self, double real);

//  Compile a string constant into the virtual machine.
void
    zs_vm_compile_string (zs_vm_t *self, const char *string);

//  Compile a new function definition; end with a commit.
void
    zs_vm_compile_define (zs_vm_t *self, const char *name);

//  Close the current function definition.
void
    zs_vm_compile_commit (zs_vm_t *self);

//  Cancel the current or last function definition and reset the virtual
//  machine to the state before the previous _define. You can call this
//  repeatedly to delete function definitions until the machine is empty.
//  Returns 0 if OK, -1 if there was no function to rollback (the machine
//  is then empty).
int
    zs_vm_compile_rollback (zs_vm_t *self);

//  Compile inline function call. The function gets the current input and
//  output pipes. Returns 0 if OK or -1 if the function was not defined.
int
    zs_vm_compile_inline (zs_vm_t *self, const char *name);

//  Compile end of phrase. The next function gets the previous output pipe
//  as its new input pipe.
int
    zs_vm_compile_phrase (zs_vm_t *self);

//  Compile end of sentence. This separates sentences so the next sentence
//  gets clean pipes. Next function gets empty input and output pipes.
int
    zs_vm_compile_period (zs_vm_t *self);

//  Compile an nest operation, saves the current output pipe and creates a
//  new input pipe. Saves the function, which is exectuted by the matching
//  unnest operation. Returns 0 if OK or -1 if the function was not defined.
int
    zs_vm_compile_nest (zs_vm_t *self, const char *name);

//  Compile an unnest operation. Uses the current output pipe as input, and
//  popes the previously saved output pipe, then calls the function specified
//  in the original nest call.
void
    zs_vm_compile_unnest (zs_vm_t *self);

//  Return input pipe for the execution context
zs_pipe_t *
    zs_vm_input (zs_vm_t *self);

//  Return output pipe for the execution context
zs_pipe_t *
    zs_vm_output (zs_vm_t *self);

//  Dump VM contents (state and code)
void
    zs_vm_dump (zs_vm_t *self);

//  Enable tracing of VM compilation and execution.
void
    zs_vm_set_verbose (zs_vm_t *self, bool verbose);

//  Run last defined function, if any, in the VM. This continues forever or
//  until the function ends. Returns 0 if stopped successfully, or -1 if
//  stopped due to some error. Each run of the VM starts with clean pipes.
int
    zs_vm_run (zs_vm_t *self);

//  Self test of this class
void
    zs_vm_test (bool animate);
//  @end

#ifdef __cplusplus
}
#endif

#endif
