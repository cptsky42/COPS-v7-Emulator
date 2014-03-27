/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "tcpserver.h"
#include "networkclient.h"
#include <algorithm> // find...

using namespace std;

TcpServer :: TcpServer(QObject* aParent)
    : QObject(aParent)
{
    mPort = 0;
    mIsListening = false;

    onConnect = nullptr;
    onReceive = nullptr;
    onDisconnect = nullptr;
}

TcpServer :: ~TcpServer()
{
    if (mIsListening)
    {
        mServer.close();
    }

    for (vector<NetworkClient*>::iterator
         it = mClients.begin(), end = mClients.end();
         it != end; ++it)
    {
        NetworkClient* client = *it;
        SAFE_DELETE(client);
    }
}

void
TcpServer :: listen(uint16_t aPort)
{
    if (!mIsListening)
    {
        mServer.listen(QHostAddress::Any, aPort);
        mPort = aPort;
        mIsListening = true;
    }
}

void
TcpServer :: accept()
{
    connect(&mServer, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
}

void
TcpServer :: incomingConnection()
{
    QTcpSocket* socket = mServer.nextPendingConnection();
    NetworkClient* client = new NetworkClient(*this, &socket);

    mClients.push_back(client);

    if (onConnect != nullptr)
    {
        onConnect(client);
    }
}

void
TcpServer :: disconnect(NetworkClient* aClient)
{
    //if (Client == nullptr || !Client.IsAlive)
    //    return;

    vector<NetworkClient*>::iterator it;
    it = find(mClients.begin(), mClients.end(), aClient);

    if (it != mClients.end())
    {
        mClients.erase(it);

        if (onDisconnect != nullptr)
        {
            onDisconnect(aClient);
        }

        // it may receives message after, so Qt will handle the memory.
        aClient->deleteLater();
    }
}
