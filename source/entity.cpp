#include "api.h"

using namespace L5RP;

void EntityManager::registerEntity(Entity* entity)
{

    this->entities[
            entity->getID()
    ] = entity;

}

void EntityManager::unregisterEntity(Entity* entity)
{
    this->entities.erase(
            entity->getID()
    );
}

EntityManager::EntityManager(Plugin* plugin)
        : PluginAttachment(plugin)
{}

Entity* EntityManager::getEntity(rage::entityId_t id) const
{
    try {
        return entities.at(id);
    } catch(std::out_of_range const&) {
        return nullptr;
    }
}

std::map<uint16_t, Entity*> const& EntityManager::getEntities() const
{
    return this->entities;
}

SmartEntityManager::SmartEntityManager(Plugin* plugin)
    : EntityManager(plugin)
{

}

void SmartEntityManager::registerDefaultComponent(std::function<EntityComponent*(SmartEntity*)> const& fn)
{
    this->defaultComponents.push_back(fn);
}

std::vector<std::function<EntityComponent*(SmartEntity*)>> const& SmartEntityManager::getDefaultComponents() const {
    return this->defaultComponents;
}

Entity::Entity(rage::Entity* handle, EntityManager* manager)
{
    this->manager = manager;
    this->handle = handle;

    this->manager->registerEntity(this);
}

Entity::~Entity()
{
    this->manager->unregisterEntity(this);

    auto handle = this->handle;

    switch(this->getType()) {
        case 0:

            return;
        case 1:
            new Promise(
                this->getManager()->getPlugin()->getTimerManager(),
                std::chrono::seconds(1),
                [handle](){
                    handle->Destroy();
                }
            );

            return;
        default:
            this->handle->Destroy();
    }
}

uint16_t Entity::getID() const
{
    return this->handle->GetId();
}

uint8_t Entity::getType() const
{
    return (uint8_t) this->handle->GetType();
}

void Entity::setDimension(uint32_t id)
{
    this->handle->SetDimension(id);
}

uint32_t Entity::getDimension() const
{
    return this->handle->GetDimension();
}

Vector3 Entity::getPosition()
{
    return Vector3(this->handle->GetPosition());
}

void Entity::setPosition(Vector3 const& pos)
{
    this->handle->SetPosition(
            {
                    pos.getX(),
                    pos.getY(),
                    pos.getZ()
            }
    );
}

void Entity::setPosition(float x, float y, float z)
{
    this->handle->SetPosition(
            {
                    x,
                    y,
                    z
            }
    );
}

Vector3 Entity::getRotation()
{
    return Vector3(this->handle->GetRotation());
}

void Entity::setRotation(Vector3 const& rot)
{
    this->handle->SetRotation(
            {
                    rot.getX(),
                    rot.getY(),
                    rot.getZ()
            }
    );
}

void Entity::setRotation(float x, float y, float z)
{
    this->handle->SetRotation(
            {
                    x,
                    y,
                    z
            }
    );
}

uint32_t Entity::getModel() const
{
    return this->handle->GetModel();
}

void Entity::setModel(uint32_t model)
{
    this->handle->SetModel(model);
}

uint8_t Entity::getAlpha() const
{
    return this->handle->GetAlpha();
}

void Entity::setAlpha(uint8_t alpha)
{
    this->handle->SetAlpha(alpha);
}

EntityManager* Entity::getManager() const
{
    return this->manager;
}

rage::IEntity* Entity::getRageMPHandle() const
{
    return this->handle;
}

void SmartEntity::registerComponent(EntityComponent* component)
{
    this->components[
        component->getScript()->getType()
    ] = component;
}

void SmartEntity::unregisterComponent(EntityComponent* component)
{
    this->components[
        component->getScript()->getType()
    ] = nullptr;
}

SmartEntity::SmartEntity(rage::Entity* handle, SmartEntityManager* manager)
    : Entity(handle, manager)
{
    manager->getPlugin()->getConsole()->trace("Creating default components for entity...");

    for (std::function<EntityComponent*(SmartEntity*)> const& fn : manager->getDefaultComponents())
        fn(this);

    manager->getPlugin()->getConsole()->trace("Created default components for entity!");
}

SmartEntity::~SmartEntity() {
    for(auto pair : this->components)
        delete pair.second;
    this->components.clear();
}

SmartEntityManager* SmartEntity::getManager() const {
    return (SmartEntityManager*) Entity::getManager();
}

std::map<uint32_t, EntityComponent*> const& SmartEntity::getComponents() const
{
    return this->components;
}

EntityComponent::EntityComponent(SmartEntity* entity, Script* script)
{
    this->entity = entity;
    this->script = script;

    this->entity->registerComponent(this);

    this->getEntity()->getManager()->getPlugin()->getConsole()->trace(
            "Creating component for "
            + std::to_string((uint8_t) entity->getType())
            + " from "
            + std::to_string(script->getType())
    );
}

EntityComponent::~EntityComponent() {
    this->entity->unregisterComponent(this);
}

SmartEntity* EntityComponent::getEntity() const
{
    return this->entity;
}

Script* EntityComponent::getScript() const
{
    return this->script;
}