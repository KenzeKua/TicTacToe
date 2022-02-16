#include "MyPhoton.h"

#include <iostream>
#include <string>

// Kenze's ID: 9d4ccadb-bb50-416b-a840-482ee37f78e8
// Victor's ID: ca1729c5-d695-4c91-b708-963bcf94fca6
// Chan Zhe's ID: 88eab4cf-e716-4fd5-a3cf-109ca086f23f

static const ExitGames::Common::JString appId = L"9d4ccadb-bb50-416b-a840-482ee37f78e8"; // Set your app id here
static const ExitGames::Common::JString appVersion = L"1.0";
static ExitGames::Common::JString PLAYER_NAME = L"Kemze";

static char CharData[123] = "Aidan wants to eat Paimon!";

static std::string KeyLock = "69";
static int IntKeyLock = std::stoi(KeyLock);

//LoadBalancing::Listener& listener, const Common::JString& applicationID, 
//const Common::JString& appVersion, nByte connectionProtocol=Photon::ConnectionProtocol::DEFAULT, bool autoLobbyStats=false, nByte regionSelectionMode=RegionSelectionMode::DEFAULT

// Functions
MyPhoton::MyPhoton() : mLoadBalancingClient(*this, appId, appVersion, ExitGames::Photon::ConnectionProtocol::TCP, false, ExitGames::LoadBalancing::RegionSelectionMode::SELECT)
{
	std::string InputDetails;

	std::cout << "\n Fill in Details \n";
	std::cout << "-===============-\n\n";
	std::cout << "Please input name: ";
	getline(std::cin, InputDetails);
	PLAYER_NAME = ExitGames::Common::JString(InputDetails.c_str());

	std::cout << "Please write an intro [123 characters]: ";
	std::cin.getline(CharData, 123);
	std::cin.clear();
}

void MyPhoton::connect(void)
{
	std::wcout << "Connecting..." << std::endl;
	
	mLoadBalancingClient.connect(ExitGames::LoadBalancing::AuthenticationValues(), PLAYER_NAME);
}

void MyPhoton::disconnect(void)
{
	std::wcout << "Disconnecting..." << std::endl;
	mLoadBalancingClient.disconnect();
}

void MyPhoton::SetTicTacToe(TicTacToe* TheGame)
{
	ReferenceGame = TheGame;
}

void MyPhoton::SendUID(void)
{
	float FloatData[4] { 0 };
	FloatData[0] = rand() % 9 + 1.0f;
	FloatData[1] = rand() % 9 + 1.0f;
	FloatData[2] = rand() % 9 + 1.0f;
	FloatData[3] = rand() % 9 + 1.0f;

	std::cout << "Your UID: ";
	for (int i = 0; i < 4; i++)
	{
		std::cout << FloatData[i];
	}
	std::cout << "\n\n";

	mLoadBalancingClient.opRaiseEvent(true, FloatData, 4, 1);
}
void MyPhoton::SendIntro(void)
{
	ExitGames::Common::JString ConvertData(CharData);

	mLoadBalancingClient.opRaiseEvent(true, ConvertData, 2);
}
void MyPhoton::SendNameList(void)
{
	ExitGames::Common::JString ConvertData(NameList->c_str());

	mLoadBalancingClient.opRaiseEvent(true, ConvertData, 3);
}
void MyPhoton::SendStringMessage(std::string stringData)
{
	std::string EncryptMessage = stringData;
	for (int i = 0; i < EncryptMessage.length(); i++)
	{
		EncryptMessage[i] ^= IntKeyLock;
	}

	ExitGames::Common::JString ConvertData(EncryptMessage.c_str());

	mLoadBalancingClient.opRaiseEvent(true, ConvertData, 4);
}
void MyPhoton::SendByteMessage(const char* cstring, int stringSize)
{
	//std::cout << "Sending byte of size: " << stringSize << std::endl;
	const unsigned char *bytes = (const unsigned char*)cstring;
	mLoadBalancingClient.opRaiseEvent(true, bytes, stringSize, 5);
}
void MyPhoton::SendGameAnswer(int PlayerAnswer)
{
	ReferenceGame->InputAnswer(PlayerAnswer);

	mLoadBalancingClient.opRaiseEvent(true, PlayerAnswer, 6);
}

void MyPhoton::run(void)
{
	mLoadBalancingClient.service();
}

// Protocol implementations
void MyPhoton::debugReturn(int debugLevel, const ExitGames::Common::JString& string)
{
}

void MyPhoton::connectionErrorReturn(int errorCode)
{
	std::wcout << "Connection error return: " << errorCode << std::endl;
}

void MyPhoton::clientErrorReturn(int errorCode)
{
	std::wcout << "Client error return: " << errorCode << std::endl;
}

void MyPhoton::warningReturn(int warningCode)
{
	std::wcout << "Warning return: " << warningCode << std::endl;
}

void MyPhoton::serverErrorReturn(int errorCode)
{
	std::wcout << "Server error return: " << errorCode << std::endl;
}

void MyPhoton::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int> &playernrs, const ExitGames::LoadBalancing::Player& player)
{
	std::wcout << player.getName() << ", Player " << playerNr << " has joined the room.\n\n";
	NameList[playerNr - 1] = player.getName().UTF8Representation();

	ReferenceGame->MarkPlayer(playerNr);

	if (playernrs.getSize() >= 2)
	{
		if (ReferenceGame->PlayerNumber == 1)
		{
			SendNameList();
		}
		SendUID();
		SendIntro();
	}
}

