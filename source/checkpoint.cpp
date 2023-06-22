#include "api.h"

using namespace L5RP;

CheckPointManager::CheckPointManager(Plugin* plugin)
    : EntityManager(plugin)
{

}

L5RP::CheckPoint* CheckPointManager::getCheckpoint(rage::entityId_t id) const
{
    return (CheckPoint*) this->getEntity(id);
}

std::map<uint16_t, L5RP::CheckPoint*> const& CheckPointManager::getCheckpoints() const
{
    return (std::map<uint16_t, CheckPoint*> const&) this->getEntities();
}

rage::ICheckpoint* CheckPoint::getRageMPHandle() const
{
    return (rage::ICheckpoint*) Entity::getRageMPHandle();
}

CheckPoint::CheckPoint(
    CheckPointManager* manager,
    uint8_t type,
    Vector3 const& position,
    Vector3 const& nextPos,
    float radius,
    ColorAlpha const& color,
    bool visible,
    rage::dimensionId_t dimension
)
    : Entity(
    manager->getPlugin()->getRageMP()->GetCheckpointPool().New(
        type,
        {
            position.getX(),
            position.getY(),
            position.getZ() - 2
        },
        {
            nextPos.getX(),
            nextPos.getY(),
            nextPos.getZ() - 2
        },
        radius,
        rage::rgba_t(
            color.getR(),
            color.getG(),
            color.getB(),
            color.getA()
        ),
        visible,
        dimension
    ),
    manager
)
{

    if(!visible) {
        for(auto pair : this->getManager()->getPlugin()->getPlayerManager()->getPlayers()) {
            auto player = pair.second;

            this->hideForPlayer(player);
        }
    }

}

CheckPoint::~CheckPoint() {

    for(auto pair : this->getManager()->getPlugin()->getPlayerManager()->getPlayers()) {
        auto player = pair.second;

        this->hideForPlayer(player);
    }

}

ColorAlpha CheckPoint::getColor()
{
    return ColorAlpha(
        this->getRageMPHandle()->GetColour()
    );
}

void CheckPoint::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    this->getRageMPHandle()->SetColour(
        r,
        g,
        b,
        a
    );
}

void CheckPoint::setColor(ColorAlpha const& color)
{
    this->setColor(
        color.getR(),
        color.getG(),
        color.getB(),
        color.getA()
    );
}

float CheckPoint::getRadius()
{
    return this->getRageMPHandle()->GetRadius();
}

void CheckPoint::setRadius(float radius)
{
    this->getRageMPHandle()->SetRadius(radius);
}

bool CheckPoint::isVisible()
{
    return this->getRageMPHandle()->IsVisible();
}

void CheckPoint::setVisible(bool visible)
{
    return this->getRageMPHandle()->SetVisible(visible);
}

void CheckPoint::showForPlayer(Player* player)
{
    std::vector<rage::IPlayer*> players;
    players.push_back(player->getRageMPHandle());

    this->getRageMPHandle()->ShowFor(players);
}

void CheckPoint::hideForPlayer(Player* player)
{
    std::vector<rage::IPlayer*> players;
    players.push_back(player->getRageMPHandle());

    this->getRageMPHandle()->HideFor(players);
}