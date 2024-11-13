#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include <argp.h>

class argp_parser_flag {
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

        argp_parser_flag(unsigned flag)
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

class argp_option_flag {
    public:
        static constexpr int empty = 0;

        static constexpr int arg_optional = OPTION_ARG_OPTIONAL,
                             hidden = OPTION_HIDDEN, alias = OPTION_ALIAS,
                             doc = OPTION_DOC, no_usage = OPTION_NO_USAGE;

        argp_option_flag(int flag)
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

struct argp_key {
    public:
        static constexpr int arg = ARGP_KEY_ARG, args = ARGP_KEY_ARGS,
                             end = ARGP_KEY_END, no_args = ARGP_KEY_NO_ARGS,
                             init = ARGP_KEY_INIT, fini = ARGP_KEY_FINI,
                             success = ARGP_KEY_SUCCESS, error = ARGP_KEY_ERROR;

        argp_key(int key)
                : key(key)
        {
        }

        operator int() const
        {
                return key;
        }

    private:
        int key;
};

enum class argp_option_group : int {
        generated = -1,
        iota = 0,
};

enum class argp_option_key : int {
        empty = 0,
        verbose = 'V',
        output_format = 'f',
        debug = INT_MIN,
};

#ifndef __cpp_designated_initializers
#define DESIGNATED_INITIALIZERS(...)
#else
#define DESIGNATED_INITIALIZERS(...) __VA_ARGS__
#endif

argp_option new_argp_option(const char *name, argp_option_key key,
                            const char *arg, argp_option_flag flag,
                            const char *doc,
                            argp_option_group group = argp_option_group::iota)
{
        return {

                DESIGNATED_INITIALIZERS(.name =) name,
                DESIGNATED_INITIALIZERS(.key =) static_cast<int>(key),
                DESIGNATED_INITIALIZERS(.arg =) arg,
                DESIGNATED_INITIALIZERS(.flags =) static_cast<int>(flag),
                DESIGNATED_INITIALIZERS(.doc =) doc,
                DESIGNATED_INITIALIZERS(.group =) static_cast<int>(group),
        };
}

argp_option
new_argp_option_doc(const char *doc,
                    argp_option_group group = argp_option_group::iota)
{
        return new_argp_option(nullptr, argp_option_key::empty, nullptr,
                               argp_option_flag::doc, doc, group);
}

argp_option
new_argp_option_flag(const char *name, argp_option_key key,
                     argp_option_flag flag, const char *doc,
                     argp_option_group group = argp_option_group::iota)
{
        return new_argp_option(name, key, nullptr,
                               flag | argp_option_flag::arg_optional, doc,
                               group);
}

const char *argp_program_version = "0.1.0";
const char *argp_program_bug_address =
        "https://github.com/black-desk/examples/issues";

struct flags {
        bool debug = false;

        bool verbose = false;

        enum class output_format { unkown, raw, json, max };
        output_format output_format = output_format::raw;

        std::optional<std::string> command;
};

error_t argp_parser(int key, char *arg, struct argp_state *state)
{
        auto &flags = *reinterpret_cast<struct flags *>(state->input);
        switch (key) {
        case static_cast<int>(argp_key::init): {
                // NOTE: Initialize state before parsing.
                return 0;
        } break;

        case static_cast<int>(argp_option_key::debug): {
                flags.debug = true;
                return 0;
        } break;

        case static_cast<int>(argp_option_key::verbose): {
                flags.verbose = true;
                return 0;
        } break;

        case static_cast<int>(argp_option_key::output_format): {
                assert(arg != nullptr);
                if (strcmp(arg, "json") == 0) {
                        flags.output_format = flags::output_format::json;
                        return 0;
                }
                if (strcmp(arg, "raw") == 0) {
                        flags.output_format = flags::output_format::raw;
                        return 0;
                }

                return ARGP_ERR_UNKNOWN;
        } break;

        case static_cast<int>(argp_key::arg): {
                // NOTE: Parse positional arguments.

                assert(arg != nullptr);
                if (!flags.command) {
                        flags.command = arg;
                        return 0;
                }
                return ARGP_ERR_UNKNOWN;
        } break;

        default: {
                return ARGP_ERR_UNKNOWN;
        } break;
        }
}

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

