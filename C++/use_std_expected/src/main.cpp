#include <expected>
#include <iostream>

void print_fib(int x)
{
        auto fib = [](int x) -> std::expected<int, std::string> {
                if (x < 0) {
                        return std::unexpected("Negative number");
                }

                if (x == 0) {
                        return 0;
                }

                if (x == 1) {
                        return 1;
                }

                return fib(x - 1) + fib(x - 2).value();

                int a = 0, b = 1;
                for (int i = 2; i <= x; ++i) {
                        int c = a + b;
                        a = b;
                        b = c;
                }
                return b;
        };
}

int main()
{
        print_fib(0);
        print_fib(1);
        print_fib(2);
        print_fib(3);
        print_fib(4);
        print_fib(11);
        return 0;
}
