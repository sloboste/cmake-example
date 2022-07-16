#include <stdbool.h>
#include <stdio.h>

#include <example_lib/example.h>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    int a[] = {123, 456, 789};
    (void)a;

    example_do_things();

    printf("Hello, World!\n");
    return 0;
}
