#include "api.h"

using namespace L5RP;

void Vehicle::updateNeon()
{
    if(this->_nenabled)
        this->getRageMPHandle()->SetNeonsColour(
            this->_ncolor.getR(),
            this->_ncolor.getG(),
            this->_ncolor.getB()
        );

    else this->getRageMPHandle()->EnableNeons(false);
}

L5RP::Vehicle::Vehicle(
    VehicleManager* manager,
    std::string const& model,
    float x,
    float y,
    float z,
    float heading,
    const std::string& numberPlate,
    uint8_t alpha,
    bool locked,
    bool engine,
    rage::dimensionId_t dimension
)
    : SmartEntity(
    manager->getPlugin()->getRageMP()->GetVehiclePool().New(
        HASH_TO_GAME(model),
        {
            x,
            y,
            z
        },
        heading,
        numberPlate,
        alpha,
        locked,
        engine,
        dimension
    ),
    manager
)
{
    this->model = model;

    this->spawnLocation = Transform(x, y, z, heading);
    this->spawnDimension = dimension;

    this->setPrimaryColour(0, 0, 0);
    this->setSecondaryColour(0, 0, 0);
    this->setNeonColour(0, 0, 0);
    this->setWindowTint(4);
}

L5RP::Vehicle::Vehicle(
    VehicleManager* manager,
    std::string const& model,
    Transform const& transform,
    const std::string& numberPlate,
    uint8_t alpha,
    bool locked,
    bool engine,
    rage::dimensionId_t dimension
)
    : Vehicle(
    manager,
    model,
    transform.getX(),
    transform.getY(),
    transform.getZ(),
    transform.getH(),
    numberPlate,
    alpha,
    locked,
    engine,
    dimension
) {}

L5RP::Vehicle::Vehicle(
    VehicleManager* manager,
    std::string const& model,
    Vector3 const& pos,
    float heading,
    std::string const& numberPlate,
    uint8_t alpha,
    bool locked,
    bool engine,
    rage::dimensionId_t dimension
)
    : Vehicle(
    manager,
    model,
    pos.getX(),
    pos.getY(),
    pos.getZ(),
    heading,
    numberPlate,
    alpha,
    locked,
    engine,
    dimension
) {}

Vehicle::~Vehicle()
{
    this->removePlayersFromVehicle();
}

float Vehicle::calcKMHSpeed() const
{
    return Vector3(this->getRageMPHandle()->GetVelocity()).calcXYDistance(Vector3(0, 0, 0)) * 3.6f;
}

bool L5RP::Vehicle::isEngineActive() const
{
    return this->getRageMPHandle()->IsEngineActive();
}

void Vehicle::setEngineActive(bool status)
{
    this->getRageMPHandle()->SetEngineActive(status);
}

float Vehicle::getEngineHealth() const
{
    return this->getRageMPHandle()->GetEngineHealth();
}

float Vehicle::getBodyHealth() const
{
    return this->getRageMPHandle()->GetBodyHealth();
}

void Vehicle::fix()
{
    this->getRageMPHandle()->Repair();
}

bool Vehicle::hasDriver() const
{
    return this->getDriver() != nullptr;
}

Player* Vehicle::getDriver() const
{
    return this->getPlayer(255);
}

Player* Vehicle::getPlayer(uint8_t seat) const
{
    auto player = this->getRageMPHandle()->GetOccupant(seat);

    if(player == nullptr)
        return nullptr;

    return this->getManager()
               ->getPlugin()
               ->getPlayerManager()
               ->getPlayer(player->GetId());
}

bool Vehicle::isSeatUsed(uint8_t seat) const
{
    return this->getPlayer(seat) != nullptr;
}

std::vector<Player*> Vehicle::getPlayers() const
{
    auto players = this->getRageMPHandle()->GetOccupants();

    std::vector<Player*> result;

    for(rage::IPlayer* player : players)
        result.push_back(
            this->getManager()
                ->getPlugin()
                ->getPlayerManager()
                ->getPlayer(player->GetId())
        );

    return result;
}

