#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <uv.h>
#include <iostream>

#include "common.h"

static uv_pipe_t pipe_handle;
static uv_connect_t conn_req;

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
  static char buffer[1024];
  buf->base = buffer;
  buf->len = sizeof(buffer);
}

static void write_cb(uv_write_t* req, int status) {
  if (status != 0) {
    printf("write operation failed\n");
    return;
  }
}

static void connect_cb(uv_connect_t* connect_req, int status) {
  if (status == 0) {
    printf("Connected with server!\n");
  } else {
    printf("Failed to connect with serer. (Error %d)\n", status);
  }

  uv_write_t write_req;
  uv_buf_t buf;
  int max_length = 32;
  char msg[max_length];
  std::cout << "Enter the messages to send to server below (Max 32 chars). "
               "Type 'EXIT' to terminate client"
            << std::endl;
  while (true) {
    std::cin.getline(msg, max_length);
    if (strcmp(msg, "EXIT") == 0) {
      break;
    }
    buf = uv_buf_init(msg, max_length);
    uv_write(&write_req, (uv_stream_t*)&pipe_handle, &buf, 1, NULL);
  }

  uv_stop(connect_req->handle->loop);

  // uv_close((uv_handle_t*)&pipe_handle, NULL); //not required as we are
  // closing on server end

  printf("Client terminated!\n");
}

int main() {
  int r;

  r = uv_pipe_init(uv_default_loop(), &pipe_handle, 0);
  assert(r == 0);

  uv_pipe_connect(&conn_req, &pipe_handle, SOCKET_NAME, connect_cb);

  r = uv_run(uv_default_loop(), UV_RUN_DEFAULT);

  uv_loop_close(uv_default_loop());
}
