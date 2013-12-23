/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "networkclient.h"
#include "tcpserver.h"
#include <QObject>
#include <QTcpSocket>

using namespace std;

NetworkClient :: NetworkClient(TcpServer& aServer, QTcpSocket** aSocket, QObject* aParent)
    : QObject(aParent), mServer(aServer)
{
    mSocket = *aSocket;
    mSocket->setParent(this);
    *aSocket = nullptr;

    mOwner = nullptr;

    connect(mSocket, SIGNAL(readyRead()), this, SLOT(incomingData()));
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(incomingDisconnection()));
}

NetworkClient :: ~NetworkClient()
{
    mSocket->close();
    SAFE_DELETE(mSocket);
}

void
NetworkClient :: send(uint8_t* aBuf, size_t aLen)
{
    mSocket->write((char*)aBuf, aLen);
}

void
NetworkClient :: disconnect()
{
    mServer.disconnect(this);
}

void
NetworkClient :: incomingData()
{
    uint8_t buf[NetworkClient::BUFFER_SIZE] = { 0 };
    size_t len = mSocket->read((char*)buf, mSocket->bytesAvailable());

    if (mServer.onReceive != nullptr)
    {
        mServer.onReceive(this, buf, len);
    }
}

void
NetworkClient :: incomingDisconnection()
{
    mServer.disconnect(this);
}
