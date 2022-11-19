/*
  ==============================================================================

    TcpListener.cpp
    Created: 11 Nov 2022 6:26:08pm
    Author:  Sloan Kelly, modified by Tiparium

  ==============================================================================
*/

#include "TcpListener.h"

int TcpListener::init()
{
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		return wsOk;
	}

	// Create a socket
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == INVALID_SOCKET)
	{
		bigDebug(false, "Failed to create a socket : Error: ", WSAGetLastError());
		return WSAGetLastError();
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(_port);
	inet_pton(AF_INET, _ipAddress, &hint.sin_addr);

	if (bind(_socket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
	{
		bigDebug(false, "Failed to bind IP & port to Socket: Error: ", WSAGetLastError());
		return WSAGetLastError();
	}

	// Tell Winsock the socket is for listening 
	if (listen(_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		bigDebug(false, "Failed to tell WINSOCK socket is for listening: Error: ", WSAGetLastError());
		return WSAGetLastError();
	}

	// Create the master file descriptor set and zero it
	FD_ZERO(&_master);

	// Add our first socket that we're interested in interacting with; the listening socket!
	// It's important that this socket is added for our server or else we won't 'hear' incoming
	// connections 
	FD_SET(_socket, &_master);

	bigDebug(true, "SUCCESS! SERVER IS ACTIVE!", 0);

	return 0;
}

int TcpListener::run()
{
	while (*_shouldRun)
	{
		fd_set copy = _master;
		

		// See who's talking to us
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		// Loop through all the current connections / potential connect
		for (int i = 0; i < socketCount; i++)
		{
			// Makes things easy for us doing this assignment
			SOCKET sock = copy.fd_array[i];

			// Is it an inbound communication?
			if (sock == _socket)
			{
				// Accept a new connection
				SOCKET client = accept(_socket, nullptr, nullptr);

				// Add the new connection to the list of connected clients
				FD_SET(client, &_master);

				onClientConnected(client);
			}
			else // It's an inbound message
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				// Receive message
				int bytesIn = recv(sock, buf, 4096, 0);
				if (bytesIn <= 0)
				{
					// Drop the client
					onClientDisconnected(sock);
					closesocket(sock);
					FD_CLR(sock, &_master);
				}
				else
				{
					onMessageReceived(sock, buf, bytesIn);
				}
			}
		}
	}

	// Remove the listening socket from the master file descriptor set and close it
	// to prevent anyone else trying to connect.
	FD_CLR(_socket, &_master);
	closesocket(_socket);

	while (_master.fd_count > 0)
	{
		// Get the socket number
		SOCKET sock = _master.fd_array[0];

		// Remove it from the master file list and close the socket
		FD_CLR(sock, &_master);
		closesocket(sock);
	}

	// Cleanup winsock
	WSACleanup();
	return 0;
}


void TcpListener::sendToClient(int clientSocket, const char* msg, int length)
{
	send(clientSocket, msg, length, 0);
}

void TcpListener::broadcastToClients(int sendingClient, const char* msg, int length)
{
	for (int i = 0; i < _master.fd_count; i++)
	{
		SOCKET outSock = _master.fd_array[i];
		if (outSock != _socket && outSock != sendingClient)
		{
			sendToClient(outSock, msg, length);
		}
	}
}

void TcpListener::onClientConnected(int clientSocket)
{

}

void TcpListener::onClientDisconnected(int clientSocket)
{

}

void TcpListener::onMessageReceived(int clientSocket, const char* msg, int length)
{

}

//DBG
void TcpListener::bigDebug(bool goodMessage, juce::String message, int error)
{
	juce::String borders = goodMessage ? "********************************************" : "---------------------------------------------";
	for (int i = 0; i < 3; i++) { DBG(borders); }
	DBG(message);
	DBG(error);
	for (int i = 0; i < 3; i++) { DBG(borders); }
}
//*DBG