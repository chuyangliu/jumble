#include "jumble/websvr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

static const int LISTEN_QUEUE_LEN = 3;
static const size_t RECV_BUF_LEN = 1024;

static const char REQUEST_LINE[] = "GET / HTTP/1.1";
static const char RESPONSE[] = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!";

static int listen_socket(unsigned short port);
static void handle(int listen_sock);
static int client_socket(int listen_sock);

void websvr_serve(unsigned short port) {
    int listen_sock = listen_socket(port);

    while (1) {
        handle(listen_sock);
    }

    close(listen_sock); // Unreachable code added for completeness
}

static int listen_socket(unsigned short port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0); // 0 to automatically choose protocol
    if (fd == -1) {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(fd, (const struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind() failed");
        exit(EXIT_FAILURE);
    }

    if (listen(fd, LISTEN_QUEUE_LEN) == -1) {
        perror("listen() failed");
        exit(EXIT_FAILURE);
    }

    printf("Listening on 0.0.0.0:%u (test via curl http://127.0.0.1:%u)\n\n", port, port);
    return fd;
}

static void handle(int listen_sock) {
    int client_sock = client_socket(listen_sock);
    if (client_sock == -1)
    {
        return;
    }

    char buf[RECV_BUF_LEN];
    if (recv(client_sock, buf, sizeof(buf), 0) == -1) {
        perror("recv() failed");
        goto cleanup;
    }

    printf("Request:\n%s\n", buf);

    if (strncmp(buf, REQUEST_LINE, strlen(REQUEST_LINE)) != 0) {
        printf("Request not supported\n");
        goto cleanup;
    }

    if (send(client_sock, RESPONSE, strlen(RESPONSE), 0) == -1) {
        perror("send() failed");
        goto cleanup;
    }

cleanup:
    close(client_sock);
}

static int client_socket(int listen_sock) {
    struct sockaddr_in addr;
    socklen_t addr_len = (socklen_t)sizeof(addr);

    int fd = accept(listen_sock, (struct sockaddr *)&addr, (socklen_t *)&addr_len);
    if (fd == -1) {
        perror("accept() failed");
        return fd;
    }

    printf("Connection accepted from %s:%u\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    return fd;
}
