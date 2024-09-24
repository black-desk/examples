#include <cstdlib>

#include <systemd/sd-bus.h>

#include "DBusGetCredentialsExample/ServerSDBus.hpp"
#include "DBusGetCredentialsExample/configure.hpp"

using namespace DBusGetCredentialsExample;

int main()
{
        sd_bus *bus = NULL;
        sd_bus_slot *slot = NULL;
        int ret;

        // Connect to the system bus
        ret = sd_bus_open_user(&bus);
        if (ret < 0) {
                fprintf(stderr, "Failed to connect to system bus: %s\n",
                        strerror(-ret));
                goto finish;
        }

        // Register the object
        ret = sd_bus_add_object_vtable(bus, &slot, serverDBusObjectPath.data(),
                                       serverDBusInterfaceName.data(),
                                       ServerSDBus::vtable, NULL);
        if (ret < 0) {
                fprintf(stderr, "Failed to issue method call: %s\n",
                        strerror(-ret));
                goto finish;
        }

        // Request the service name
        ret = sd_bus_request_name(bus, serverDBusServiceName.data(), 0);
        if (ret < 0) {
                fprintf(stderr, "Failed to acquire service name: %s\n",
                        strerror(-ret));
                goto finish;
        }

        // Process requests
        for (;;) {
                ret = sd_bus_process(bus, NULL);
                if (ret < 0) {
                        fprintf(stderr, "Failed to process bus: %s\n",
                                strerror(-ret));
                        goto finish;
                }
                if (ret > 0) {
                        // we processed a request, try to process another one, right-away
                        continue;
                }

                // Wait for the next request to process
                ret = sd_bus_wait(bus, (uint64_t)-1);
                if (ret < 0) {
                        fprintf(stderr, "Failed to wait on bus: %s\n",
                                strerror(-ret));
                        goto finish;
                }
        }

finish:
        sd_bus_slot_unref(slot);
        sd_bus_unref(bus);

        return ret < 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
