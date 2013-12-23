/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_TCP_SERVER_H
#define _COPS_V7_EMULATOR_TCP_SERVER_H

#include "common.h"
#include <QtNetwork>
#include <QTcpServer>
#include <vector>

class NetworkClient;
class QObject;

/** Delegate function called when a new client is connected to the server. */
typedef void(*TCP_CONNECT)(NetworkClient* aClient);
/** Delegate function called when the server is received new data from a client. */
typedef void(*TCP_RECEIVE)(NetworkClient* aClient, uint8_t* aBuf, size_t aLen);
/** Delegate function called when a new client is disconnected from the server. */
typedef void(*TCP_DISCONNECT)(NetworkClient* aClient);

/**
 * A tcp server.
 */
class TcpServer : private QObject
{
    Q_OBJECT
public:
    /**
     * Create a new tcp server.
     * @param[in]      aParent    (Optional) the parent Qt object of the client
     *                            Default value is nullptr.
     */
    TcpServer(QObject* aParent = nullptr);

    /**
     * Destroy the tcp server and close all the connections.
     */
    ~TcpServer();

    /**
     * Start listening on the specified port.
     *
     * @param[in]      aPort     The port to listen on.
     */
    void listen(uint16_t aPort);

    /**
     * Start accepting new connections.
     */
    void accept();

    /**
     * Disconnect the specified client.
     *
     * @param[in]      aClient     The client to disconnect
     */
    void disconnect(NetworkClient* aClient);

public:
    /** Get the port on which the server is listening. */
    uint16_t getPort() const { return mPort; }

public:
    TCP_CONNECT onConnect; //!< The delegate function to call on new connection
    TCP_RECEIVE onReceive; //!< The delegate function to call when receiving data
    TCP_DISCONNECT onDisconnect; //!< The delegate function to call on disconnection

private slots:
    /* Incoming connection event */
    void incomingConnection();

private:
    QTcpServer mServer; //!< the tcp server
    uint16_t mPort; //!< the listening port
    bool mIsListening; //!< the server is currently listening

    std::vector<NetworkClient*> mClients; //!< the list of all connected clients
};

#endif // _COPS_V7_EMULATOR_TCP_SERVER_H
