/*
  ==============================================================================

    MultiClientChat.h
    Created: 12 Nov 2022 3:43:03pm
    Author:  Sloan Kelly, modified by Tiparium

  ==============================================================================
*/

#pragma once
#include "TcpListener.h"

class MultiClientChat : public TcpListener
{
public:

	MultiClientChat(const char* ipAddress, int port, volatile bool* shouldRun) :
		TcpListener(ipAddress, port, shouldRun) { }

protected:

	// Handler for client connections
	virtual void onClientConnected(int clientSocket);

	// Handler for client disconnections
	virtual void onClientDisconnected(int clientSocket);

	// Handler for when a message is received from the client
	virtual void onMessageReceived(int clientSocket, const char* msg, int length);
};