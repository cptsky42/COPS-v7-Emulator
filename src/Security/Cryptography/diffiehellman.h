/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 * Copyright (C) 1993 Bruce Schneier
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_DIFFIE_HELLMAN_H_
#define _COPS_V7_EMULATOR_DIFFIE_HELLMAN_H_

#include "common.h"
#include <string>

extern "C" {
#include "bigint.h"
}

class DiffieHellman
{
public:
    DiffieHellman(char* aP, char* aG);
    ~DiffieHellman();

    std::string generateRequest();
    std::string generateResponse(char* aPubKey);
    std::string handleResponse(char* aPubKey);

private:
    bigint p;
    bigint g;
    bigint a;
    bigint b;
    bigint s;
    bigint A;
    bigint B;
};

#endif // _COPS_V7_EMULATOR_DIFFIE_HELLMAN_H_
