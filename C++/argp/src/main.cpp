#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <vector>

#include <argp.h>

const char *argp_program_version = "0.1.0";
const char *argp_program_bug_address =
        "https://github.com/black-desk/examples/issues";

namespace argpp
{
namespace detail
{
class parser_flag {
    public:
        static constexpr unsigned empty = 0;

        static constexpr unsigned parse_argv0 = ARGP_PARSE_ARGV0,
                                  no_errs = ARGP_NO_ERRS,
                                  no_args = ARGP_NO_ARGS,
                                  in_order = ARGP_IN_ORDER,
                                  no_help = ARGP_NO_HELP,
                                  no_exit = ARGP_NO_EXIT,
                                  long_only = ARGP_LONG_ONLY,
                                  silent = ARGP_SILENT;

        parser_flag(unsigned flag)
                : flag(flag)
        {
        }

        operator unsigned() const
        {
                return flag;
        }

    private:
        unsigned flag;
};

class option_flag {
    public:
        static constexpr int empty = 0;

        static constexpr int arg_optional = OPTION_ARG_OPTIONAL,
                             hidden = OPTION_HIDDEN, alias = OPTION_ALIAS,
                             doc = OPTION_DOC, no_usage = OPTION_NO_USAGE;

        option_flag(int flag)
                : flag(flag)
        {
        }

        operator int() const
        {
                return flag;
        }

    private:
        int flag;
};

class key {
    public:
        static constexpr int arg = ARGP_KEY_ARG, args = ARGP_KEY_ARGS,
                             end = ARGP_KEY_END, no_args = ARGP_KEY_NO_ARGS,
                             init = ARGP_KEY_INIT, fini = ARGP_KEY_FINI,
                             success = ARGP_KEY_SUCCESS, error = ARGP_KEY_ERROR;

        key(int key)
                : value(key)
        {
        }

        operator int() const
        {
                return this->value;
        }

    private:
        int value;
};

}

class error {
    public:
        static constexpr int success = 0, unknow = ARGP_ERR_UNKNOWN;

        error(int value)
                : value(value)
        {
        }

        operator int() const
        {
                return this->value;
        }

    private:
        int value;
};

enum class option_group_id : int {
        generated = -1,
        iota = 0,
};

class command;

class option {
    public:
        option &with_key(int key)
        {
                assert(key >= 0 &&
                       "use with_short() for printable short option name");
                assert(!this->key && "already set");

                this->key = key;
                return *this;
        }

        option &with_short(char name)
        {
                assert(!this->key && "already set");

                this->key = name;
                return *this;
        }

        option &with_long_name(std::string &&name)
        {
                assert(!this->long_name && "already set");

                this->long_name = std::move(name);
                return *this;
        }

        option &with_bool_value(bool &value)
        {
                assert(!this->bool_value && "already set");
                assert(!this->arg_name && "already set");
                assert(!this->string_value && "already set");

                this->bool_value = std::ref(value);
                return *this;
        }

        option &with_string_value(std::string &&arg_name, std::string &value)
        {
                assert(!this->arg_name && "already set");
                assert(!this->string_value && "already set");

                this->arg_name = std::move(arg_name);
                this->string_value = std::ref(value);
                return *this;
        }

        option &with_doc(std::string &&doc)
        {
                assert(!this->doc && "already set");

                this->doc = std::move(doc);
                return *this;
        }

        friend command;

    private:
        std::optional<std::string> doc;
        std::optional<int> key;
        std::optional<std::string> long_name;
        std::optional<std::string> arg_name;
        std::optional<std::reference_wrapper<std::string>> string_value;
        std::optional<std::reference_wrapper<bool>> bool_value;
};

class option_group {
    public:
        option_group &with_doc(std::string &&doc)
        {
                assert(!this->doc && "already set");

                this->doc = std::move(doc);
                return *this;
        }
        option_group &with_options(std::vector<option> &&options)
        {
                assert(!this->options && "already set");

                this->options = std::move(options);
                return *this;
        }

        option_group &with_id(option_group_id id)
        {
                assert(!this->id && "already set");

                this->id = id;
                return *this;
        }

        friend command;

    private:
        std::optional<option_group_id> id;
        std::optional<std::string> doc;
        std::optional<std::vector<option>> options;
};

class command {
    public:
        command() = default;

        command &with_name(std::string &&name)
        {
                assert(!this->name && "already set");

                this->name = std::move(name);
                return *this;
        }

        command &with_doc(std::string &&doc)
        {
                assert(!this->doc && "already set");

                this->doc = std::move(doc);
                return *this;
        }

        command &with_option_groups(std::vector<option_group> &&option_group)
        {
                assert(!this->option_groups && "already set");

                option_groups = std::move(option_group);
                return *this;
        }

        command &with_args(std::vector<std::string> &args, std::string &&doc)
        {
                assert(!this->subcommands && "already set");
                assert(!this->args && "already set");

                this->args = args;
                this->args_doc = std::move(doc);
                return *this;
        }

