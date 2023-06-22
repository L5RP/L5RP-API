#pragma once

namespace L5RP {

    class Blip;

    class BlipManager
        : public EntityManager {
        friend class Blip;

    public:

        explicit BlipManager(Plugin* plugin);
        virtual ~BlipManager() = default;

        Blip* getBlip(uint16_t id) const;
        std::map<uint16_t, Blip*> const& getBlips() const;
    };

    class Blip
        : public Entity {

        bool visibleOnMap;

        void clientUpdate();

    public:

        Blip(
            BlipManager* manager,
            std::string const& title,
            uint32_t sprite,
            uint8_t color,
            Vector3 const& position,
            float scale = 1,
            bool shortRange = true,
            bool isVisibleOnMap = true,
            uint8_t alpha = 255,
            float drawDistance = 0.f,
            short rotation = 0,
            rage::dimensionId_t dimension = 0
        );
        virtual ~Blip() = default;

        float getDrawDistance();
        void setDrawDistance(float distance);

        short getBlipRotation();
        void setRotation(short rotation);

        bool isShortRanged();
        void setShortRanged(bool toggle);

        void showRouteFor(Player* player);
        void showRouteFor(Player* player, uint8_t color, float scale);
        void hideRouteFor(Player* player);

        uint8_t getColor();
        void setColor(uint8_t color);

        float getScale();
        void setScale(float scale);

        std::string const& getName();
        void setName(std::string const& name);

        bool isVisibleOnMap() const;
        void setIsVisibleOnMap(bool visible);

        rage::IBlip* getRageMPHandle() const;
    };

}