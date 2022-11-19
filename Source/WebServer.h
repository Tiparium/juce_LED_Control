/*
  ==============================================================================

    WebServer.h
    Created: 12 Nov 2022 3:43:03pm
    Author:  Sloan Kelly, modified by Tiparium

	Host a basic WebServer on a given IP and Port.

  ==============================================================================
*/

#pragma once
#include "TcpListener.h"

class WebServer : public TcpListener
{
public:

	WebServer(const char* ipAddress, int port, volatile bool* shouldRun) :
		TcpListener(ipAddress, port, shouldRun) { }

protected:

	// Handler for client connections
	virtual void onClientConnected(int clientSocket);

	// Handler for client disconnections
	virtual void onClientDisconnected(int clientSocket);

	// Handler for when a message is received from the client
	virtual void onMessageReceived(int clientSocket, const char* msg, int length);
};