        command &with_subcommands(
                std::vector<std::pair<std::string, command>> &&subcommands)
        {
                assert(!this->subcommands && "already set");
                assert(!this->args_doc && "already set");
                assert(!this->args && "already set");

                this->args_doc = std::move(doc);
                this->subcommands = std::move(subcommands);
                return *this;
        }

        int parse(int argc, char **argv)
        {
                std::vector<argp_option> options;
                for (auto &group : *option_groups) {
                        argp_option option_title{};
                        if (group.doc) {
                                option_title.doc = group.doc->c_str();
                        }
                        if (group.id) {
                                option_title.group =
                                        static_cast<int>(*group.id);
                        }
                        options.push_back(option_title);

                        for (auto &opt : *group.options) {
                                argp_option option{};
                                if (opt.key) {
                                        option.key = *opt.key;
                                }
                                if (opt.long_name) {
                                        option.name = opt.long_name->c_str();
                                }
                                if (opt.arg_name) {
                                        option.arg = opt.arg_name->c_str();
                                }
                                if (opt.doc) {
                                        option.doc = opt.doc->c_str();
                                }
                                if (opt.bool_value) {
                                        option.flags =
                                                detail::option_flag::arg_optional;
                                }
                                options.push_back(std::move(option));
                        }
                }
                options.push_back({});

                argp argp{};
                argp.options = options.data();
                argp.parser = parser;
                argp.args_doc = this->args_doc->c_str();
                argp.doc = this->doc->c_str();

                return argp_parse(&argp, argc, argv, 0, 0, this);
        }

    private:
        std::optional<std::string> name;
        std::optional<std::string> doc;

        std::optional<std::map<std::string, command>> subcommands;

        std::optional<std::reference_wrapper<std::vector<std::string>>> args;
        std::optional<std::string> args_doc;

        std::optional<std::vector<option_group>> option_groups;

        static error_t parser(int key, char *arg, struct argp_state *state)
        {
                auto &cmd = *reinterpret_cast<command *>(state->input);
                if (key == static_cast<int>(detail::key::init)) {
                        // NOTE: Initialize state before parsing.
                        return error::success;
                }
                if (key == static_cast<int>(detail::key::arg)) {
                        // NOTE: Parse subcommands.
                        if (cmd.subcommands) {
                                for (auto &subcmd : *cmd.subcommands) {
                                        if (arg != subcmd.first) {
                                                continue;
                                        }
                                        return subcmd.second.parse(
                                                state->argc - state->next,
                                                state->argv + state->next);
                                }
                                return error::unknow;
                        }

                        // NOTE: Parse positional arguments.
                        assert(arg != nullptr);
                        if (!cmd.args) {
                                return error::unknow;
                        }
                        cmd.args->get().push_back(arg);
                        return 0;
                }

                if (!cmd.option_groups) {
                        return error::success;
                }

                for (auto &group : *cmd.option_groups) {
                        if (!group.options) {
                                continue;
                        }

                        for (auto &opt : *group.options) {
                                if (opt.key && *opt.key != key) {
                                        continue;
                                }

                                if (opt.bool_value) {
                                        opt.bool_value->get() = true;
                                        return error::success;
                                }

                                if (opt.string_value) {
                                        assert(arg != nullptr);

                                        opt.string_value->get() = arg;
                                        return error::success;
                                }
                        }
                }
                return error::unknow;
        }
};

int main(int argc, char **argv)
{
        struct {
                bool debug = false;
                bool verbose = false;
                std::string format = "raw";
                std::optional<std::string> command;
        } flags;

        argpp::command root;

        root.with_name("argp-example")
                .with_doc("\n"
                          "Example program to show how to use argp.h in c++.")
                .with_option_groups({
                        argpp::option_group()
                                .with_doc("Global options")
                                .with_options({
                                        argpp::option()
                                                .with_short('d')
                                                .with_long_name("debug")
                                                .with_doc("Enable debug mode")
                                                .with_bool_value(flags.debug),
                                        argpp::option()
                                                .with_short('V')
                                                .with_long_name("verbose")
                                                .with_doc("Enable verbose mode")
                                                .with_bool_value(flags.verbose),
                                        argpp::option()
                                                .with_short('f')
                                                .with_long_name("output-format")
                                                .with_doc(
                                                        R"(Output format ("json" or "raw") [default: "raw"])")
                                                .with_string_value(
                                                        "FORMAT", flags.format),
                                }),
                        argpp::option_group().with_doc(
                                "Some documentation display here."),
                        argpp::option_group()
                                .with_id(argpp::option_group_id::generated)
                                .with_doc(
                                        "Options below were generated by <arpg.h>:"),
                })
                .with_subcommands({});

        if (auto ret = root.parse(argc, argv)) {
                return ret;
        }

        return 0;
}
