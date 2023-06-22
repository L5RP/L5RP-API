#pragma once

namespace L5RP {

    class CheckPoint;

    class CheckPointManager
        : public EntityManager {
        friend class CheckPoint;

    public:

        explicit CheckPointManager(Plugin* plugin);
        virtual ~CheckPointManager() = default;

        CheckPoint* getCheckpoint(uint16_t id) const;
        std::map<uint16_t, CheckPoint*> const& getCheckpoints() const;
    };

    class CheckPoint
        : public Entity {

    public:

        CheckPoint(
            CheckPointManager* manager,
            uint8_t type,
            Vector3 const& position,
            Vector3 const& nextPos,
            float radius,
            ColorAlpha const& color,
            bool visible = true,
            rage::dimensionId_t dimension = 0
        );
        virtual ~CheckPoint();

        ColorAlpha getColor();
        void setColor(ColorAlpha const& color);
        void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

        //Not adding direction because its probably normalized vector between positions.
        //We will not use it anyways probably. And would require to add some maths for it.

        float getRadius();
        void setRadius(float radius);

        bool isVisible();
        void setVisible(bool visible);

        //Idk ragemp function is a vector. So lets hope that it doesnt reset old players.
        //If it indeed does reset then I'll do a workaround.
        void showForPlayer(Player* player);
        void hideForPlayer(Player* player);

        rage::ICheckpoint* getRageMPHandle() const;
    };

}