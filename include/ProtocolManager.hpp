//
//  ProtocolManager.hpp
//  hunter_network
//
//  Created by Thomas Fournier on 26/02/2020.
//
//

#ifndef ProtocolManager_hpp
#define ProtocolManager_hpp

#include <stdio.h>
#include <list>
#include <unordered_map>
#include <SFML/Network.hpp>


class NetworkConnection;
class Game;
class Client;
class Server;
class ProtocolManager;

class ProtocolManager {
private:
public:

    // void (ProtocolManager::*fptr_servers)(Server &);
    // void (ProtocolManager::*fptr_client)(Client  &);
    enum Protocol {
        CLIENT_MOUSEMOVE_SEND,
        CLIENT_SHOT_SEND,
        CLIENT_NEWPLAYER_RECEIVE,
        SERVER_ID_RESPONSE,
        SERVER_NEWPLAYER_SEND,
        SERVER_PLAYERLEFT_SEND,
        SERVER_BIRDSPAWN_SEND,
        SERVER_BIRDDIE_SEND,
        SERVER_PAUSE_SEND,
        SERVER_STARTGAME_SEND,
        SERVER_ENDGAME_SEND,
        SERVER_MOUSEMOVE_RESPONSE,
    };
    ProtocolManager();
    ~ProtocolManager();

    void ClientMouseMoveSend(Server &s, sf::Packet &p);
    void ClientShotSend(Server &s, sf::Packet &p);


    void ServerIdResponse(Client &c, sf::Packet &p);
    void ServerMouseMoveResponse(Client &s, sf::Packet &p);
    void ServerPlayerLeftSend(Client &c, sf::Packet &p);
    void ServerNewPlayerSend(Client &c, sf::Packet &p);
    void ServerBirdDieSend(Client &c, sf::Packet &p);
    void ServerBirdSpawnSend(Client &c, sf::Packet &p);
    void ServerStartGameSend(Client &c, sf::Packet &p);
    void ServerPauseGameSend(Client &c, sf::Packet &p);
    void ServerEndGameSend(Client &c, sf::Packet &p);



    std::unordered_map<Protocol, void (ProtocolManager::*)(Server &s, sf::Packet &p)> _sfptr;
    std::unordered_map<Protocol, void (ProtocolManager::*)(Client &c, sf::Packet &p)> _cfptr;
protected:

};


#endif /* ProtocolManager_hpp */
