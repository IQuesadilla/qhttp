static const char* headers = \
"HTTP/1.1 200 OK\r\n"\
"Content-type: text/html\r\n"\
"Connection: close\r\n"\
"\r\n";

struct QHTTP;
extern int QHTTPSZ;

struct QHTTP_backend {
  void (*load_file)(void *ud, int id, char *buff, int buffsz);
};

void qhttp_register_webpage(struct QHTTP *q, int id, const char *path);
int parse(const char *data);
