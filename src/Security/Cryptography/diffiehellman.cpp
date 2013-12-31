/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "diffiehellman.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

using namespace std;

static char* format_bi_str(const char* str);
static void bi_to_str_base(string& str, bigint bi, int base);
static void bi_to_str_pos(string& str, bigint bi, int base);
static char digit_to_char(int d);

DiffieHellman :: DiffieHellman(const char* aP, const char* aG)
    : p(nullptr), g(nullptr),
      a(nullptr), b(nullptr),
      s(nullptr), A(nullptr), B(nullptr)
{
    p = str_to_bi_base(format_bi_str(aP), 16);
    g = str_to_bi_base(format_bi_str(aG), 16);
}

DiffieHellman :: ~DiffieHellman()
{
    if (p != nullptr)
        bi_free(p);

    if (g != nullptr)
        bi_free(g);

    if (a != nullptr)
        bi_free(a);

    if (b != nullptr)
        bi_free(b);

    if (s != nullptr)
        bi_free(s);

    if (A != nullptr)
        bi_free(A);

    if (B != nullptr)
        bi_free(B);
}

string
DiffieHellman :: generateRequest()
{
    a = bi_generate_prime(256, 30);
    A = bi_mod_power(bi_copy(g), bi_copy(a), bi_copy(p));

    string request;
    bi_to_str_base(request, bi_copy(A), 16);
    return request;
}

string
DiffieHellman :: generateResponse(const char* aPubKey)
{
    b = bi_generate_prime(256, 30);
    B = bi_mod_power(bi_copy(g), bi_copy(b), bi_copy(p));

    A = str_to_bi_base(format_bi_str(aPubKey), 16);
    s = bi_mod_power(bi_copy(A), bi_copy(b), bi_copy(p));

    string response;
    bi_to_str_base(response, bi_copy(B), 16);
    return response;
}

string
DiffieHellman :: handleResponse(const char* aPubKey)
{
    B = str_to_bi_base(format_bi_str(aPubKey), 16);
    s = bi_mod_power(bi_copy(B), bi_copy(a), bi_copy(p));

    string response;
    bi_to_str_base(response, bi_copy(s), 16);
    return response;
}

///////////////////////////////////////////////////
//// BigInt extension
///////////////////////////////////////////////////

static char* format_bi_str(const char* str)
{
    static char* buf = (char*)malloc(sizeof(int));

    size_t len = strlen(str);
    buf = (char*)realloc(buf, len + 1);

    size_t i = 0;
    for ( ; i < len; ++i)
    {
        buf[i] = tolower(str[i]);
    }
    buf[i] = '\0';

    return buf;
}

static void
bi_to_str_base(string& str, bigint bi, int base)
{
    str.clear();

    if (bi_is_negative(bi_copy(bi)))
    {
        str += '-';
        bi = bi_negate(bi);
    }

    bi_to_str_pos(str, bi, base);
}

static void
bi_to_str_pos(string& str, bigint bi, int base)
{
    if (bi_compare(bi_copy(bi), int_to_bi(base)) >= 0)
        bi_to_str_pos(str, bi_int_divide(bi_copy(bi), base), base);
    str += digit_to_char(bi_int_mod(bi, base));
}

static char
digit_to_char(int d)
{
    if (d >= 0 && d <= 9)
        return d + '0';
    else
        return d - 10 + 'A';
}
