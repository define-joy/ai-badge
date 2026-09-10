# Host tests

Does not link ESP-IDF. Covers event dispatch order, disabled small-loop, and Idle → Sleep → Wake.

```text
cmake -S tests/host -B tests/host/build
cmake --build tests/host/build
tests/host/build/badge_host_tests
```

If CMake is missing, any C++17 compiler works. Example:

```text
c++ -std=c++17 -fno-exceptions -fno-rtti -DBADGE_HOST=1 -I kit/include -I . -o tests/host/build/badge_host_tests tests/host/test_runtime.cpp kit/src/AppRegistry.cpp kit/src/Badge.cpp core/PortHub.cpp core/Time.cpp core/EventPump.cpp core/PowerPolicy.cpp core/JobQueue.cpp core/Runtime.cpp adapters/board/bringup.cpp
```
