/*
** mrb_pmq.c - PMQ class
**
** Copyright (c) Uchio Kondo 2016
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mrb_posix_ipc.h"

void mrb_pmq_class_init(mrb_state *mrb);
void mrb_psem_class_init(mrb_state *mrb);

#define DONE mrb_gc_arena_restore(mrb, 0);

void mrb_mruby_posix_ipc_gem_init(mrb_state *mrb)
{
  mrb_pmq_class_init(mrb); DONE;
  mrb_psem_class_init(mrb); DONE;
}

void mrb_mruby_posix_ipc_gem_final(mrb_state *mrb)
{
}
