#include "jumble/websvr.h"
#include <stdlib.h>

static const unsigned short DEFAULT_PORT = 9876;

int main(int argc, char *argv[]) {
    unsigned short port = argc > 1 ? atoi(argv[1]) : DEFAULT_PORT;
    websvr_serve(port);
    return 0;
}
