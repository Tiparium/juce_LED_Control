/*
  ==============================================================================

    MultiClientChat.cpp
    Created: 12 Nov 2022 3:43:03pm
    Author:  Sloan Kelly, modified by Tiparium

  ==============================================================================
*/

#include "WebServer.h"
#include <string>

void WebServer::onClientConnected(int clientSocket)
{
	// Send a welcome message to the connected client
	std::string welcomeMsg = "Welcome to the Awesome Chat Server!\r\n";
	sendToClient(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1);
}

void WebServer::onClientDisconnected(int clientSocket)
{

}

void WebServer::onMessageReceived(int clientSocket, const char* msg, int length)
{
	broadcastToClients(clientSocket, msg, length);
}