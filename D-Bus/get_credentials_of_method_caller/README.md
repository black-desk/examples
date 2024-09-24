# How to get credentials of method caller

This is an example project
to show how to get `UID`, `GID`, `PID` and Linux security labels
of method caller as a D-Bus service implemented with QDBus and `sd_bus`.

- [Server source entry point](./apps/server/src/main.cpp)
- [Server source entry point implemented by `sd_bus`](./apps/server-sd_bus/src/main.cpp)
- [Client source entry point](./apps/client/src/main.cpp)