void Vehicle::removePlayersFromVehicle() const
{
    for(auto player : this->getPlayers())
        player->removeFromVehicle();
}

bool Vehicle::isLocked() const
{
    return this->getRageMPHandle()->IsLocked();
}

void Vehicle::lock(bool toggle)
{
    return this->getRageMPHandle()->Lock(toggle);
}

bool Vehicle::isDead() const
{
    return this->getRageMPHandle()->IsDead();
}

void Vehicle::kill()
{
    this->getRageMPHandle()->Explode();
}

void Vehicle::revive(Vector3 const& pos, float heading)
{
    this->revive(
        pos.getX(),
        pos.getY(),
        pos.getZ(),
        heading
    );
}

void Vehicle::revive(float x, float y, float z, float h)
{
    return this->getRageMPHandle()->Spawn(
        {
            x,
            y,
            z
        },
        h
    );
}

void Vehicle::respawn()
{

    if(this->isDead())
        this->revive(
            Vector3(this->spawnLocation.getX(), this->spawnLocation.getY(), this->spawnLocation.getZ()),
            this->spawnLocation.getH()
        );

    else {
        this->setPosition(
            Vector3(this->spawnLocation.getX(), this->spawnLocation.getY(), this->spawnLocation.getZ())
        );
        this->setHeading(
            this->spawnLocation.getH()
        );
    }

    this->setDimension(
        this->getSpawnDimension()
    );

}

void Vehicle::setSpawnLocation(Transform const& transform)
{
    this->spawnLocation = transform;
}

void Vehicle::setSpawnLocation(float x, float y, float z, float h)
{
    this->setSpawnLocation(
        Transform(
            x,
            y,
            z,
            h
        )
    );
}

void Vehicle::setSpawnDimension(rage::dimensionId_t dimension)
{
    this->spawnDimension = dimension;
}

Transform const& Vehicle::getSpawnLocation() const
{
    return this->spawnLocation;
}

rage::dimensionId_t Vehicle::getSpawnDimension() const
{
    return this->spawnDimension;
}

bool Vehicle::isAutoRespawnEnabled() const
{
    return this->autoRespawn;
}

void Vehicle::enableAutoRespawn(bool autoRespawn)
{
    this->autoRespawn = true;
}

void Vehicle::setMod(uint8_t index, uint8_t value)
{
    this->getRageMPHandle()->SetMod(index, value);
}

uint8_t Vehicle::getMod(uint8_t index) const
{
    return this->getRageMPHandle()->GetMod(index);
}

void Vehicle::setExtra(uint8_t id, bool enabled)
{
    this->getRageMPHandle()->SetExtra(id, enabled);
}

bool Vehicle::getExtra(uint8_t id) const
{
    return this->getRageMPHandle()->GetExtra(id);
}

uint8_t Vehicle::getWheelType() const
{
    return this->getRageMPHandle()->GetWheelType();
}

void Vehicle::setWheelType(uint8_t type)
{
    this->getRageMPHandle()->SetWheelType(type);
}

void Vehicle::setFrontWheel(uint8_t type, uint8_t mod)
{
    this->setWheelType(type);

    this->setMod(23, mod);
}

void Vehicle::setBackWheel(uint8_t type, uint8_t mod)
{
    this->setWheelType(type);

    this->setMod(23, mod);
}

void Vehicle::setLivery(uint8_t type)
{
    this->getRageMPHandle()->SetLivery(type);
}

uint8_t Vehicle::getLivery() const
{
    return this->getRageMPHandle()->GetLivery();
}

void Vehicle::setWindowTint(uint8_t type)
{
    this->getRageMPHandle()->SetWindowTint(type);
}

