#include "../headers/philosophers.h"

int ft_atoi(const char *str)
{
    int res;
    int modulator;

    res = 0;
    modulator = 1;
    while (*str && (*str == 9 || *str == 10 || *str == 11 || *str == 12 || *str == 13 || *str == 32))
        str++;
    if (*str == '-' || *str == '+')
    {
        if (!(*(str + 1) >= '0' && *(str + 1) <= '9'))
            return (0);
        if (*str == '-')
            modulator = -1;
        str++;
    }
    while (*str && (*(str) >= '0' && *(str) <= '9'))
    {
        res += *str - 48;
        if ((*(str + 1) >= '0' && *(str + 1) <= '9'))
            res *= 10;
        str++;
    }
    return (res * modulator);
}