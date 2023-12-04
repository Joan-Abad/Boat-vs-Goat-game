#pragma once
#include <SFML/Network.hpp>
#include <mutex>
#include <memory>
#include <vector>

class NetworkingManager
{
public:
    template<typename NetworkManager>
    static NetworkingManager& GetNetworkingManager()
    {
        std::call_once(onceFlag, []() {
            instance = new NetworkManager();
            });

        return *instance;
    }

    virtual void Bind(int port) = 0;
    virtual void SendPackages();
    virtual void RecievePackage() = 0;
    virtual void ListenIncomingPackages();
    void AddPacketToSend(sf::Packet packet);

protected:
    NetworkingManager();
    ~NetworkingManager();
    sf::UdpSocket* socket;
    int port; 
    std::vector<sf::Packet> packetsToSend;
private:
    static std::once_flag onceFlag;
    static NetworkingManager* instance;

    NetworkingManager(const NetworkingManager&) = delete;
    NetworkingManager& operator=(const NetworkingManager&) = delete;
};