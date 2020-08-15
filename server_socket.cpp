#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <uv.h>

#include "common.h"

static uv_pipe_t connection;

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
  static char buffer[1024];
  buf->base = buffer;
  buf->len = sizeof(buffer);
}

void read_data_from_client(uv_stream_t* stream,
                           ssize_t nread,
                           const uv_buf_t* buf) {
  if (nread < 0) {
    uv_stop(stream->loop);
    uv_close((uv_handle_t*)stream, NULL);
    printf("Client got disconnect. Server terminated!\n");
    return;
  }

  printf("message from client : %s\n", buf->base);
}

static void connection_cb(uv_stream_t* server, int status) {
  int r;
  assert(status == 0);

  r = uv_pipe_init(server->loop, &connection, 0);
  assert(r == 0);

  r = uv_accept(server, (uv_stream_t*)&connection);
  assert(r == 0);

  printf("received a new connection\n");
  r = uv_read_start((uv_stream_t*)&connection, alloc_buffer,
                    read_data_from_client);
  uv_run(((uv_stream_t*)&connection)->loop, UV_RUN_DEFAULT);

  uv_stop(server->loop);
}

int main() {
  uv_pipe_t server_handle;
  uv_loop_t* loop = uv_default_loop();
  int r;

  r = uv_pipe_init(loop, &server_handle, 0);
  assert(r == 0);

  unlink(SOCKET_NAME);
  r = uv_pipe_bind(&server_handle, SOCKET_NAME);
  assert(r == 0);

  r = uv_listen((uv_stream_t*)&server_handle, 128, connection_cb);
  assert(r == 0);

  uv_run(loop, UV_RUN_DEFAULT);
}
