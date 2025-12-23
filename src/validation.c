#include "../headers/philosophers.h"

int check_numbers(int argc, char **argv) {
    int i;
    int j;

    i = 1;
    while (i < argc)
    {
        j = 0;
        while (argv[i][j])
        {
            if(!(argv[i][j] >= '0' && argv[i][j] <= '9'))
                return (1);
            ++j;
        }  
        ++i;
    }
    return (0);
}

int check_nbr_size(int argc, char **argv) {
    int i;

    i = 1;
    while (i < argc)
    {
        if(safe_p_atoi(argv[i]) == -1)
            return (1);
        ++i;
    }
    return (0);
}

int validate_input(int argc, char **argv) {
    int res;

    res = 0;
    res += check_numbers(argc, argv);
    res += check_nbr_size(argc, argv);
    return (res);
}