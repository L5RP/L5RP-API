#include "api.h"

using namespace L5RP;

L5RP::Player* fnCreatePlayerDefault(L5RP::PlayerManager* manager, rage::IPlayer* player)
{
    return new L5RP::Player(manager, player);
}

L5RP::PlayerManager::PlayerManager(Plugin* plugin)
    : SmartEntityManager(plugin)
{
    plugin->getRageMP()->AddEventHandler(this);
}

void L5RP::PlayerManager::registerDefaultComponent(
    std::function<PlayerComponent*(Player*)> fn
)
{
    SmartEntityManager::registerDefaultComponent(
        [fn](
            Entity* entity
        )->EntityComponent*
        {
            return (EntityComponent*)
                fn((Player*) entity);
        }
    );
}

rage::IPlayerHandler* L5RP::PlayerManager::GetPlayerHandler()
{
    return this;
}

void L5RP::PlayerManager::OnPlayerJoin(rage::IPlayer* player)
{

    if(this->getPlugin()->isInitialized())
        new Player(this, player);

    else {
        player->OutputChatBox(
            u8"Serveris šiuo metu nepriima žaidėjų! Bandykite prisijungti po kelių sekundžių!"
        );

        player->Kick("heheheheheh");
    }


}

void L5RP::PlayerManager::OnPlayerReady(rage::IPlayer* player)
{
    if(this->getPlugin()->isInitialized()) {
        rage::entityId_t networkId = player->GetId();
        Player* _player = this->getPlayer(networkId);

        if(_player != nullptr)
            _player->OnReady();

        else
            Console::getInstance()->warn("OnPlayerReady was triggered with unknown Player!");
    }
}

void L5RP::PlayerManager::OnPlayerQuit(rage::IPlayer* player, rage::exit_t exitType, const char* reason)
{
    if(this->getPlugin()->isInitialized()) {
        rage::entityId_t networkId = player->GetId();
        Player* _player = this->getPlayer(networkId);

        if(_player != nullptr) {
            _player->OnDisconnect(exitType, reason);

            delete _player;
        } else
            Console::getInstance()->warn("OnPlayerQuit was triggered with unknown player!");
    }
}

void L5RP::PlayerManager::OnPlayerSpawn(rage::IPlayer* player)
{
    if(this->getPlugin()->isInitialized()) {
        rage::entityId_t networkId = player->GetId();
        Player* _player = this->getPlayer(networkId);

        if(_player != nullptr)
            _player->OnSpawn();

        else
            Console::getInstance()->warn("OnPlayerSpawn was triggered with unknown player!");
    }
}

void L5RP::PlayerManager::OnPlayerCommand(rage::IPlayer* _player, const std::u16string& _cmd)
{
    if(this->getPlugin()->isInitialized()) {

        if(_cmd.length() > 100)
            Console::getInstance()->warn(
                "OnPlayerCommand was triggered with too long command message! This event won't be handled"
            );

        else {
            std::string cmd = UTF16_TO_UTF8(_cmd);

            std::string cmdName;
            std::string cmdArgs;

            size_t it = cmd.find(' ');
            if(it == std::string::npos)
                cmdName = cmd;

            else {
                cmdName = cmd.substr(0, it);
                cmdArgs = cmd.substr(it + 1, cmd.size());
            }

            rage::entityId_t networkId = _player->GetId();
            Player* player = this->getPlayer(networkId);

            if(player != nullptr)
                player->OnCommand(cmdName, cmdArgs);

            else
                Console::getInstance()->warn("OnPlayerCommand was triggered with unknown player!");
        }
    }
}

void L5RP::PlayerManager::OnPlayerChat(rage::IPlayer* player, const std::u16string& _text)
{
    if(this->getPlugin()->isInitialized()) {

        if(_text.length() > 100)
            Console::getInstance()->warn(
                "OnPlayerChat was triggered with too long chat message! This event won't be handled"
            );

        else {
            std::string text = UTF16_TO_UTF8(_text);

            text = std::regex_replace(text, std::regex("\\\\"), "\\\\");
            text = std::regex_replace(text, std::regex("\\\""), "\\\"");

            rage::entityId_t networkId = player->GetId();
            Player* _player = this->getPlayer(networkId);

            if(_player != nullptr)
                _player->OnMessage(text);

            else
                Console::getInstance()->warn("OnPlayerChat was triggered with unknown player!");
        }
    }
}

void L5RP::PlayerManager::OnPlayerStartEnterVehicle(rage::IPlayer* player, rage::IVehicle* vehicle, uint8_t seatId)
{
    if(this->getPlugin()->isInitialized()) {
        rage::entityId_t playerNetworkId = player->GetId();
        Player* _player = this->getPlayer(playerNetworkId);

        rage::entityId_t vehicleNetworkId = vehicle->GetId();
        Vehicle* _vehicle = this->getPlugin()->getVehicleManager()->getVehicle(vehicleNetworkId);

        if(_vehicle == nullptr)
            Console::getInstance()->warn("OnPlayerStartEnterVehicle was triggered with unknown vehicle");

        else if(_player == nullptr)
            Console::getInstance()->warn("OnPlayerStartEnterVehicle was triggered with unknown vehicle");

        else
            _player->OnStartEnterVehicle(_vehicle, seatId);
    }
}

void L5RP::PlayerManager::OnPlayerEnterVehicle(rage::IPlayer* player, rage::IVehicle* vehicle, uint8_t seatId)
{
    if(this->getPlugin()->isInitialized()) {
        rage::entityId_t playerNetworkId = player->GetId();
        Player* _player = this->getPlayer(playerNetworkId);

        rage::entityId_t vehicleNetworkId = vehicle->GetId();
        Vehicle* _vehicle = this->getPlugin()->getVehicleManager()->getVehicle(vehicleNetworkId);

        if(_vehicle == nullptr)
            Console::getInstance()->warn("OnPlayerEnterVehicle was triggered with unknown vehicle");

        else if(_player == nullptr)
            Console::getInstance()->warn("OnPlayerEnterVehicle was triggered with unknown vehicle");

        else
            _player->OnEnterVehicle(_vehicle, seatId);
    }
}

