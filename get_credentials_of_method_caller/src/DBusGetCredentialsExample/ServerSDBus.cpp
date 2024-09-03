#include "DBusGetCredentialsExample/ServerSDBus.hpp"

#include <cassert>
#include <cstdlib>

namespace
{

static void sd_bus_creds_print_fields_availability(sd_bus_creds *c)
{
        if (!getenv("DBUS_GET_CREDENTIALS_EXAMPLE_SD_BUS_CREDENTIALS_DEBUG")) {
                return;
        }
        if (strcmp(getenv("DBUS_GET_CREDENTIALS_EXAMPLE_SD_BUS_CREDENTIALS_DEBUG"),
                   "") == 0) {
                return;
        }

        uint64_t mask = sd_bus_creds_get_mask(c);
        fprintf(stderr, "Credentials fields availability:\n");

#define sd_bus_creds_mask_print_field_availability(name) \
        fprintf(stderr,                                  \
                "  %-32s"                                \
                "%s\n",                                  \
                #name, name &mask ? "true" : "false")

        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_PID);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_TID);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_PPID);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_UID);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_EUID);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_SUID);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_FSUID);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_GID);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_EGID);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_SGID);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_FSGID);
        sd_bus_creds_mask_print_field_availability(
                SD_BUS_CREDS_SUPPLEMENTARY_GIDS);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_COMM);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_TID_COMM);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_EXE);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_CMDLINE);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_CGROUP);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_UNIT);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_SLICE);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_USER_UNIT);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_USER_SLICE);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_SESSION);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_OWNER_UID);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_EFFECTIVE_CAPS);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_PERMITTED_CAPS);
        sd_bus_creds_mask_print_field_availability(
                SD_BUS_CREDS_INHERITABLE_CAPS);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_BOUNDING_CAPS);
        sd_bus_creds_mask_print_field_availability(
                SD_BUS_CREDS_SELINUX_CONTEXT);
        sd_bus_creds_mask_print_field_availability(
                SD_BUS_CREDS_AUDIT_SESSION_ID);
        sd_bus_creds_mask_print_field_availability(
                SD_BUS_CREDS_AUDIT_LOGIN_UID);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_TTY);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_UNIQUE_NAME);
        sd_bus_creds_mask_print_field_availability(
                SD_BUS_CREDS_WELL_KNOWN_NAMES);
        sd_bus_creds_mask_print_field_availability(SD_BUS_CREDS_DESCRIPTION);

#undef print_cred_field
}

