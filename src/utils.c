#include "../headers/philosophers.h"

int safe_p_atoi(const char *str) {
    int res;
    int digit;

    res = 0;
    while (*str && (*(str) >= '0' && *(str) <= '9')) {
        digit = *str - '0';
        if(res > (INT_MAX - digit) / 10)
            return (-1);
        res = res * 10 + digit;
        str++;
    }
    return (res);
}