void L5RP::PlayerManager::OnPlayerStartExitVehicle(rage::IPlayer* player, rage::IVehicle* vehicle)
{
    if(this->getPlugin()->isInitialized()) {
        rage::entityId_t playerNetworkId = player->GetId();
        Player* _player = this->getPlayer(playerNetworkId);

        rage::entityId_t vehicleNetworkId = vehicle->GetId();
        Vehicle* _vehicle = this->getPlugin()->getVehicleManager()->getVehicle(vehicleNetworkId);

        if(_vehicle == nullptr)
            Console::getInstance()->warn("OnPlayerStartExitVehicle was triggered with unknown vehicle");

        else if(_player == nullptr)
            Console::getInstance()->warn("OnPlayerStartExitVehicle was triggered with unknown vehicle");

        else
            _player->OnStartExitVehicle(_vehicle);
    }
}

void L5RP::PlayerManager::OnPlayerExitVehicle(rage::IPlayer* player, rage::IVehicle* vehicle)
{
    if(this->getPlugin()->isInitialized()) {
        rage::entityId_t playerNetworkId = player->GetId();
        Player* _player = this->getPlayer(playerNetworkId);

        rage::entityId_t vehicleNetworkId = vehicle->GetId();
        Vehicle* _vehicle = this->getPlugin()->getVehicleManager()->getVehicle(vehicleNetworkId);

        if(_vehicle == nullptr)
            Console::getInstance()->warn("OnPlayerExitVehicle was triggered with unknown vehicle");

        else if(_player == nullptr)
            Console::getInstance()->warn("OnPlayerExitVehicle was triggered with unknown vehicle");

        else
            _player->OnExitVehicle(_vehicle);
    }
}

void L5RP::PlayerManager::OnPlayerDeath(rage::IPlayer* player, rage::hash_t reason, rage::IPlayer* killer)
{
    if(this->getPlugin()->isInitialized()) {
        rage::entityId_t networkId1 = player->GetId();
        Player* _player = this->getPlayer(networkId1);

        Player* _killer = nullptr;
        if(killer != nullptr) {
            rage::entityId_t networkId2 = killer->GetId();
            _killer = this->getPlayer(networkId2);

            if(_killer == nullptr)
                Console::getInstance()->warn("OnPlayerDeath was triggered with unknown killed");
        }

        if(_player == nullptr)
            Console::getInstance()->warn("OnPlayerDeath was triggered with unknown player");

        else
            _player->OnDeath(reason, _killer);
    }
}

rage::ICheckpointHandler* PlayerManager::GetCheckpointHandler()
{
    return this;
}

void PlayerManager::OnPlayerEnterCheckpoint(rage::IPlayer* player, rage::ICheckpoint* checkpoint)
{
    if(this->getPlugin()->isInitialized()) {
        rage::entityId_t playerNetworkId = player->GetId();
        Player* _player = this->getPlayer(playerNetworkId);

        rage::entityId_t checkPointNetworkId = checkpoint->GetId();
        CheckPoint* _checkPoint = this->getPlugin()->getCheckPointManager()->getCheckpoint(checkPointNetworkId);

        if(_player == nullptr)
            Console::getInstance()->warn("OnPlayerEnterCheckpoint was triggered with unknown player");

        else if(_checkPoint == nullptr)
            Console::getInstance()->warn("OnPlayerEventCheckpoint was triggered with unknown checkpoint");

        else
            _player->OnEnterCheckPoint(_checkPoint);
    }
}

void PlayerManager::OnPlayerExitCheckpoint(rage::IPlayer* player, rage::ICheckpoint* checkpoint)
{
    if(this->getPlugin()->isInitialized()) {
        rage::entityId_t playerNetworkId = player->GetId();
        Player* _player = this->getPlayer(playerNetworkId);

        rage::entityId_t checkPointNetworkId = checkpoint->GetId();
        CheckPoint* _checkPoint = this->getPlugin()->getCheckPointManager()->getCheckpoint(checkPointNetworkId);

        if(_player == nullptr)
            Console::getInstance()->warn("OnPlayerExitCheckpoint was triggered with unknown player");

        else if(_checkPoint == nullptr)
            Console::getInstance()->warn("OnPlayerExitCheckpoint was triggered with unknown checkpoint");

        else
            _player->OnExitCheckPoint(_checkPoint);
    }
}

void
L5RP::PlayerManager::OnPlayerRemoteEvent(rage::IPlayer* _player, uint64_t eventName, const rage::args_t& args)
{
    if(this->getPlugin()->isInitialized()) {
        rage::entityId_t networkId1 = _player->GetId();
        Player* player = this->getPlayer(networkId1);

        std::vector<std::string> arguments;
        for(rage::arg_t const& arg : args)
            switch(arg.GetType()) {

                case rage::arg_t::val_t::Int:
                    arguments.push_back(std::to_string(arg.Int()));
                    break;

                case rage::arg_t::val_t::Float:
                    arguments.push_back(std::to_string(arg.Float()));
                    break;

                case rage::arg_t::val_t::String:
                    arguments.push_back(std::string(arg.String()));
                    break;

                case rage::arg_t::val_t::Boolean:
                    arguments.push_back(std::to_string(arg.Boolean()));
                    break;

                case rage::arg_t::val_t::Null:
                    arguments.push_back(std::string(""));
                    break;

                case rage::arg_t::val_t::Entity:
                    arguments.push_back(std::to_string(arg.Entity()->GetId()));
                    break;

                case rage::arg_t::val_t::Object:
                    arguments.push_back(std::string(arg.Object()));
                    break;

            }

        if(player == nullptr)
            Console::getInstance()->warn("OnPlayerRemoteEvent was triggered with unknown player!");

        else
            player->OnRemoteEvent(eventName, arguments);
    }
}

