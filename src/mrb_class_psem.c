/*
** mrb_pmq.c - PMQ class
**
** Copyright (c) Uchio Kondo 2016
**
** See Copyright Notice in LICENSE
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <semaphore.h>

#include "mruby.h"
#include "mruby/data.h"
#include "mruby/error.h"
#include "mruby/string.h"
#include "mrb_posix_ipc.h"

typedef struct {
  sem_t *sem;
  const char *name;
  bool unlinked;
} mrb_psem_data;

static void mrb_psem_free(mrb_state *mrb, void *p)
{
  mrb_psem_data *d = (mrb_psem_data *)p;
  if(d->name == NULL) {
    sem_destroy(d->sem);
  } else {
    sem_close(d->sem);
  }
  mrb_free(mrb, d);
}

static const struct mrb_data_type mrb_psem_data_type = {
  "mrb_psem_data", mrb_psem_free,
};

static int psem_initialize_with_name(mrb_psem_data *psem, const char* name, int flag, int initvalue)
{
  sem_t *sem;
  sem = sem_open(name, flag, 0644, initvalue);
  if(sem == SEM_FAILED) {
    return -1;
  }
  psem->sem = sem;
  psem->name = name;
  psem->unlinked = false;
  return 0;
}

static int psem_initialize_without_name(mrb_psem_data *psem, int flag, int initvalue)
{
  sem_t *sem = malloc(sizeof(sem));

  if(sem_init(sem, flag, initvalue)) {
    return -1;
  }
  psem->sem = sem;
  psem->name = NULL;
  psem->unlinked = false;
  return 0;
}

static mrb_value mrb_psem_init(mrb_state *mrb, mrb_value self)
{
  mrb_psem_data *psem;
  char *name;
  mrb_int flag, initvalue = 0;
  int ret;

  psem = (mrb_psem_data *)DATA_PTR(self);
  if (psem) {
    mrb_free(mrb, psem);
  }
  DATA_TYPE(self) = &mrb_psem_data_type;
  DATA_PTR(self) = NULL;

  mrb_get_args(mrb, "z?i|i", &name, &flag, &initvalue);
  psem = (mrb_psem_data *)mrb_malloc(mrb, sizeof(mrb_psem_data));

  if(name == NULL){
    ret = psem_initialize_without_name(psem, flag, initvalue);
  } else {
    ret = psem_initialize_with_name(psem, name, flag, initvalue);
  }
  if(ret < 0) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "Failed to create PSem data.");
  }
  DATA_PTR(self) = psem;

  return self;
}

static mrb_value mrb_psem_wait(mrb_state *mrb, mrb_value self)
{
  mrb_psem_data *psem = (mrb_psem_data *)DATA_PTR(self);
  if(sem_wait(psem->sem) < 0){
    mrb_sys_fail(mrb, "sem_wait failed.");
  }

  return mrb_fixnum_value(0);
}

static mrb_value mrb_psem_trywait(mrb_state *mrb, mrb_value self)
{
  mrb_psem_data *psem = (mrb_psem_data *)DATA_PTR(self);
  int ret;
  ret = sem_trywait(psem->sem);
  if(ret < 0){
    if(errno != EAGAIN) {
      mrb_sys_fail(mrb, "sem_trywait looks failed.");
    }
  }

  return mrb_fixnum_value(ret);
}

static mrb_value mrb_psem_post(mrb_state *mrb, mrb_value self)
{
  mrb_psem_data *psem = (mrb_psem_data *)DATA_PTR(self);
  if(sem_post(psem->sem) < 0){
    mrb_sys_fail(mrb, "sem_post failed.");
  }

  return mrb_funcall(mrb, self, "value", 0);
}

static mrb_value mrb_psem_close(mrb_state *mrb, mrb_value self)
{
  mrb_psem_data *psem = (mrb_psem_data *)DATA_PTR(self);

  if(psem->name == NULL) {
    return mrb_fixnum_value(sem_destroy(psem->sem));
  } else {
    return mrb_fixnum_value(sem_close(psem->sem));
  }
}

static mrb_value mrb_psem_unlink(mrb_state *mrb, mrb_value self)
{
  mrb_psem_data *psem = (mrb_psem_data *)DATA_PTR(self);
  int ret;

  if(psem->name == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "semaphore has no name");
  }

  ret = sem_unlink(psem->name);
  if(ret < 0) {
    mrb_sys_fail(mrb, "sem_unlink failed.");
  }
  psem->unlinked = true;
  return mrb_fixnum_value(ret);
}

static mrb_value mrb_psem_name(mrb_state *mrb, mrb_value self)
{
  mrb_psem_data *psem = (mrb_psem_data *)DATA_PTR(self);
  if(psem->name == NULL) {
    return mrb_nil_value();
  }
  return mrb_str_new_cstr(mrb, psem->name);
}

static mrb_value mrb_psem_value(mrb_state *mrb, mrb_value self)
{
  mrb_psem_data *psem = (mrb_psem_data *)DATA_PTR(self);
  int value;
  if(sem_getvalue(psem->sem, &value) < 0) {
    mrb_sys_fail(mrb, "sem_getvalue failed.");
  }
  return mrb_fixnum_value(value);
}

static mrb_value mrb_psem_is_unlinked(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(((mrb_psem_data *) DATA_PTR(self))->unlinked);
}

void mrb_psem_class_init(mrb_state *mrb)
{
  struct RClass *psem;
  psem = mrb_define_class(mrb, "PSem", mrb->object_class);
  mrb_define_method(mrb, psem, "initialize", mrb_psem_init,        MRB_ARGS_ARG(2,1));
  mrb_define_method(mrb, psem, "wait",       mrb_psem_wait,        MRB_ARGS_NONE());
  mrb_define_method(mrb, psem, "trywait",    mrb_psem_trywait,     MRB_ARGS_NONE());
  mrb_define_method(mrb, psem, "post",       mrb_psem_post,        MRB_ARGS_NONE());
  mrb_define_method(mrb, psem, "close",      mrb_psem_close,       MRB_ARGS_NONE());
  mrb_define_method(mrb, psem, "unlink",     mrb_psem_unlink,      MRB_ARGS_NONE());
  mrb_define_method(mrb, psem, "name",       mrb_psem_name,        MRB_ARGS_NONE());
  mrb_define_method(mrb, psem, "value",      mrb_psem_value,       MRB_ARGS_NONE());
  mrb_define_method(mrb, psem, "unlinked?",  mrb_psem_is_unlinked, MRB_ARGS_NONE());
}
