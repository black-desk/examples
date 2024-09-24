# Examples

![](https://img.shields.io/github/check-runs/black-desk/examples/master)
![](https://img.shields.io/github/commit-activity/w/black-desk/examples/master)
![](https://img.shields.io/github/contributors/black-desk/examples)

This repository holds the examples I write for libraries and tools that
not created by me.

---------------------------------------------------------------------

The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT",
"SHOULD", "SHOULD NOT", "RECOMMENDED", "MAY", and "OPTIONAL" in this
document are to be interpreted as described in RFC 2119.

## Table of Contents

- [D-Bus](#d-bus)
  - [How to create and connect to peer-to-peer message
    bus](#how-to-create-and-connect-to-peer-to-peer-message-bus)
  - [How to get credentials of method
    caller](#how-to-get-credentials-of-method-caller)

## D-Bus

Some How-to-X examples of D-Bus IPC protocol.

### How to create and connect to peer-to-peer message bus

This is an example project to show how to write a D-Bus service and a
client with QDBus communicating with each other via peer-to-peer message
bus.

- [Server source entry point](./apps/server/src/main.cpp)
- [Client source entry point](./apps/client/src/main.cpp)

### How to get credentials of method caller

This is an example project to show how to get `UID`, `GID`, `PID` and
Linux security labels of method caller as a D-Bus service implemented
with QDBus and `sd_bus`.

- [Server source entry point](./apps/server/src/main.cpp)
- [Server source entry point implemented by
  `sd_bus`](./apps/server-sd_bus/src/main.cpp)
- [Client source entry point](./apps/client/src/main.cpp)