void PlayerManager::sendGlobalMessage(std::string const& message)
{
    this->getPlugin()
        ->getRageMP()
        ->GetPlayerPool()
        .Broadcast(
            message
        );
}

std::vector<Player*> L5RP::PlayerManager::getPlayersInRange(const L5RP::Vector3& pos, float range) {
    std::vector<Player*> result;

    for(auto pair : this->getPlayers()) {
        auto player = pair.second;

        if(pos.calcDistance(player->getPosition()) < range)
            result.push_back(player);
    }

    return result;
}

L5RP::Player* L5RP::PlayerManager::getPlayer(rage::entityId_t id) const
{
    return (Player*)
        this->getEntity(id);
}

L5RP::Player* L5RP::PlayerManager::getPlayer(std::string const& name) const
{
    Player* result = nullptr;

    try {

        result = this->getPlayer(
            static_cast<rage::entityId_t>(std::stoul(name))
        );

    }

    catch(...) {

        for(
            std::pair<rage::entityId_t, Player*> pair : this->getPlayers()
            ) {

            if(STRING_CONTAINS_IGNORE_CASE(pair.second->getName(), name)) {
                result = pair.second;

                break;
            }
        }

    }

    return result;
}

std::map<uint16_t, Player*> const& L5RP::PlayerManager::getPlayers() const
{

    return (std::map<uint16_t, Player*> const&) this->getEntities();

}

void L5RP::Player::OnReady()
{
    this->getManager()->getPlugin()->getConsole()->info(
        this->getName() + " loaded his client up!"
    );

    this->initSyncedData();

    for(auto const& pair : this->getComponents())
        ((PlayerComponent*) pair.second)->OnInitialized();
}

void L5RP::Player::OnDisconnect(rage::exit_t exitType, const char* reason)
{
    this->isConnected = false;

    this->getManager()->getPlugin()->getConsole()->info(
        this->getName() + " left server!"
    );


    switch(exitType) {

        case rage::exit_t::timeout:

            for(auto const& pair : this->getComponents())
                ((PlayerComponent*) pair.second)->OnTimeout();

            break;
        case rage::exit_t::kicked:

            for(auto const& pair : this->getComponents())
                ((PlayerComponent*) pair.second)->OnKicked(std::string(reason));

            break;
        default:

            break;
    }

    for(auto const& pair : this->getComponents())
        ((PlayerComponent*) pair.second)->OnDisconnect();

}

void L5RP::Player::OnDeath(rage::hash_t reason, Player* killer)
{
    this->getManager()->getPlugin()->getConsole()->info(
        this->getName() + " died!"
    );


    for(auto const& pair : this->getComponents())
        ((PlayerComponent*) pair.second)->OnDeath(reason, killer);

}

void L5RP::Player::OnSpawn()
{
    this->getManager()->getPlugin()->getConsole()->info(
        this->getName() + " spawned!"
    );

    for(auto const& pair : this->getComponents())
        ((PlayerComponent*) pair.second)->OnSpawn();

}

void L5RP::Player::OnMessage(const std::string& text)
{
    this->getManager()->getPlugin()->getConsole()->info(
        this->getName() + " sent a message: " + text
    );

    for(auto const& pair : this->getComponents())
        ((PlayerComponent*) pair.second)->OnMessage(text);

}

void L5RP::Player::sendLocalMessage(std::string const& message, float range)
{
    this->getManager()
        ->getPlugin()
        ->getRageMP()
        ->GetPlayerPool()
        .BroadcastInRange(
            message,
            this->getRageMPHandle()->GetPosition(),
            range,
            this->getDimension()
        );
}

void L5RP::Player::sendGlobalMessage(std::string const& message)
{
    this->getManager()->sendGlobalMessage(message);
}

void L5RP::Player::OnStartEnterVehicle(Vehicle* vehicle, uint8_t seat)
{

    for(auto const& pair : this->getComponents())
        ((PlayerComponent*) pair.second)->OnStartEnterVehicle(vehicle, seat);

    for(auto const& pair : vehicle->getComponents())
        ((VehicleComponent*) pair.second)->OnStartEnter(this, seat);

}

void L5RP::Player::OnEnterVehicle(Vehicle* vehicle, uint8_t seat)
{

    for(auto const& pair : this->getComponents())
        ((PlayerComponent*) pair.second)->OnEnterVehicle(vehicle, seat);

    for(auto const& pair : vehicle->getComponents())
        ((VehicleComponent*) pair.second)->OnEnter(this, seat);

}

void L5RP::Player::OnStartExitVehicle(Vehicle* vehicle)
{

    for(auto const& pair : this->getComponents())
        ((PlayerComponent*) pair.second)->OnStartExitVehicle(vehicle);

    for(auto const& pair : vehicle->getComponents())
        ((VehicleComponent*) pair.second)->OnStartExit(this);

}

void L5RP::Player::OnExitVehicle(Vehicle* vehicle)
{

    for(auto const& pair : this->getComponents())
        ((PlayerComponent*) pair.second)->OnExitVehicle(vehicle);

    for(auto const& pair : vehicle->getComponents())
        ((VehicleComponent*) pair.second)->OnExit(this);

}

void L5RP::Player::OnRemoteEvent(uint64_t eventName, std::vector<std::string> const& args)
{
    //Console::getInstance()->info(
    //    this->getName() + " called client event: " + std::to_string(eventName)
    //);

    bool executed = false;

    for(auto const& pair : this->getComponents()) {

        try {

            ((PlayerComponent*) pair.second)->events.at(eventName)->onExecute(args);

            executed = true;

        }

        catch(std::out_of_range const&) {}

    }

    if(!executed)
        this->getManager()->getPlugin()->getConsole()->warn(
            this->getName()
            + " tried to execute event that doesn't exist "
            + std::to_string(eventName)
        );

}