uint8_t Vehicle::getWindowTint() const
{
    return this->getRageMPHandle()->GetWindowTint();
}

bool Vehicle::isNeonEnabled() const
{
    return this->_nenabled;
}

void Vehicle::enableNeon(bool toggle)
{
    this->_nenabled = toggle;

    this->updateNeon();
}

void Vehicle::setNeonColour(Color const& col)
{
    this->_ncolor = col;

    this->updateNeon();
}

void Vehicle::setNeonColour(uint8_t r, uint8_t g, uint8_t b)
{
    this->_ncolor.setR(r);
    this->_ncolor.setG(g);
    this->_ncolor.setB(b);

    this->updateNeon();
}

Color Vehicle::getNeonColour() const
{
    return this->_ncolor;
}

Color Vehicle::getPrimaryColour() const
{
    return Color(this->getRageMPHandle()->GetColourRGB(0));
}

Color Vehicle::getSecondaryColour() const
{
    return Color(this->getRageMPHandle()->GetColourRGB(1));
}

void Vehicle::setPrimaryColour(Color const& color)
{
    this->setPrimaryColour(
        color.getR(),
        color.getG(),
        color.getB()
    );
}

void Vehicle::setPrimaryColour(uint8_t r, uint8_t g, uint8_t b)
{
    auto color = this->getSecondaryColour();

    this->getRageMPHandle()->SetColourRGB(
        {
            r,
            g,
            b
        },
        {
            color.getR(),
            color.getG(),
            color.getB()
        }
    );
}

void Vehicle::setSecondaryColour(Color const& color)
{
    this->setSecondaryColour(
        color.getR(),
        color.getG(),
        color.getB()
    );
}

void Vehicle::setSecondaryColour(uint8_t r, uint8_t g, uint8_t b)
{
    auto color = this->getPrimaryColour();

    this->getRageMPHandle()->SetColourRGB(
        {
            color.getR(),
            color.getG(),
            color.getB()
        },
        {
            r,
            g,
            b
        }
    );
}

uint8_t Vehicle::getWheelColour() const
{
    return this->getRageMPHandle()->GetWheelColor();
}

void Vehicle::setWheelColour(uint8_t color)
{
    this->getRageMPHandle()->SetWheelColor(
        color
    );
}

uint8_t Vehicle::getTrimColour() const
{
    return this->getRageMPHandle()->GetTrimColor();
}

void Vehicle::setTrimColour(uint8_t color)
{
    this->getRageMPHandle()->SetTrimColor(color);
}

uint8_t Vehicle::getDashboardColour() const
{
    return this->getRageMPHandle()->GetDashboardColor();
}

void Vehicle::setDashboardColour(uint8_t color)
{
    return this->getRageMPHandle()->SetDashboardColor(color);
}

std::string const& Vehicle::getNumberPlate() const
{
    return this->getRageMPHandle()->GetNumberPlate();
}

void Vehicle::setNumberPlate(std::string const& plate)
{
    this->getRageMPHandle()->SetNumberPlate(plate);
}

uint8_t Vehicle::getNumberPlateType() const
{
    return this->getRageMPHandle()->GetNumberPlateType();
}

void Vehicle::setNumberPlateType(uint8_t type)
{
    this->getRageMPHandle()->SetNumberPlateType(type);
}

void Vehicle::setHeading(float h)
{
    auto rot = this->getRotation();

    this->setRotation(
        rot.getX(),
        rot.getY(),
        h
    );
}

float Vehicle::getHeading()
{
    return this->getRageMPHandle()->GetHeading();
}

Transform Vehicle::getTransform()
{
    auto pos = this->getPosition();
    auto h = this->getHeading();

    return Transform(pos, h);
}

void Vehicle::setTransform(float x, float y, float z, float h)
{
    this->setPosition(x, y, z);
    this->setHeading(h);
}

