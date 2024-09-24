#pragma once

#include <systemd/sd-bus.h>

namespace DBusGetCredentialsExample
{
class ServerSDBus {
    public:
        static const sd_bus_vtable vtable[];
};
}