void L5RP::Player::OnCommand(std::string const& cmd, std::string const& args)
{

    this->getManager()->getPlugin()->getConsole()->info(
        this->getName()
        + " executed command: /"
        + cmd + " " + args
    );

    bool executed = false;

    for(auto const& pair : this->getComponents()) {

        try {

            auto command = ((PlayerComponent*) pair.second)->commands.at(cmd);

            if(!command->canExecute())
                executed = false;

            else {
                command->onExecute(args);

                executed = true;
            }
        }

        catch(ChatException const& e) {
            for(
                std::string const& msg : e.getMessages())
                this->sendMessage(msg);

            executed = true;
        }

        catch(Exception const& e) {
            this->sendMessage(e.getMessage());

            executed = true;
        }
        catch(std::out_of_range const&) {}

    }

    if(!executed)
        this->sendMessage(u8"{255, 0, 0}Tokios komandos nėra!");

}

void Player::OnEnterCheckPoint(CheckPoint* checkPoint)
{

    for(auto const& pair : this->getComponents())
        ((PlayerComponent*) pair.second)->OnEnterCheckPoint(checkPoint);


}

void Player::OnExitCheckPoint(CheckPoint* checkPoint)
{

    for(auto const& pair : this->getComponents())
        ((PlayerComponent*) pair.second)->OnExitCheckPoint(checkPoint);


}

void L5RP::Player::initSyncedData()
{
    //Vehicle Data
    for(auto pair : this->getManager()->getPlugin()->getVehicleManager()->getVehicles()) {
        this->callClientEvent(
            "updateBlinker",
            pair.first,
            0,
            pair.second->isBlinkerActive(0)
        );

        this->callClientEvent(
            "updateBlinker",
            pair.first,
            1,
            pair.second->isBlinkerActive(1)
        );
    }

    //Blip Data
    nlohmann::json blipData;
    blipData["blips"] = nlohmann::json::array();

    for(auto pair : this->getManager()->getPlugin()->getBlipManager()->getBlips()) {
        nlohmann::json bData;
        bData["id"] = pair.first;
        bData["visible"] = pair.second->isVisibleOnMap();

        blipData["blips"].push_back(bData);
    }

    this->callClientEvent(
        "initBlipData",
        blipData.dump()
    );

    //Component Variation.
    for(auto pair : this->getManager()->getPlayers()) {
        auto player = pair.second;

        if(player != this) {
            this->callClientEvent(
                "updateCustomSyncedPlayerData",
                (int) player->getID(),
                player->getCustomDataReference().dump()
            );

            for(uint8_t id = 0; id < player->componentVariations.size(); id++) {
                auto const& data = player->componentVariations.at(id);

                this->callClientEvent(
                    "setComponentVariation",
                    (int) player->getID(),
                    (int) id,
                    (int) data.drawable,
                    (int) data.texture,
                    (int) data.pallete
                );
            }

            for(uint8_t id = 0; id < player->headOverlays.size(); id++) {
                auto const& data = player->headOverlays.at(id);

                this->callClientEvent(
                    "setHeadOverlay",
                    (int) player->getID(),
                    (int) id,
                    (int) data.value,
                    (float) data.opacity,
                    (int) data.color,
                    (int) data.secondaryColor
                );
            }
        }
    }

    this->getManager()->getPlugin()->getRageMP()->GetPlayerPool().Call(
        "updateCustomSyncedPlayerData",
        (int) this->getID(),
        this->customData.dump()
    );

    for(uint8_t id = 0; id < this->componentVariations.size(); id++) {
        auto const& data = this->componentVariations.at(id);

        this->getManager()->getPlugin()->getRageMP()->GetPlayerPool().Call(
            "setComponentVariation",
            (int) this->getID(),
            (int) id,
            (int) data.drawable,
            (int) data.texture,
            (int) data.pallete
        );
    }

    for(uint8_t id = 0; id < this->headOverlays.size(); id++) {
        auto const& data = this->headOverlays.at(id);

        this->getManager()->getPlugin()->getRageMP()->GetPlayerPool().Call(
            "setHeadOverlay",
            (int) this->getID(),
            (int) id,
            (int) data.value,
            (float) data.opacity,
            (int) data.color,
            (int) data.secondaryColor
        );
    }

}

L5RP::Player::Player(PlayerManager* manager, rage::IPlayer* player)
    : SmartEntity(player, manager)
{

    this->getManager()->getPlugin()->getConsole()->info(
        player->GetName() + " (" + player->GetIp() + ") connected to the server!"
    );

}

std::string const& Player::getHWID() const
{
    return this->getRageMPHandle()->GetSerial();
}

std::string const& Player::getSocialClubName() const
{
    return this->getRageMPHandle()->GetSocialClubName();
}

void L5RP::Player::setGender(Gender gender)
{
    this->getRageMPHandle()->SetModel(
        HASH_TO_GAME(
            gender == Gender::MALE
            ? "mp_m_freemode_01"
            : "mp_f_freemode_01"
        )
    );
}

L5RP::Player::Gender L5RP::Player::getGender() const
{
    uint32_t skinHash = this->getRageMPHandle()->GetModel();

    if(skinHash == HASH_TO_GAME("mp_m_freemode_01"))
        return Gender::MALE;
    else if(skinHash == HASH_TO_GAME("mp_f_freemode_01"))
        return Gender::FEMALE;
    else {

        this->getManager()->getPlugin()->getConsole()->warn(
            "Server requested for " + this->getName() +
            "'s gender, but user uses different model! Returning Female"
        );

        return Gender::FEMALE;

    }

}

void L5RP::Player::setFaceShape(uint8_t faceShape)
{
    this->setHeadBlend(
        faceShape,
        this->getSkinColour()
    );
}

uint8_t L5RP::Player::getFaceShape() const
{
    return this->getRageMPHandle()->GetHeadBlend().m_shape[0];
}

