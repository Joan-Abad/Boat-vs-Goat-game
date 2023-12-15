#BoatvsGoat

Boat vs Goat is a PVP game project where the objective of the player is to destroy the other pirate boats who are also players.

The map has multiple cannons that will be shooting at the players that they need to avoid. Also, there are 2 dangerous sharks that the player for sure will try to avoid. 

![BoatVsGoatGif](https://github.com/Joan-Abad/NetworkingSocketGame/assets/59310847/f18a7c89-798b-4b14-8c2b-2a900f198a1c)

This networking project has been done to put into practice my knowledge of networking. I've been using engines like Unreal Engine and Unity that have their networking framework, but I was interested in building my own. I decided to use SFML as I want to build the project with C++ and has some really useful networking features like TCP/UDP sockets and net packets built in. 

The map class is used to handle all the lifetime of the GameObjects of the game. All networking is handled by the game objects, and also the map can send network data. If we want to send data over the network, we only need to call the AddLocalNetworkDataToSend function with a key and the value we want to send. This function can be called from the client or the server, and the sent information will be received on the virtual functions UpdateClientNetData and UpdateServerData. 

A basic collision system is also managed in the Map base class function, as all actors live in it. 

The Manager's Folder controls almost all the application management. Networking sends and receives data is managed by the NetworkingManager class. In the managers folder, we also have Input, Font, Sound, Texture...

Each map has its Init function which initializes all the assets needed for the map. 

<img width="924" alt="Captura2" src="https://github.com/Joan-Abad/NetworkingSocketGame/assets/59310847/91415c2d-883f-41c3-8007-a56207e62185">

<img width="927" alt="Captura3" src="https://github.com/Joan-Abad/NetworkingSocketGame/assets/59310847/264c74e0-7ca4-4e0e-b80e-f4afbe80f661">

Assets used in this project:

* [SFML] used for graphics, input and networking
[SFML]: https://www.sfml-dev.org/
* [JsonCPP] used for packet serialization
[JsonCPP]
* [Kenney Pirate Pack]: for graphics
[Kenney Pirate Pack]: https://kenney-assets.itch.io/pirate-pack

