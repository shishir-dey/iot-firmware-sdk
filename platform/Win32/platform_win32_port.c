#include "transport.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

typedef struct {
    SOCKET sock;
} socket_context;

int port_open(void** ctx, const char* host, const char* port)
{
    WSADATA wsaData;
    struct addrinfo hints, *res;
    int result;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return -1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    result = getaddrinfo(host, port, &hints, &res);
    if (result != 0) {
        WSACleanup();
        return -1;
    }

    SOCKET sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == INVALID_SOCKET) {
        freeaddrinfo(res);
        WSACleanup();
        return -1;
    }

    if (connect(sock, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }

    freeaddrinfo(res);

    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return -1;
    }

    socket_context* ctx_ptr = (socket_context*)malloc(sizeof(socket_context));
    ctx_ptr->sock = sock;
    *ctx = ctx_ptr;

    return 0;
}

void port_close(void* ctx)
{
    socket_context* sock_ctx = (socket_context*)ctx;
    if (sock_ctx) {
        closesocket(sock_ctx->sock);
        free(sock_ctx);
    }
    WSACleanup();
}

int port_ssl_send(void* ctx, const unsigned char* buf, size_t len)
{
    socket_context* sock_ctx = (socket_context*)ctx;
    int total_sent = 0;
    int ret;

    while (total_sent < len) {
        ret = send(sock_ctx->sock, (const char*)buf + total_sent, (int)(len - total_sent), 0);
        if (ret == SOCKET_ERROR) {
            int wsa_err = WSAGetLastError();
            if (wsa_err == WSAEWOULDBLOCK) {
                return total_sent;
            } else {
                return -1;
            }
        }
        total_sent += ret;
    }
    return total_sent;
}

int port_ssl_recv(void* ctx, unsigned char* buf, size_t len)
{
    socket_context* sock_ctx = (socket_context*)ctx;
    int total_received = 0;
    int ret;

    while (total_received < len) {
        ret = recv(sock_ctx->sock, (char*)buf + total_received, (int)(len - total_received), 0);
        if (ret < 0) {
            int wsa_err = WSAGetLastError();
            if (wsa_err == WSAEWOULDBLOCK) {
                return total_received;
            } else {
                return -1;
            }
        } else if (ret == 0) {
            return total_received;
        }
        total_received += ret;
    }
    return total_received;
}