void L5RP::Player::setSkinColour(float skinColour)
{
    this->skinColour = skinColour;

    this->setHeadBlend(
        this->getFaceShape(),
        skinColour
    );
}

float L5RP::Player::getSkinColour() const
{
    return this->skinColour;
}

void L5RP::Player::setEyeColour(uint8_t id)
{
    this->getRageMPHandle()->SetEyeColour(id);
}

uint8_t L5RP::Player::getEyeColour() const
{
    return this->getRageMPHandle()->GetEyeColour();
}

void L5RP::Player::setHaircut(uint8_t id)
{
    this->setComponentVariation(
        2,
        id,
        0,
        0
    );
}

uint8_t L5RP::Player::getHaircut() const
{
    return this->getComponentDrawable(2);
}

void L5RP::Player::setHairColour(uint8_t color)
{
    this->getRageMPHandle()->SetHairColour(
        color,
        this->getHairHighlightColour()
    );
}

uint8_t L5RP::Player::getHairColour() const
{
    return this->getRageMPHandle()->GetHairColour();
}

void L5RP::Player::setHairHighlightColour(uint8_t color)
{
    this->getRageMPHandle()->SetHairColour(
        this->getHairColour(),
        color
    );
}

uint8_t L5RP::Player::getHairHighlightColour() const
{
    return this->getRageMPHandle()->GetHairHighlightColour();
}

void L5RP::Player::setEyeBrows(uint8_t id)
{
    this->setHeadOverlay(
        2,
        id,
        this->getEyeBrowsColour()
    );
}

uint8_t L5RP::Player::getEyeBrows() const
{
    return this->getHeadOverlay(2);
}

void L5RP::Player::setEyeBrowsColour(uint8_t color)
{
    this->setHeadOverlay(
        2,
        this->getEyeBrows(),
        color
    );
}

uint8_t L5RP::Player::getEyeBrowsColour() const
{
    return this->getHeadOverlayColor(2);
}

void L5RP::Player::setBeard(uint8_t id)
{
    this->setHeadOverlay(
        1,
        id,
        this->getBeardColour()
    );
}

uint8_t L5RP::Player::getBeard() const
{
    return this->getHeadOverlay(1);
}

void L5RP::Player::setBeardColour(uint8_t color)
{
    this->setHeadOverlay(
        1,
        this->getBeard(),
        color
    );
}

uint8_t L5RP::Player::getBeardColour() const
{
    return this->getHeadOverlayColor(1);
}

void L5RP::Player::setFaceBlemishes(uint8_t id)
{
    this->setHeadOverlay(
        0,
        id,
        0
    );
}

uint8_t L5RP::Player::getFaceBlemishes() const
{
    return this->getHeadOverlay(0);
}

void L5RP::Player::setFaceAge(uint8_t id)
{
    this->setHeadOverlay(
        3,
        id,
        0
    );
}

uint8_t L5RP::Player::getFaceAge() const
{
    return this->getHeadOverlay(3);
}

void L5RP::Player::setFaceComplexion(uint8_t id)
{
    this->setHeadOverlay(
        6,
        id,
        0
    );
}

uint8_t L5RP::Player::getFaceComplexion() const
{
    return this->getHeadOverlay(6);
}

void L5RP::Player::setFaceFreckles(uint8_t id)
{
    this->setHeadOverlay(
        9,
        id,
        0
    );
}

uint8_t L5RP::Player::getFaceFreckles() const
{
    return this->getHeadOverlay(9);
}

void L5RP::Player::setFaceSunDamage(uint8_t id)
{
    this->setHeadOverlay(
        7,
        id,
        0
    );
}

uint8_t L5RP::Player::getFaceSunDamage() const
{
    return this->getHeadOverlay(7);
}

void L5RP::Player::setChestHair(uint8_t id)
{
    this->setHeadOverlay(
        10,
        id,
        this->getChestHairColour()
    );
}

uint8_t L5RP::Player::getChestHair() const
{
    return this->getHeadOverlay(10);
}

void L5RP::Player::setChestHairColour(uint8_t color)
{
    this->setHeadOverlay(
        10,
        this->getChestHair(),
        color
    );
}

uint8_t L5RP::Player::getChestHairColour() const
{
    return this->getHeadOverlayColor(10);
}

void L5RP::Player::setBodyBlemishes(uint8_t id)
{
    this->setHeadOverlay(
        11,
        id,
        0
    );
}

uint8_t L5RP::Player::getBodyBlemishes() const
{
    return this->getHeadOverlay(11);
}

void L5RP::Player::setDenseBodyBlemishes(uint8_t id)
{
    this->setHeadOverlay(
        12,
        id,
        0
    );
}

uint8_t L5RP::Player::getDenseBodyBlemishes() const
{
    return this->getHeadOverlay(12);
}

void L5RP::Player::setFaceFeature(uint8_t id, float value)
{
    this->getRageMPHandle()->SetFaceFeature(id, value);
}

float L5RP::Player::getFaceFeature(uint8_t id) const
{
    return this->getRageMPHandle()->GetFaceFeature(id);
}

void Player::kick(std::string const& reason)
{
    this->getRageMPHandle()->Kick(reason.c_str());
}

void Player::ban(std::string const& reason)
{
    this->getRageMPHandle()->Ban(reason.c_str());
}

void Player::sendMessage(std::string const& message)
{
    this->getRageMPHandle()->OutputChatBox(message);
}

void Player::sendNotification(std::string const& message)
{
    this->getRageMPHandle()->Notify(message);
}

void Player::requestIPL(std::string const& ipl)
{
    this->callClientEvent(
        "IPL_request",
        ipl
    );
}

void Player::removeIPL(std::string const& ipl)
{
    this->callClientEvent(
        "IPL_remove",
        ipl
    );
}

std::string const& Player::getName() const
{
    return this->getRageMPHandle()->GetName();
}

