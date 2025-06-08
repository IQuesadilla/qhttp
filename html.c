#include "html.h"

struct QHTTP_webpage {
  int hash;
};

struct QHTTP {
  int temp[64];
};

int QHTTPSZ = sizeof(struct QHTTP);

// Since path is hashed, it is very important that it is of the form "/dir/file/etc"
void qhttp_register_webpage(struct QHTTP *q, int id, const char *path) {
  int hash = 0;
  for (int k = 0; path[k] != 0; ++k) {
    hash += path[k] * (k+1);
  }
}

int parse(const char *data) {
  return 1;
}
