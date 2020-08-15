# libuv_pipe_server_client
An example of UNIX Domain Socket communication using Libuv Asynchronous library.



## Build

```bash
git clone https://github.com/manishmanu/libuv_pipe_server_client.git
git submodule update --init --recursive
```

#### Linux

```bash
mkdir build && cd build
cmake ..
make -j${nproc}
```

#### Android

```bash
mkdir build_android && cd build_android
cmake -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake -DANDROID_ABI=arm64-v8a -DANDROID_NATIVE_API_LEVEL=29 ..
make -j${nproc}
```



## Test

Run `uv_server` and `uv_client` binaries separately and data sent from client will be received in server.

```
./uv_server
server started. Waiting for client... 
connected to client
message from client : Hello
message from client : I'm Client
message from client : John Doe 
Client got disconnect. Server terminated!
```

```
./uv_client
Connected with server!
Enter the messages to send to server below (Max 32 chars). Type 'EXIT' to terminate client
Hello
I'm Client
John Doe 
EXIT
Client terminated!
```
