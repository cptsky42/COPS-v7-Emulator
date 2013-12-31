/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include <QtCore/QCoreApplication>

#include "log.h"
#include "common.h"
#include "server.h"

extern "C" {
#include "bigint.h"
}

#include <stdlib.h> // srandom()
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    #ifdef _WIN32
    SetConsoleTitleA("COPS v7 Emulator for " TARGET_SYSTEM " (" TARGET_ARCH ") by Jean-Philippe Boivin (alias CptSky)");
    #endif

    fprintf(stdout, "COPS v7 Emulator by Jean-Philippe Boivin (alias CptSky)...\n");
    fprintf(stdout, "Build for %s (%s), using Qt %s, the %s...\n",
            TARGET_SYSTEM, TARGET_ARCH, QT_VERSION_STR, __TIMESTAMP__);
    fprintf(stdout, "\n");

    // Initialize the BigInt package
    LOG(INFO, "Initializing the BigInt package...");
    bi_initialize();
    srand(time(NULL)); // for generatePrime() call

    const Server& server = Server::getInstance();
    int result = app.exec();

    // Terminate the BigInt package
    LOG(INFO, "Destroying the BigInt package...");
    bi_terminate();

    return result;
}
