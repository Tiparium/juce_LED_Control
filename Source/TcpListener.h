/*
  ==============================================================================

    TcpListener.h
    Created: 11 Nov 2022 6:26:08pm
    Author:  Sloan Kelly, modified by Tiparium

    Courtesy of this toturial:
        https://www.youtube.com/watch?v=Kc1kwm1WyVM&t=1s&ab_channel=SloanKelly
		Lots of modification to works friendly in a threaded environment by me.
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>

#pragma comment (lib, "ws2_32.lib")

class TcpListener
{

public:

	TcpListener(const char* ipAddress, int port, volatile bool* shouldRun) :
		_ipAddress(ipAddress), 
		_port(port),
		_shouldRun(shouldRun)
	{ }

	// Initialize the listener
	int init();

	// Run the listener
	int run();

protected:

	// Handler for client connections
	virtual void onClientConnected(int clientSocket);

	// Handler for client disconnections
	virtual void onClientDisconnected(int clientSocket);

	// Handler for when a message is received from the client
	virtual void onMessageReceived(int clientSocket, const char* msg, int length);

	// Send a message to a client
	void sendToClient(int clientSocket, const char* msg, int length);

	// Broadcast a message from a client
	void broadcastToClients(int sendingClient, const char* msg, int length);

private:

	volatile bool*  _shouldRun;
	const char*		_ipAddress;	// IP Address server will run on
	int				_port;			// Port # for the web service
	int				_socket;		// Internal FD for the listening socket
	fd_set			_master;		// Master file descriptor set
};