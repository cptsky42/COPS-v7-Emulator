/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "common.h"
#include "server.h"

int main(int argc, char* argv[])
{
    Server server(argc, argv);
    return server.exec();
}
