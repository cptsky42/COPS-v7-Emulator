#include "diffiehellman.h"

using namespace std;

static void bi_to_str_base(string& str, bigint bi, int base);
static void bi_to_str_pos(string& str, bigint bi, int base);
static char digit_to_char(int d);

DiffieHellman :: DiffieHellman(char* aP, char* aG)
    : p(nullptr), g(nullptr),
      a(nullptr), b(nullptr),
      s(nullptr), A(nullptr), B(nullptr)
{
    p = str_to_bi_base(aP, 16);
    g = str_to_bi_base(aG, 16);
}

DiffieHellman :: ~DiffieHellman()
{
    bi_free(p); bi_free(g);
    bi_free(a); bi_free(b);
    bi_free(s); bi_free(A); bi_free(B);
}

string
DiffieHellman :: generateRequest()
{
    a = bi_generate_prime(256, 30);
    A = bi_mod_power(g, a, p);

    string request;
    bi_to_str_base(request, A, 16);
    return request;
}

string
DiffieHellman :: generateResponse(char* aPubKey)
{
    b = bi_generate_prime(256, 30);
    B = bi_mod_power(g, b, p);

    A = str_to_bi_base(aPubKey, 16);
    s = bi_mod_power(A, b, p);

    string response;
    bi_to_str_base(response, B, 16);
    return response;
}

void
DiffieHellman :: handleResponse(char* aPubKey)
{
    B = str_to_bi_base(aPubKey, 16);
    s = bi_mod_power(B, a, p);
}

///////////////////////////////////////////////////
//// BigInt extension
///////////////////////////////////////////////////

static void
bi_to_str_base(string& str, bigint bi, int base)
{
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
