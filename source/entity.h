#pragma once

namespace L5RP
{

    class SmartEntity;
    class Entity;
    class EntityComponent;

    class EntityManager
        : public PluginAttachment
    {

        friend class Entity;

        std::map<uint16_t, Entity*> entities;

    protected:

        void registerEntity(Entity* entity);
        void unregisterEntity(Entity* entity);

    public:

        explicit EntityManager(Plugin* plugin);
        virtual ~EntityManager() = default;

        Entity* getEntity(rage::entityId_t id) const;
        std::map<uint16_t, Entity*> const& getEntities() const;
    };

    class SmartEntityManager
        : public EntityManager
    {

        std::vector<std::function<EntityComponent*(SmartEntity*)>> defaultComponents;

    public:

        explicit SmartEntityManager(Plugin* plugin);
        virtual ~SmartEntityManager() = default;

        void registerDefaultComponent(std::function<EntityComponent*(SmartEntity*)> const& fn);

        std::vector<std::function<EntityComponent*(SmartEntity*)>> const& getDefaultComponents() const;
    };

    class Entity
    {
        friend class EntityComponent;

        friend class Player;
        friend class Vehicle;
        friend class Blip;

        EntityManager* manager;
        rage::Entity* handle;

    protected:

        virtual rage::Entity* getRageMPHandle() const;

    public:

        Entity(rage::Entity* handle, EntityManager* manager);
        virtual ~Entity();

        uint16_t getID() const;
        uint8_t getType() const;

        void setDimension(uint32_t id);
        uint32_t getDimension() const;

        Vector3 getPosition();
        void setPosition(Vector3 const& pos);
        void setPosition(float x, float y, float z);

        Vector3 getRotation();
        void setRotation(Vector3 const& rot);
        void setRotation(float x, float y, float z);

        uint32_t getModel() const;
        void setModel(uint32_t model);

        uint8_t getAlpha() const;
        void setAlpha(uint8_t alpha);

        virtual EntityManager* getManager() const;

    };

    class SmartEntity
        : public Entity
    {
        friend class EntityComponent;

        std::map<uint32_t, EntityComponent*> components;

    protected:

        void registerComponent(EntityComponent* component);
        void unregisterComponent(EntityComponent* component);

    public:

        SmartEntity(rage::Entity* handle, SmartEntityManager* manager);
        virtual ~SmartEntity();

        virtual SmartEntityManager* getManager() const;

        template <typename T>
        T* getComponent(uint32_t scriptType) const
        {
            try {
                return (T*) this->components.at(scriptType);
            }

            catch(std::out_of_range const&) {
                return nullptr;
            }
        }
        std::map<uint32_t, EntityComponent*> const& getComponents() const;
    };

    class EntityComponent
    {

        SmartEntity* entity;
        Script* script;

    public:

        EntityComponent(SmartEntity* entity, Script* script);
        virtual ~EntityComponent();

        SmartEntity* getEntity() const;
        Script* getScript() const;

    };

}