void MyPhoton::leaveRoomEventAction(int playerNr, bool isInactive)
{
	std::cout << NameList[playerNr - 1] << ", Player " << playerNr << " has left the room.\n\n";
}

void MyPhoton::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object &eventContent)
{
	if (eventCode == 1)
	{
		float *floatData = ExitGames::Common::ValueObject<float *>(eventContent).getDataCopy();
		if (floatData)
		{
			std::cout << NameList[playerNr - 1] << "'s UID: ";
			for (int i = 0; i < 4; i++)
			{
				std::cout << floatData[i];
			}
			std::wcout << std::endl;
		}
		else
			std::wcout << "Invalid float data\n";
	}
	else if (eventCode == 2)
	{
		ExitGames::Common::JString CharData = ExitGames::Common::ValueObject<ExitGames::Common::JString>(eventContent).getDataCopy();
		if (CharData)
		{
			std::cout << NameList[playerNr - 1] << "'s intro: ";
			std::wcout << CharData << "\n\n";
		}
		else
			std::wcout << "Invalid char data\n";
	}
	else if (eventCode == 3)
	{
		ExitGames::Common::JString NameListData = ExitGames::Common::ValueObject<ExitGames::Common::JString>(eventContent).getDataCopy();
		if (NameListData)
			*NameList = NameListData.UTF8Representation();
		else
			std::wcout << "Invalid name list\n";
	}
	else if (eventCode == 4)
	{
		ExitGames::Common::JString stringData = ExitGames::Common::ValueObject<ExitGames::Common::JString>(eventContent).getDataCopy();
		std::string decryptMessage = stringData.UTF8Representation().cstr();
		//std::string decryptMessage = stringData.ANSIRepresentation().cstr();
		if (stringData)
		{
			for (int i = 0; i < decryptMessage.length(); i++)
			{
				decryptMessage[i] ^= IntKeyLock;
			}

			std::cout << NameList[playerNr - 1] << ": " << decryptMessage << std::endl;
		}
		else
			std::wcout << "Invalid string data\n";
	}
	else if (eventCode == 5)
	{
		unsigned char *data = ExitGames::Common::ValueObject<unsigned char *>(eventContent).getDataCopy();
		const int *byteNum = ExitGames::Common::ValueObject<unsigned char *>(eventContent).getSizes();
		if (data)
		{
			//std::cout << "Size: " << byteNum[0] << std::endl;

			char *string = (char *)data;
			//std::cout << "Before decryption, Player " << playerNr << ": " << string << std::endl;

			for (int i = 0; i < byteNum[0]; i++)
			{
				data[i] = data[i] ^ CharCipherKey;
			}
			data[byteNum[0]] = '\0';

			std::cout << NameList[playerNr - 1] << ": " << data << std::endl;
		}
		else
			std::wcout << "Invalid byte data\n";
	}
	else if (eventCode == 6)
	{
		int gameData = ExitGames::Common::ValueObject<int>(eventContent).getDataCopy();
		ReferenceGame->FetchAnswer(playerNr, gameData);
	}
}

void MyPhoton::connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& cluster)
{
	if (errorCode)
	{
		std::wcout << "Failed to connect: " << errorString.cstr() << std::endl;
	}
	else
	{
		std::wcout << "Connected to Photon Server." << std::endl;

		std::wcout << "Trying to join a random room..." << std::endl;
		mLoadBalancingClient.opJoinRandomRoom();
	}
}

void MyPhoton::disconnectReturn(void)
{
	std::wcout << "Disconnected" << std::endl;
}

void MyPhoton::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout << "Create room return" << std::endl;
}

void MyPhoton::joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout << "Join or create room return" << std::endl;
}

void MyPhoton::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout << "Join room return" << std::endl;
}

void MyPhoton::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout << "Join random room return" << std::endl;

	if (errorCode)
	{
		std::wcout << "Failed to join a random room: " << errorCode << ", " << errorString.cstr() << std::endl;

		if (errorCode == 32760) //no match found error code
		{
			std::wcout << "Creating a room..." << std::endl;
			// Try to create a room and wait;
			ExitGames::Common::JString name;
			name += GETTIMEMS();
			mLoadBalancingClient.opCreateRoom(name, ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(20));
		}

		return;
	}
}

void MyPhoton::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout << "Leave room return" << std::endl;
}

void MyPhoton::joinLobbyReturn(void)
{
	std::wcout << "Join lobby return" << std::endl;
}

void MyPhoton::leaveLobbyReturn(void)
{
	std::wcout << "Leave lobby return" << std::endl;
}

void MyPhoton::onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
	std::wcout << "On lobby stats response" << std::endl;
}

void MyPhoton::onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
	std::wcout << "On lobby stats update" << std::endl;
}

void MyPhoton::onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers)
{
	//std::wcout<<availableRegions[0]<<std::endl;
	//std::wcout<<availableRegions[1]<<std::endl;
	//mLoadBalancingClient.selectRegion(availableRegions[0]);
	mLoadBalancingClient.selectRegion("asia");
}