void Player::setName(std::string const& name)
{
    this->getRageMPHandle()->SetName(name);
}

Transform Player::getTransform()
{
    auto pos = this->getPosition();
    auto heading = this->getHeading();

    return Transform(
        pos.getX(),
        pos.getY(),
        pos.getZ(),
        heading
    );
}

void Player::setTransform(const L5RP::Transform& transform)
{
    this->setTransform(
        transform.getX(),
        transform.getY(),
        transform.getZ(),
        transform.getH()
    );
}

void Player::setTransform(float x, float y, float z, float h)
{
    this->setPosition(
        x,
        y,
        z
    );

    this->setHeading(
        h
    );
}

float Player::getHeading() const
{
    return this->getRageMPHandle()->GetHeading();
}

void Player::setHeading(float heading)
{
    this->getRageMPHandle()->SetHeading(heading);
}

float Player::getHealth() const
{
    return this->getRageMPHandle()->GetHealth();
}

void Player::setHealth(float health)
{
    this->getRageMPHandle()->SetHealth(health);
}

bool Player::isDead() const
{
    return this->getHealth() == 0.f;
}

float Player::getArmour() const
{
    return this->getRageMPHandle()->GetArmour();
}

void Player::setArmour(float armour) const
{
    this->getRageMPHandle()->SetArmour(armour);
}

void Player::revive(Vector3 const& pos, float h)
{
    this->getRageMPHandle()->Spawn(
        {
            pos.getX(),
            pos.getY(),
            pos.getZ()
        },
        h
    );
}

void Player::revive(float x, float y, float z, float h)
{
    this->getRageMPHandle()->Spawn(
        {
            x,
            y,
            z
        },
        h
    );
}

bool Player::isInVehicle() const
{
    return this->getRageMPHandle()->GetVehicle() != nullptr;
}

Vehicle* Player::getVehicle() const
{
    auto _vehicle = this->getRageMPHandle()->GetVehicle();

    if(_vehicle == nullptr)
        return nullptr;

    return this->getManager()
               ->getPlugin()
               ->getVehicleManager()
               ->getVehicle(_vehicle->GetId());
}

void Player::setVehicle(Vehicle* vehicle, int8_t seat)
{
    if(vehicle == nullptr)
        this->removeFromVehicle();

    else
        this->getRageMPHandle()->PutIntoVehicle(
            vehicle->getRageMPHandle(),
            seat
        );
}

void Player::removeFromVehicle()
{
    if(this->isConnected)
        this->getRageMPHandle()->RemoveFromVehicle();
}

void Player::connectVoiceTo(Player* player)
{
    this->getRageMPHandle()->EnableVoiceTo(
        player->getRageMPHandle()
    );
}

void Player::disconnectVoiceFrom(Player* player)
{
    this->getRageMPHandle()->DisableVoiceTo(
        player->getRageMPHandle()
    );
}

std::vector<Player*> Player::getConnectedPlayersByVoice() const
{
    std::vector<Player*> result;

    for(auto player : this->getRageMPHandle()->GetVoiceListeners())
        result.push_back(
            this->getManager()->getPlayer(
                player->GetId()
            )
        );

    return result;
}

int8_t Player::getSeat() const
{
    return this->getRageMPHandle()->GetSeat();
}

bool Player::isInDriverSeat() const
{
    return this->isInVehicle() && this->getSeat() == -1;
}

std::string Player::getIp() const
{
    return this->getRageMPHandle()->GetIp();
}

int Player::getPing() const
{
    return this->getRageMPHandle()->GetPing();
}

float Player::getPacketLoss() const
{
    return this->getRageMPHandle()->GetPacketLoss();
}

uint32_t Player::getActiveWeapon() const
{
    return this->getRageMPHandle()->GetCurrentWeapon();
}

void Player::setActiveWeapon(uint32_t weapon)
{
    this->getRageMPHandle()->SetCurrentWeapon(weapon);
}

uint16_t Player::getActiveWeaponAmmo() const
{
    return this->getRageMPHandle()->GetCurrentWeaponAmmo();
}

void Player::setActiveWeaponAmmo(uint16_t ammo)
{
    this->getRageMPHandle()->SetCurrentWeaponAmmo(ammo);
}

uint16_t Player::getWeaponAmmo(uint32_t weapon) const
{
    return this->getRageMPHandle()->GetWeaponAmmo(weapon);
}

void Player::setWeaponAmmo(uint32_t weapon, uint16_t ammo)
{
    this->getRageMPHandle()->SetWeaponAmmo(weapon, ammo);
}

std::map<uint32_t, uint16_t> Player::getWeapons() const
{
    return this->getRageMPHandle()->GetWeapons();
}

void Player::giveWeapon(uint32_t weapon, uint16_t ammo)
{
    this->getRageMPHandle()->GiveWeapon(weapon, ammo);
}

void Player::removeWeapon(uint32_t weapon)
{
    this->getRageMPHandle()->RemoveWeapon(weapon);
}

void Player::removeWeapons() const
{
    this->getRageMPHandle()->RemoveAllWeapons();
}

void L5RP::Player::callServerEvent(std::string const& eventName, std::string const& args)
{

    uint64_t event = XXHash64::hash(
        eventName.c_str(),
        eventName.length(),
        0
    );

    for(auto const& pair : this->getComponents()) {
        try {
            ((PlayerComponent*) pair.second)->getEvents().at(event)->onExecute(args);
        }

        catch(std::out_of_range const&) {}
    }

}

void L5RP::Player::callServerEvent(std::string const& eventName, std::vector<std::string> const& args)
{

    uint64_t event = XXHash64::hash(
        eventName.c_str(),
        eventName.length(),
        0
    );

    for(auto const& pair : this->getComponents()) {
        try {
            ((PlayerComponent*) pair.second)->getEvents().at(event)->onExecute(args);
        }

        catch(std::out_of_range const&) {}
    }

}

