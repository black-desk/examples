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

class option {
    public:
        option &with_short(char name)
        {
                short_name = name;
                return *this;
        }

        option &with_long(std::string &&name)
        {
                long_name = std::move(name);
                return *this;
        }

        option &with_arg_name(std::string &&arg_name)
        {
                this->arg_name = std::move(arg_name);
                return *this;
        }

        option &with_doc(std::string &&doc)
        {
                this->doc = std::move(doc);
                return *this;
        }

    private:
        std::optional<std::string> doc;
        std::optional<char> short_name;
        std::optional<std::string> long_name;
        std::optional<std::string> arg_name;
};

class command {
    public:
        command() = default;
        command &with_doc(std::string &&doc)
        {
                this->doc = std::move(doc);
                return *this;
        }

        command &with_args_doc(std::string &&args_doc)
        {
                this->args_doc = std::move(args_doc);
                return *this;
        }

        command &with_options(std::vector<option> &&opts)
        {
                if (!options) {
                        options = std::vector<std::unique_ptr<option>>();
                }

                options->push_back(std::make_unique<option>(std::move(opt)));
                return *this;
        }

    private:
        std::optional<std::string> doc;
        std::optional<std::string> args_doc;
        std::optional<std::vector<std::unique_ptr<option>>> options;
};
}

int main(int argc, char **argv)
{
        argp_option option[] = {
                new_argp_option_doc("Global options:"),
                new_argp_option_flag("debug", argp_option_key::debug,
                                     argp_option_flag::empty,
                                     "Enable debug mode"),
                new_argp_option_flag("verbose", argp_option_key::verbose,
                                     argp_option_flag::empty,
                                     "Enable verbose mode"),
                new_argp_option(
                        "output-format", argp_option_key::output_format,
                        "FORMAT", argp_option_flag::empty,
                        R"(Output format ("json" or "raw") [default: "raw"])"),
                new_argp_option_doc("Options below were generated by <arpg.h>:",
                                    argp_option_group::generated),
                {}
        };

        argp global = {
                DESIGNATED_INITIALIZERS(.options =) option,
                DESIGNATED_INITIALIZERS(.parser =) argp_parser,
                DESIGNATED_INITIALIZERS(.args_doc =) "\n"
                                                     "<COMMAND> [OPTION...]",
                DESIGNATED_INITIALIZERS(.doc =) //
                "\n"
                "Example program to show how to use argp.h in c++.",

                DESIGNATED_INITIALIZERS(.children =) nullptr,
                DESIGNATED_INITIALIZERS(.help_filter =) nullptr,
                DESIGNATED_INITIALIZERS(.argp_domain =) nullptr,
        };

        int index_of_first_unparsed_option = 0;
        flags flags = {};
        auto ret = argp_parse(&global, argc, argv,
                              static_cast<unsigned>(argp_parser_flag::in_order),
                              &index_of_first_unparsed_option, &flags);
        if (ret) {
                return ret;
        }

        std::cout << "flags.debug: " << flags.debug << std::endl
                  << "flags.verbose: " << flags.verbose << std::endl
                  << "flags.output_format: "
                  << static_cast<int>(flags.output_format) << std::endl
                  << "flags.command: " << flags.command.value_or("NULL")
                  << std::endl;

        return 0;
}
