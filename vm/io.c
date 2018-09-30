#include "tr.h"
#include "internal.h"
#include "utils.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

OBJ TrIO_new(VM, OBJ self, OBJ fd) {
  UNUSED(self);
  TrIO *io = TR_INIT_CORE_OBJECT(IO);
  io->fd = TR_FIX2INT(fd);
  return (OBJ)io;
}

OBJ TrIO_close(VM, OBJ self) {
  UNUSED(vm);
  close(TR_CIO(self)->fd);
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

// borrowed from the mruby-io
static int modestr_to_flags(VM, const char *mode) {
  int flags = 0;
  const char *m = mode;

  switch (*m++) {
    case 'r':
      flags |= O_RDONLY;
      break;
    case 'w':
      flags |= O_WRONLY | O_CREAT | O_TRUNC;
      break;
    case 'a':
      flags |= O_WRONLY | O_APPEND | O_CREAT;
      break;
    default:
      tr_raise(ArgumentError, "illegal access mode %s", mode);
  }

  while (*m) {
    switch (*m++) {
      case 't':
        // not relevant for linux
        break;
      case 'b':
        // not relevant for linux
        break;
      case '+':
        flags |= O_RDWR;
        break;
      case ':':
        /* XXX: PASSTHROUGH*/
      default:
        tr_raise(ArgumentError, "illegal access mode %s", mode);
    }
  }

  return flags;
}

OBJ TrIO_sysopen(VM, OBJ self, int argc, OBJ argv[]) {
  UNUSED(self);
  char *path = NULL;
  mode_t perm = 0;
  char *mode = NULL;
  int fd = 0;

  switch(argc) {
      case 3:
        perm = TR_FIX2INT(argv[2]);
      case 2:
        mode = TR_STR_PTR(argv[1]);
      case 1:
        path = TR_STR_PTR(argv[0]);
        break;
      default:
        tr_raise(ArgumentError, "wrong number of arguments (%d for 1..3)", argc);
  }
  if (perm == 0) {
      perm = 0666;
  }
  if (mode == NULL) {
      mode = "r";
  }
  fd = open(path, modestr_to_flags(vm, mode), perm);
  if (fd == -1) {
    tr_raise_errno(path);
  }
  return TR_INT2FIX(fd);
}

// TODO add support to read into the string per spec
OBJ TrIO__sysread(VM, OBJ self, OBJ _maxlen) {
  UNUSED(self);
  int maxlen = TR_FIX2INT(_maxlen);
  int fd = (TR_CIO(self)->fd);
  char *buf = TR_ALLOC_N(char, maxlen + 1);
  ssize_t bytes_read = read(fd, buf, maxlen);
  if (bytes_read != -1) {
    buf[bytes_read] = 0;
    TrString *s = TR_INIT_CORE_OBJECT(String);
    s->len = bytes_read;
    s->interned = 0;
    s->ptr = buf;
    return (OBJ)s;
  }
  TR_FREE(buf);
  tr_raise_errno("IO::sysread");
}

OBJ TrIO__syswrite(VM, OBJ self, OBJ _str) {
  UNUSED(self);
  int fd = (TR_CIO(self)->fd);
  char *str = TR_STR_PTR(_str);
  int str_len = TR_CSTRING(_str)->len;
  ssize_t bytes_written = write(fd, str, str_len);
  if (bytes_written == str_len) {
    return TR_INT2FIX(bytes_written);
  }
  tr_raise_errno("IO::syswrite");
}

void TrIO_init(VM) {
  OBJ c = TR_INIT_CORE_CLASS(IO, Object);
  tr_def(c, "close", TrIO_close, 0);
  tr_metadef(c, "new", TrIO_new, 1);
  tr_metadef(c, "read", TrIO_read, 1);
  tr_metadef(c, "sysopen", TrIO_sysopen, -1);
  tr_def(c, "sysread", TrIO__sysread, 1);
  tr_def(c, "syswrite", TrIO__syswrite, 1);
}