void Vehicle::setTransform(Transform const& transform)
{
    this->setPosition(transform.getX(), transform.getY(), transform.getZ());
    this->setHeading(transform.getH());
}

std::string const& L5RP::Vehicle::getModelName() const
{
    return this->model;
}

L5RP::VehicleManager* L5RP::Vehicle::getManager() const
{
    return (VehicleManager*) Entity::getManager();
}

bool Vehicle::isTrailerAttached() const
{
    return this->trailer != nullptr;
}

Vehicle* Vehicle::getAttachedTrailer() const
{
    return this->trailer;
}

void Vehicle::setBlinkerActive(uint8_t id, bool toggle)
{
    if(id != 0 && id != 1)
        return;

    this->blinkers[id] = toggle;

    this->getManager()->getPlugin()->getRageMP()->GetPlayerPool().Call(
        "updateBlinker",
        this->getID(),
        id,
        toggle
    );
}

bool Vehicle::isBlinkerActive(uint8_t id) const
{
    try {
        return this->blinkers.at(id);
    } catch(std::out_of_range const&) {
        return false;
    }
}

bool Vehicle::isLeftTurnSignalActive() const {
    return this->isBlinkerActive(1) && !this->isBlinkerActive(0);
}

bool Vehicle::isRightTurnSignalActive() const {
    return this->isBlinkerActive(0) && !this->isBlinkerActive(1);
}

bool Vehicle::isEmergencySignalActive() const {
    return this->isBlinkerActive(0) && this->isBlinkerActive(1);
}

void Vehicle::setLeftTurnSignalActive() {
    this->setBlinkerActive(0, false);
    this->setBlinkerActive(1, true);
}

void Vehicle::setRightTurnSignalActive() {
    this->setBlinkerActive(0, true);
    this->setBlinkerActive(1, false);
}

void Vehicle::setEmergencySignalActive() {
    this->setBlinkerActive(0, true);
    this->setBlinkerActive(1, true);
}

void Vehicle::disableAllTurnSignals() {
    this->setBlinkerActive(0, false);
    this->setBlinkerActive(1, false);
}

void Vehicle::OnDeath(rage::hash_t hash, Player* killer)
{
    if(this->isAutoRespawnEnabled())
        this->respawn();

    for(auto pair : this->getComponents()) {
        auto component = (VehicleComponent*) pair.second;

        component->OnDeath(hash, killer);
    }

}

void Vehicle::OnSirenToggle(bool toggle)
{

    for(auto pair : this->getComponents()) {
        auto component = (VehicleComponent*) pair.second;

        component->OnSirenToggle(toggle);
    }

}

void Vehicle::OnHornToggle(bool toggle)
{

    for(auto pair : this->getComponents()) {
        auto component = (VehicleComponent*) pair.second;

        component->OnHornToggle(toggle);
    }

}

void Vehicle::OnTrailerAttached(Vehicle* trailer)
{

    this->trailer = trailer;

    if(trailer == nullptr)
        for(auto pair : this->getComponents()) {
            auto component = (VehicleComponent*) pair.second;

            component->OnTrailedDetached(this->trailer);
        }

    else
        for(auto pair : this->getComponents()) {
            auto component = (VehicleComponent*) pair.second;

            component->OnTrailerAttached(this->trailer);
        }
}

void Vehicle::OnDamage(float bodyHealthLoss, float engineHealthLoss)
{
    for(auto pair : this->getComponents()) {
        auto component = (VehicleComponent*) pair.second;

        component->OnDamage(bodyHealthLoss, engineHealthLoss);
    }
}

rage::IVehicle* L5RP::Vehicle::getRageMPHandle() const
{
    return (rage::IVehicle*) Entity::getRageMPHandle();
}

L5RP::VehicleComponent::VehicleComponent(Vehicle* vehicle, Script* script)
    : EntityComponent(vehicle, script)
{

}

L5RP::Vehicle* L5RP::VehicleComponent::getVehicle() const
{
    return (Vehicle*) this->getEntity();
}