        command &with_subcommands(std::string &&doc,
                                  std::vector<command> &&subcommands)
        {
                assert(!this->subcommands && "already set");
                assert(!this->args_doc && "already set");
                assert(!this->args && "already set");

                this->args_doc = std::move(doc);
                this->subcommands = std::move(subcommands);
                return *this;
        }

        int execute(int argc, char **argv)
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
                                                argp_option_flag::arg_optional;
                                }
                                options.push_back(std::move(option));
                        }
                }
                options.push_back({});

                argp argp{
                        DESIGNATED_INITIALIZERS(.options =) options.data(),
                        DESIGNATED_INITIALIZERS(.parser =) argp_parser,
                        DESIGNATED_INITIALIZERS(
                                        .args_doc =) "\n"
                                                     "<COMMAND> [OPTION...]",
                        DESIGNATED_INITIALIZERS(.doc =) //
                        "\n"
                        "Example program to show how to use argp.h in c++.",

                        DESIGNATED_INITIALIZERS(.children =) nullptr,
                        DESIGNATED_INITIALIZERS(.help_filter =) nullptr,
                        DESIGNATED_INITIALIZERS(.argp_domain =) nullptr,
                };

                return 0;
        }

    private:
        std::optional<std::string> name;
        std::optional<std::string> doc;

        std::optional<std::vector<command>> subcommands;

        std::optional<std::reference_wrapper<std::vector<std::string>>> args;
        std::optional<std::string> args_doc;

        std::optional<std::vector<option_group>> option_groups;
};
}

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
                .with_subcommands("<COMMAND> [...]", {});

        return root.execute(argc, argv);

        // argp_option option[] = {
        // new_argp_option_doc("Global options:"),
        // new_argp_option_flag("debug", argp_option_key::debug,
        // argp_option_flag::empty,
        // "Enable debug mode"),
        // new_argp_option_flag("verbose", argp_option_key::verbose,
        // argp_option_flag::empty,
        // "Enable verbose mode"),
        // new_argp_option(
        // "output-format", argp_option_key::output_format,
        // "FORMAT", argp_option_flag::empty,
        // R"(Output format ("json" or "raw") [default: "raw"])"),
        // new_argp_option_doc("Options below were generated by <arpg.h>:",
        // argp_option_group::generated),
        // {}
        // };

        // argp global = {
        // DESIGNATED_INITIALIZERS(.options =) option,
        // DESIGNATED_INITIALIZERS(.parser =) argp_parser,
        // DESIGNATED_INITIALIZERS(.args_doc =) "\n"
        // "<COMMAND> [OPTION...]",
        // DESIGNATED_INITIALIZERS(.doc =) //
        // "\n"
        // "Example program to show how to use argp.h in c++.",

        // DESIGNATED_INITIALIZERS(.children =) nullptr,
        // DESIGNATED_INITIALIZERS(.help_filter =) nullptr,
        // DESIGNATED_INITIALIZERS(.argp_domain =) nullptr,
        // };

        // int index_of_first_unparsed_option = 0;
        // flags flags = {};
        // auto ret = argp_parse(&global, argc, argv,
        // static_cast<unsigned>(argp_parser_flag::in_order),
        // &index_of_first_unparsed_option, &flags);
        // if (ret) {
        // return ret;
        // }

        // std::cout << "flags.debug: " << flags.debug << std::endl
        // << "flags.verbose: " << flags.verbose << std::endl
        // << "flags.output_format: "
        // << static_cast<int>(flags.output_format) << std::endl
        // << "flags.command: " << flags.command.value_or("NULL")
        // << std::endl;

        return 0;
}
