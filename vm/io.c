#include "tr.h"
#include "internal.h"

OBJ TrIO_new(VM) {
  TrIO *io = TR_INIT_CORE_OBJECT(IO);
  return (OBJ)io;
}

OBJ TrIO_close(VM, OBJ self) {
  UNUSED(vm);
  UNUSED(self);
  return TR_NIL;
}


void TrIO_init(VM) {
  OBJ c = TR_INIT_CORE_CLASS(IO, Object);
  tr_def(c, "close", TrIO_close, 0);
}