static int method_ping(sd_bus_message *m, void * /*userdata*/,
                       sd_bus_error *ret_error)
{
        fprintf(stderr, "New method_call message \"Ping\" arrived\n");

        int ret;

        sd_bus_creds *c;
        c = sd_bus_message_get_creds(m);
        if (!c) {
                sd_bus_error_set_const(ret_error, SD_BUS_ERROR_FAILED,
                                       "Failed to get credentials");
                return -1;
        }

        // NOTE:
        // `sd_bus_creds` get from `sd_bus_message_get_creds`
        // only contains the `SD_BUS_CREDS_UNIQUE_NAME` field.
        // function below should print like this:
        // ```plaintext
        // Credentials fields availability:
        //   SD_BUS_CREDS_PID                False
        //   SD_BUS_CREDS_TID                False
        //   SD_BUS_CREDS_PPID               False
        //   SD_BUS_CREDS_UID                False
        //   SD_BUS_CREDS_EUID               False
        //   SD_BUS_CREDS_SUID               False
        //   SD_BUS_CREDS_FSUID              False
        //   SD_BUS_CREDS_GID                False
        //   SD_BUS_CREDS_EGID               False
        //   SD_BUS_CREDS_SGID               False
        //   SD_BUS_CREDS_FSGID              False
        //   SD_BUS_CREDS_SUPPLEMENTARY_GIDS False
        //   SD_BUS_CREDS_COMM               False
        //   SD_BUS_CREDS_TID_COMM           False
        //   SD_BUS_CREDS_EXE                False
        //   SD_BUS_CREDS_CMDLINE            False
        //   SD_BUS_CREDS_CGROUP             False
        //   SD_BUS_CREDS_UNIT               False
        //   SD_BUS_CREDS_SLICE              False
        //   SD_BUS_CREDS_USER_UNIT          False
        //   SD_BUS_CREDS_USER_SLICE         False
        //   SD_BUS_CREDS_SESSION            False
        //   SD_BUS_CREDS_OWNER_UID          False
        //   SD_BUS_CREDS_EFFECTIVE_CAPS     False
        //   SD_BUS_CREDS_PERMITTED_CAPS     False
        //   SD_BUS_CREDS_INHERITABLE_CAPS   False
        //   SD_BUS_CREDS_BOUNDING_CAPS      False
        //   SD_BUS_CREDS_SELINUX_CONTEXT    False
        //   SD_BUS_CREDS_AUDIT_SESSION_ID   False
        //   SD_BUS_CREDS_AUDIT_LOGIN_UID    False
        //   SD_BUS_CREDS_TTY                False
        //   SD_BUS_CREDS_UNIQUE_NAME        True
        //   SD_BUS_CREDS_WELL_KNOWN_NAMES   False
        //   SD_BUS_CREDS_DESCRIPTION        False
        // ```
        sd_bus_creds_print_fields_availability(c);

        const char *unique_name;
        ret = sd_bus_creds_get_unique_name(c, &unique_name);
        if (ret < 0) {
                sd_bus_error_setf(ret_error, SD_BUS_ERROR_FAILED,
                                  "Failed to get unique name: %s",
                                  strerror(-ret));
                return -1;
        }

        // NOTE:
        // If we want to get other credentials fields,
        // we need to send message the message bus to get creds by
        // the unique_name we just get.

        sd_bus *bus;
        bus = sd_bus_message_get_bus(m);
        assert(bus);

        ret = sd_bus_get_name_creds(bus, unique_name, SD_BUS_CREDS_PID, &c);
        if (ret < 0) {
                sd_bus_error_setf(ret_error, SD_BUS_ERROR_FAILED,
                                  "Failed to get credentials: %s",
                                  strerror(-ret));
                return -1;
        }

        // NOTE:
        // `sd_bus_creds` get from `sd_bus_get_name_creds`
        // only contains `SD_BUS_CREDS_PID` field.
        // function below should print like this:
        // ```plaintext
        // Credentials fields availability:
        //   SD_BUS_CREDS_PID                True
        //   SD_BUS_CREDS_TID                False
        //   SD_BUS_CREDS_PPID               False
        //   SD_BUS_CREDS_UID                False
        //   SD_BUS_CREDS_EUID               False
        //   SD_BUS_CREDS_SUID               False
        //   SD_BUS_CREDS_FSUID              False
        //   SD_BUS_CREDS_GID                False
        //   SD_BUS_CREDS_EGID               False
        //   SD_BUS_CREDS_SGID               False
        //   SD_BUS_CREDS_FSGID              False
        //   SD_BUS_CREDS_SUPPLEMENTARY_GIDS False
        //   SD_BUS_CREDS_COMM               False
        //   SD_BUS_CREDS_TID_COMM           False
        //   SD_BUS_CREDS_EXE                False
        //   SD_BUS_CREDS_CMDLINE            False
        //   SD_BUS_CREDS_CGROUP             False
        //   SD_BUS_CREDS_UNIT               False
        //   SD_BUS_CREDS_SLICE              False
        //   SD_BUS_CREDS_USER_UNIT          False
        //   SD_BUS_CREDS_USER_SLICE         False
        //   SD_BUS_CREDS_SESSION            False
        //   SD_BUS_CREDS_OWNER_UID          False
        //   SD_BUS_CREDS_EFFECTIVE_CAPS     False
        //   SD_BUS_CREDS_PERMITTED_CAPS     False
        //   SD_BUS_CREDS_INHERITABLE_CAPS   False
        //   SD_BUS_CREDS_BOUNDING_CAPS      False
        //   SD_BUS_CREDS_SELINUX_CONTEXT    False
        //   SD_BUS_CREDS_AUDIT_SESSION_ID   False
        //   SD_BUS_CREDS_AUDIT_LOGIN_UID    False
        //   SD_BUS_CREDS_TTY                False
        //   SD_BUS_CREDS_UNIQUE_NAME        False
        //   SD_BUS_CREDS_WELL_KNOWN_NAMES   False
        //   SD_BUS_CREDS_DESCRIPTION        False
        // ```
        sd_bus_creds_print_fields_availability(c);

        pid_t pid = -1;

        ret = sd_bus_creds_get_pid(c, &pid);
        if (ret < 0) {
                sd_bus_error_setf(ret_error, SD_BUS_ERROR_FAILED,
                                  "Failed to get PID: %s", strerror(-ret));
                return -1;
        }

        // NOTE:
        // If we want to get other credentials,
        // we have to get creds by the pid we just get.

        ret = sd_bus_creds_new_from_pid(&c, pid, _SD_BUS_CREDS_ALL);
        if (ret < 0) {
                sd_bus_error_setf(ret_error, SD_BUS_ERROR_FAILED,
                                  "Failed to get credentials: %s",
                                  strerror(-ret));
                return -1;
        }

        // NOTE:
        // `sd_bus_creds` get from `sd_bus_creds_new_from_pid`
        // contains almost every fields.
        // function below should print like this:
        // ```plaintext
        // Credentials fields availability:
        //   SD_BUS_CREDS_PID                True
        //   SD_BUS_CREDS_TID                False
        //   SD_BUS_CREDS_PPID               True
        //   SD_BUS_CREDS_UID                True
        //   SD_BUS_CREDS_EUID               True
        //   SD_BUS_CREDS_SUID               True
        //   SD_BUS_CREDS_FSUID              True
        //   SD_BUS_CREDS_GID                True
        //   SD_BUS_CREDS_EGID               True
        //   SD_BUS_CREDS_SGID               True
        //   SD_BUS_CREDS_FSGID              True
        //   SD_BUS_CREDS_SUPPLEMENTARY_GIDS True
        //   SD_BUS_CREDS_COMM               True
        //   SD_BUS_CREDS_TID_COMM           False
        //   SD_BUS_CREDS_EXE                True
        //   SD_BUS_CREDS_CMDLINE            True
        //   SD_BUS_CREDS_CGROUP             True
        //   SD_BUS_CREDS_UNIT               True
        //   SD_BUS_CREDS_SLICE              True
        //   SD_BUS_CREDS_USER_UNIT          True
        //   SD_BUS_CREDS_USER_SLICE         True
        //   SD_BUS_CREDS_SESSION            True
        //   SD_BUS_CREDS_OWNER_UID          True
        //   SD_BUS_CREDS_EFFECTIVE_CAPS     True
        //   SD_BUS_CREDS_PERMITTED_CAPS     True
        //   SD_BUS_CREDS_INHERITABLE_CAPS   True
        //   SD_BUS_CREDS_BOUNDING_CAPS      True
        //   SD_BUS_CREDS_SELINUX_CONTEXT    True
        //   SD_BUS_CREDS_AUDIT_SESSION_ID   True
        //   SD_BUS_CREDS_AUDIT_LOGIN_UID    True
        //   SD_BUS_CREDS_TTY                True
        //   SD_BUS_CREDS_UNIQUE_NAME        False
        //   SD_BUS_CREDS_WELL_KNOWN_NAMES   False
        //   SD_BUS_CREDS_DESCRIPTION        False
        // ```
        sd_bus_creds_print_fields_availability(c);

        uid_t euid;
        ret = sd_bus_creds_get_euid(c, &euid);
        if (ret < 0) {
                sd_bus_error_setf(ret_error, SD_BUS_ERROR_FAILED,
                                  "Failed to get EUID: %s", strerror(-ret));
                return -1;
        }

        gid_t egid;
        ret = sd_bus_creds_get_egid(c, &egid);
        if (ret < 0) {
                sd_bus_error_setf(ret_error, SD_BUS_ERROR_FAILED,
                                  "Failed to get EGID: %s", strerror(-ret));
                return -1;
        }

        const gid_t *supplementary_gids;
        ret = sd_bus_creds_get_supplementary_gids(c, &supplementary_gids);
        if (ret < 0) {
                sd_bus_error_setf(ret_error, SD_BUS_ERROR_FAILED,
                                  "Failed to get supplementary GIDs: %s",
                                  strerror(-ret));
                return -1;
        }
        assert(ret != 0); // Length of supplementary GIDs array should not be 0

        fprintf(stderr, "Caller unique name: %s\n", unique_name);
        fprintf(stderr, "Caller PID: %d\n", pid);
        fprintf(stderr, "Caller EUID: %d\n", euid);
        fprintf(stderr, "Caller EGID: %d\n", egid);
        fprintf(stderr, "Caller Supplementary GIDs: %d", supplementary_gids[0]);
        for (int i = 1; i < ret; i++) {
                fprintf(stderr, " %d", supplementary_gids[i]);
        }
        fprintf(stderr, "\n");

        return sd_bus_reply_method_return(m, "");
}

}

namespace DBusGetCredentialsExample
{
const sd_bus_vtable ServerSDBus::vtable[] = {
        SD_BUS_VTABLE_START(0),
        SD_BUS_METHOD("Ping", "", "", ::method_ping,
                      SD_BUS_VTABLE_UNPRIVILEGED),
        SD_BUS_VTABLE_END
};
}