std::vector<Player*> Player::findClosestPlayers(float maxDist)
{
    std::vector<Player*> result;

    for(auto pair : this->getManager()->getPlayers()) {
        auto player = pair.second;
        auto currDist = player->getPosition().calcDistance(this->getPosition());

        if(player != this && currDist < maxDist)
            result.push_back(player);
    }

    return result;
}

Player* Player::findClosestPlayer(float maxDist)
{
    auto currMaxDist = maxDist;
    Player* currPlayer = nullptr;

    for(auto pair : this->getManager()->getPlayers()) {
        auto player = pair.second;
        auto currDist = player->getPosition().calcDistance(this->getPosition());

        if(player != this && currDist < currMaxDist) {
            currMaxDist = currDist;
            currPlayer = player;
        }
    }

    return currPlayer;
}

void L5RP::Player::callServerEvent(std::string const& eventName, std::vector<ArgumentValue> const& args)
{

    uint64_t event = XXHash64::hash(
        eventName.c_str(),
        eventName.length(),
        0
    );

    for(auto const& pair : this->getComponents()) {
        try {
            ((PlayerComponent*) pair.second)->getEvents().at(event)->onExecute(args);
        }

        catch(std::out_of_range const&) {}
    }

}

rage::IPlayer* L5RP::Player::getRageMPHandle() const
{
    return (rage::IPlayer*) Entity::getRageMPHandle();
}

L5RP::PlayerManager* L5RP::Player::getManager() const
{
    return (PlayerManager*) Entity::getManager();
}

void L5RP::Player::setHeadOverlay(uint8_t id, HeadOverlay const& overlay)
{
    this->headOverlays[id] = overlay;

    this->getManager()->getPlugin()->getRageMP()->GetPlayerPool().Call(
        "setHeadOverlay",
        (int) this->getID(),
        (int) id,
        (int) overlay.value,
        (float) overlay.opacity,
        (int) overlay.color,
        (int) overlay.secondaryColor
    );
}

void L5RP::Player::setHeadOverlay(uint8_t id, uint8_t value, uint8_t color, float opacity)
{
    this->setHeadOverlay(
        id,
        HeadOverlay(value, opacity, color, color)
    );
}

void L5RP::Player::setHeadOverlay(uint8_t id, uint8_t value, uint8_t color, uint8_t secondaryColour, float opacity)
{
    this->setHeadOverlay(
        id,
        HeadOverlay(value, opacity, color, secondaryColour)
    );
}

uint8_t L5RP::Player::getHeadOverlay(uint8_t id) const
{
    return this->headOverlays.at(id).value;
}

float L5RP::Player::getHeadOverlayOpacity(uint8_t id) const
{
    return this->headOverlays.at(id).opacity;
}

uint8_t L5RP::Player::getHeadOverlayColor(uint8_t id) const
{
    return this->headOverlays.at(id).color;
}

uint8_t L5RP::Player::getHeadOverlaySecondaryColor(uint8_t id) const
{
    return this->headOverlays.at(id).secondaryColor;
}

void L5RP::Player::setComponentVariation(uint8_t id, ComponentVariation const& data)
{
    this->componentVariations[id] = data;

    this->getManager()->getPlugin()->getRageMP()->GetPlayerPool().Call(
        "setComponentVariation",
        (int) this->getID(),
        (int) id,
        (int) data.drawable,
        (int) data.texture,
        (int) data.pallete
    );
}

void Player::setPropertyVariation(uint8_t id, uint8_t drawable, uint8_t texture)
{
    this->getRageMPHandle()->SetProp(
        id,
        {
            drawable,
            texture
        }
    );
}

uint8_t Player::getPropertyDrawable(uint8_t id) const
{
    return this->getRageMPHandle()->GetProp(id).drawableId;
}

uint8_t Player::getPropertyTexture(uint8_t id) const
{
    return this->getRageMPHandle()->GetProp(id).textureId;
}

void L5RP::Player::setComponentVariation(uint8_t id, uint16_t drawable, uint8_t texture, uint8_t palette)
{
    this->setComponentVariation(
        id,
        ComponentVariation(drawable, texture, palette)
    );
}

uint16_t L5RP::Player::getComponentDrawable(uint8_t id) const
{
    return this->getComponentVariation(id).drawable;
}

uint8_t L5RP::Player::getComponentTexture(uint8_t id) const
{
    return this->getComponentVariation(id).texture;
}

uint8_t L5RP::Player::getComponentPalette(uint8_t id) const
{
    return this->getComponentVariation(id).pallete;
}

ComponentVariation const& L5RP::Player::getComponentVariation(uint8_t id) const
{
    try {
        return this->componentVariations.at(id);
    } catch(std::out_of_range const&) {
        throw new Exception("ComponentVariation ids are from 0 to 11. Id of " + std::to_string(id) + "was requested!");
    }
}

std::array<ComponentVariation, 12> const& L5RP::Player::getComponentVariations() const
{
    return this->componentVariations;
}

nlohmann::json& L5RP::Player::getCustomDataReference()
{
    return this->customData;
}

void L5RP::Player::updateCustomData() const
{
    this->getManager()->getPlugin()->getRageMP()->GetPlayerPool().Call(
        "updateCustomSyncedPlayerData",
        (int) this->getID(),
        this->customData.dump()
    );
}

void L5RP::Player::setHeadBlend(uint8_t faceShape, float skinColour)
{
    this->getRageMPHandle()
        ->SetHeadBlend(
            faceShape,
            faceShape,
            0,
            2,
            45,
            0,
            0.5,
            skinColour,
            0
        );
}

void L5RP::PlayerComponent::registerEvent(PlayerEvent* event)
{

    uint64_t hash = XXHash64::hash(
        event->getName().c_str(),
        event->getName().length(),
        0
    );

    this->events[hash] = event;

}

void L5RP::PlayerComponent::registerCommand(PlayerCommand* command, std::string const& name)
{

    this->commands[name] = command;

}