rage::IVehicleHandler* VehicleManager::GetVehicleHandler()
{
    return this;
}

void VehicleManager::OnVehicleDeath(rage::IVehicle* vehicle, rage::hash_t hash, rage::IPlayer* killer)
{
    auto _vehicle = this->getVehicle(
        vehicle->GetId()
    );

    Player* _player = nullptr;
    if(killer != nullptr) {
        _player = this->getPlugin()->getPlayerManager()->getPlayer(
            killer->GetId()
        );

        if(_player == nullptr)
            Console::getInstance()->warn("OnVehicleDeath was triggered with an unknown player");
    }

    if(_vehicle == nullptr)
        Console::getInstance()->warn("OnVehicleDeath was triggered with an unknown vehicle");

    else
        _vehicle->OnDeath(hash, _player);
}

void VehicleManager::OnVehicleSirenToggle(rage::IVehicle* vehicle, bool toggle)
{
    auto _vehicle = this->getVehicle(
        vehicle->GetId()
    );

    if(_vehicle == nullptr)
        Console::getInstance()->warn("OnVehicleSirenToggle was triggered with an unknown vehicle!");

    else
        _vehicle->OnSirenToggle(toggle);
}

void VehicleManager::OnVehicleHornToggle(rage::IVehicle* vehicle, bool toggle)
{
    auto _vehicle = this->getVehicle(
        vehicle->GetId()
    );

    if(_vehicle == nullptr)
        Console::getInstance()->warn("OnVehicleHornToggle was triggered with an unknown vehicle!");

    else
        _vehicle->OnHornToggle(toggle);
}

void VehicleManager::OnTrailerAttached(rage::IVehicle* vehicle, rage::IVehicle* trailer)
{
    auto _vehicle = this->getVehicle(
        vehicle->GetId()
    );

    Vehicle* _trailer = nullptr;
    if(trailer != nullptr) {
        _trailer = this->getVehicle(
            trailer->GetId()
        );

        if(_trailer == nullptr)
            Console::getInstance()->warn("OnTrailerAttached was triggered with an unknown trailer!");
    }

    if(_vehicle == nullptr)
        Console::getInstance()->warn("OnTrailerAttached was triggered with an unknown vehicle!");

    else
        _vehicle->OnTrailerAttached(_trailer);
}

void VehicleManager::OnVehicleDamage(rage::IVehicle* vehicle, float bodyHealthLoss, float engineHealthLoss)
{
    auto _vehicle = this->getVehicle(
        vehicle->GetId()
    );

    if(_vehicle == nullptr)
        Console::getInstance()->warn("OnVehicleDamage was triggered with an unknown vehicle");

    else
        _vehicle->OnDamage(bodyHealthLoss, engineHealthLoss);
}

L5RP::VehicleManager::VehicleManager(Plugin* plugin)
    : SmartEntityManager(plugin)
{
    plugin->getRageMP()->AddEventHandler(this);
}

void L5RP::VehicleManager::registerDefaultComponent(
    std::function<VehicleComponent*(Vehicle*)> fn
)
{
    SmartEntityManager::registerDefaultComponent(
        [fn](
            Entity* entity
        )->EntityComponent*
        {
            return (EntityComponent*) fn((Vehicle*) entity);
        }
    );
}

L5RP::Vehicle* L5RP::VehicleManager::getVehicle(std::string const& id) const
{
    return (Vehicle*) this->getEntity((rage::entityId_t) std::stoul(id));
}

L5RP::Vehicle* L5RP::VehicleManager::getVehicle(rage::entityId_t id) const
{
    return (Vehicle*) this->getEntity(id);
}

std::map<uint16_t, L5RP::Vehicle*> const& L5RP::VehicleManager::getVehicles() const
{
    return (std::map<uint16_t, Vehicle*> const&) this->getEntities();
}