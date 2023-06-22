#include "api.h"

using namespace L5RP;

BlipManager::BlipManager(Plugin* plugin)
    : EntityManager(plugin)
{

}

L5RP::Blip* BlipManager::getBlip(rage::entityId_t id) const
{
    return (Blip*) this->getEntity(id);
}

std::map<uint16_t, L5RP::Blip*> const& BlipManager::getBlips() const
{
    return (std::map<uint16_t, Blip*> const&) this->getEntities();
}

rage::IBlip* Blip::getRageMPHandle() const
{
    return (rage::IBlip*) Entity::getRageMPHandle();
}

Blip::Blip(
    BlipManager* manager,
    std::string const& title,
    uint32_t sprite,
    uint8_t color,
    Vector3 const& position,
    float scale,
    bool shortRange,
    bool isVisibleOnMap,
    uint8_t alpha,
    float drawDistance,
    short rotation,
    rage::dimensionId_t dimension
)
    : Entity(
    manager->getPlugin()->getRageMP()->GetBlipPool().New(
        sprite,
        position,
        scale,
        color,
        title,
        alpha,
        drawDistance,
        shortRange,
        rotation,
        dimension
    ),
    manager
)
{
    this->visibleOnMap = isVisibleOnMap;

    this->clientUpdate();
}

void Blip::clientUpdate() {
    this->getManager()->getPlugin()->getRageMP()->GetPlayerPool().Call(
        "updateBlipData",
        this->getID(),
        this->isVisibleOnMap()
    );
}

bool Blip::isVisibleOnMap() const {
    return this->visibleOnMap;
}

void Blip::setIsVisibleOnMap(bool visibleOnMap) {
    this->visibleOnMap = visibleOnMap;

    this->clientUpdate();
}

float Blip::getDrawDistance() {
    return this->getRageMPHandle()->GetDrawDistance();
}

void Blip::setDrawDistance(float distance) {
    this->getRageMPHandle()->SetDrawDistance(distance);
}

short Blip::getBlipRotation() {
    return this->getRageMPHandle()->GetBlipRotation();
}

void Blip::setRotation(short rotation) {
    this->getRageMPHandle()->SetRotation(rotation);
}

bool Blip::isShortRanged() {
    return this->getRageMPHandle()->IsShortRange();
}

void Blip::setShortRanged(bool toggle) {
    this->getRageMPHandle()->SetShortRange(toggle);
}

void Blip::showRouteFor(Player* player) {
    this->showRouteFor(
        player,
        this->getColor(),
        this->getScale()
    );
}

void Blip::showRouteFor(Player* player, uint8_t color, float scale) {
    std::vector<rage::IPlayer*> players;
    players.push_back(player->getRageMPHandle());

    this->getRageMPHandle()->RouteFor(players, color, scale);
}

void Blip::hideRouteFor(Player* player) {
    std::vector<rage::IPlayer*> players;
    players.push_back(player->getRageMPHandle());

    this->getRageMPHandle()->UnrouteFor(players);
}

uint8_t Blip::getColor() {
    return this->getRageMPHandle()->GetColour();
}

void Blip::setColor(uint8_t color) {
    this->getRageMPHandle()->SetColour(color);
}

float Blip::getScale() {
    return this->getRageMPHandle()->GetScale();
}

void Blip::setScale(float scale) {
    this->getRageMPHandle()->SetScale(scale);
}

std::string const& Blip::getName() {
    return this->getRageMPHandle()->GetName();
}

void Blip::setName(std::string const& name) {
    this->getRageMPHandle()->SetName(name);
}