L5RP::PlayerComponent::PlayerComponent(Player* player, Script* script)
    : EntityComponent(player, script)
{

}

L5RP::Player* L5RP::PlayerComponent::getPlayer() const
{
    return (L5RP::Player*) this->getEntity();
}

std::map<uint64_t, L5RP::PlayerEvent*> const& L5RP::PlayerComponent::getEvents() const
{
    return this->events;
}

std::map<std::string, L5RP::PlayerCommand*> const& L5RP::PlayerComponent::getCommands() const
{
    return this->commands;
}

L5RP::PlayerEvent::PlayerEvent(
    PlayerComponent* component,
    std::string const& eventName,
    std::function<
        void(
            std::vector<ArgumentValue> const
            &
        )> const& function,
    std::vector<IArgument*> const& mandatoryArgs,
    std::vector<IArgument*> const
    & optionalArgs
)
    :
    Callable<void>(
        mandatoryArgs,
        optionalArgs
    )
{

    this->
            component = component;
    this->
            function = function;
    this->
            eventName = eventName;

    component->registerEvent(this);

}

void L5RP::PlayerEvent::onExecute(std::string const& args)
{
    try {
        Callable<void>::onExecute(args);
    } catch(ArgumentParseException const& e) {
        this->getComponent()->getPlayer()->sendMessage(
            e.getMessage()
        );
    }
}

void L5RP::PlayerEvent::onExecute(std::vector<std::string> const& args)
{
    try {
        Callable<void>::onExecute(args);
    } catch(ArgumentParseException const& e) {
        this->getComponent()->getPlayer()->sendMessage(
            e.getMessage()
        );
    }
}

void L5RP::PlayerEvent::onExecute(std::vector<ArgumentValue> const& args)
{
    this->function(args);
}

L5RP::PlayerComponent* L5RP::PlayerEvent::getComponent() const
{
    return this->component;
}

std::string const& L5RP::PlayerEvent::getName() const
{
    return this->eventName;
}

#define CHAT_COLOR_BLUE u8"@B"

void L5RP::PlayerCommand::throwArgsCountException()
{

    std::vector<std::string> msg;
    std::string msg1 = CHAT_COLOR_BLUE u8"*Naudojimas: /" + this->names.at(0) + u8" ";

    for(
        PlayerCommand::Argument* arg : this->mandatoryArgs
        )
        msg1 += u8"<" + arg->getName() + u8">";

    for(
        PlayerCommand::Argument* arg : this->optionalArgs
        )
        msg1 += u8"[" + arg->getName() + u8"]";

    msg.push_back(msg1);

    for(
        PlayerCommand::Argument* arg : this->mandatoryArgs
        )
        msg.push_back(CHAT_COLOR_BLUE + arg->getName() + u8" - " + arg->getDescription());

    for(
        PlayerCommand::Argument* arg : this->optionalArgs
        )
        msg.push_back(CHAT_COLOR_BLUE + arg->getName() + u8" - " + arg->getDescription());

    if(this->names.size() != 1) {

        std::string msg2 = CHAT_COLOR_BLUE u8"*Ši komanda turi dar vardų: ";

        for(
            size_t i = 1; i < this->names.size(); i++
            )
            msg2 += u8"/" + this->names[i] + u8" ";

        msg.push_back(msg2);

    }

    throw CallableArgumentCountException(msg);

}

L5RP::PlayerCommand::Argument::Argument(IArgument* argument, std::string const& name, std::string const& desc)
{

    this->argument = argument;
    this->name = name;
    this->desc = desc;

}

std::string const& L5RP::PlayerCommand::Argument::getName() const
{
    return this->name;
}

std::string const& L5RP::PlayerCommand::Argument::getDescription() const
{
    return this->desc;
}

L5RP::IArgument* L5RP::PlayerCommand::Argument::getArgument() const
{
    return this->argument;
}

PlayerComponent* L5RP::PlayerCommand::getComponent() const
{
    return this->component;
}

L5RP::PlayerCommand::PlayerCommand(
    PlayerComponent* component,
    std::function<
        void(
            std::vector<ArgumentValue> const
            &
        )> const& function,
    std::initializer_list<std::string> const& names,
    std::initializer_list<Argument*> const
    & mandatoryArgs,
    std::initializer_list<Argument*> const& optionalArgs,
    std::string const
    & desc
)
    :
    Callable<void>(
        ARGUMENT_CONVERTER(mandatoryArgs),
        ARGUMENT_CONVERTER(optionalArgs)
    )
{

    this->component = component;
    this->function = function;
    this->desc = desc;
    this->names = std::vector<std::string>(names);
    this->mandatoryArgs = std::vector<Argument*>(mandatoryArgs);
    this->optionalArgs = std::vector<Argument*>(optionalArgs);

    for(std::string const& name : names)
        this->getComponent()
            ->registerCommand(this, name);

}

L5RP::PlayerCommand::~PlayerCommand()
{
    for(Argument* arg : this->mandatoryArgs)
        delete arg;

    for(Argument* arg : this->optionalArgs)
        delete arg;
}

void L5RP::PlayerCommand::onExecute(std::string const& args)
{
    Callable<void>::onExecute(args);
}

void L5RP::PlayerCommand::onExecute(std::vector<std::string> const& args)
{
    Callable<void>::onExecute(args);
}

void L5RP::PlayerCommand::onExecute(std::vector<ArgumentValue> const& args)
{
    this->function(args);
}

std::string const& L5RP::PlayerCommand::getDescription() const
{
    return this->desc;
}

std::vector<L5RP::IArgument*> ARGUMENT_CONVERTER(std::initializer_list<L5RP::PlayerCommand::Argument*> args)
{

    std::vector<L5RP::IArgument*> result;

    for(
        L5RP::PlayerCommand::Argument* arg : args
        )
        result.push_back(arg->getArgument());

    return result;

}
