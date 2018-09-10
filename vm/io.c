#include "tr.h"
#include "internal.h"
#include "utils.h"

#include <sys/stat.h>

OBJ TrIO_new(VM) {
  TrIO *io = TR_INIT_CORE_OBJECT(IO);
  return (OBJ)io;
}

OBJ TrIO_close(VM, OBJ self) {
  UNUSED(vm);
  UNUSED(self);
  return TR_NIL;
}

OBJ TrIO_read(VM, OBJ self, OBJ path_arg) {
  UNUSED(vm);
  UNUSED(self);
  char *path = TR_STR_PTR(path_arg);
  size_t str_len;

  char *str = read_file_into_string(path, &str_len);
  if (str != NULL) {
    TrString *s = TR_INIT_CORE_OBJECT(String);
    s->len = str_len;
    s->interned = 0;
    s->ptr = str;
    return (OBJ)s;
  }
  tr_raise_errno(path);
  return TR_NIL;
}

void TrIO_init(VM) {
  OBJ c = TR_INIT_CORE_CLASS(IO, Object);
  tr_def(c, "close", TrIO_close, 0);
  tr_metadef(c, "read", TrIO_read, 1);
}
