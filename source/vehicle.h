#pragma once

namespace L5RP {

    class Vehicle;
    class VehicleComponent;

    class VehicleManager
            : public SmartEntityManager,
              public rage::IEventHandler,
              public rage::IVehicleHandler
    {
        friend class Vehicle;

    protected:

        IVehicleHandler *GetVehicleHandler() override;

        void OnVehicleDeath(rage::IVehicle *vehicle, rage::hash_t hash, rage::IPlayer *killer) override;
        void OnVehicleSirenToggle(rage::IVehicle *vehicle, bool toggle) override;
        void OnVehicleHornToggle(rage::IVehicle *vehicle, bool toggle) override;
        void OnTrailerAttached(rage::IVehicle *vehicle, rage::IVehicle *trailer) override;
        void OnVehicleDamage(rage::IVehicle *vehicle, float bodyHealthLoss, float engineHealthLoss) override;

    public:

        explicit VehicleManager(Plugin* plugin);
        virtual ~VehicleManager() = default;

        void registerDefaultComponent(std::function<VehicleComponent*(Vehicle*)> fn);

        Vehicle* getVehicle(uint16_t id) const;
        Vehicle* getVehicle(std::string const& id) const;
        std::map<uint16_t, Vehicle*> const& getVehicles() const;
    };

    class Vehicle
            : public SmartEntity {

        friend class Player;
        friend class VehicleComponent;
        friend class VehicleManager;

        void OnDeath(uint32_t hash, Player* killer);
        void OnSirenToggle(bool toggle);
        void OnHornToggle(bool toggle);
        void OnTrailerAttached(Vehicle* trailer);
        void OnDamage(float bodyHealthLoss, float engineHealthLoss);

        Vehicle* trailer = nullptr;

        Transform spawnLocation;
        rage::dimensionId_t spawnDimension;
        bool autoRespawn = false;

        bool _nenabled = false;
        Color _ncolor = Color();
        void updateNeon();

        std::string model;

        std::array<bool, 2> blinkers = {
            false,
            false
        };

    public:

        Vehicle(
            VehicleManager* manager,
            std::string const& model,
            float x, float y, float z,
            float heading = 0,
            std::string const& numberPlate = "",
            uint8_t alpha = 255,
            bool locked = false,
            bool engine = false,
            rage::dimensionId_t dimension = 0
        );
        Vehicle(
                VehicleManager* manager,
                std::string const& model,
                Vector3 const& pos,
                float heading = 0,
                std::string const& numberPlate = "",
                uint8_t alpha = 255,
                bool locked = false,
                bool engine = false,
                rage::dimensionId_t dimension = 0
        );
        Vehicle(
            VehicleManager* manager,
            std::string const& model,
            Transform const& transform,
            std::string const& numberPlate = "",
            uint8_t alpha = 255,
            bool locked = false,
            bool engine = false,
            rage::dimensionId_t dimension = 0
        );
        virtual ~Vehicle();

        rage::IVehicle* getRageMPHandle() const;

        float calcKMHSpeed() const;

        bool isEngineActive() const;
        void setEngineActive(bool status);

        float getEngineHealth() const;
        float getBodyHealth() const;
        void fix();

        bool hasDriver() const;
        Player* getDriver() const;
        Player* getPlayer(uint8_t seat) const;
        bool isSeatUsed(uint8_t seat) const;
        std::vector<Player*> getPlayers() const;
        void removePlayersFromVehicle() const;

        bool isLocked() const;
        void lock(bool toggle);

        bool isDead() const;
        void kill();
        void revive(Vector3 const& pos, float heading);
        void revive(float x, float y, float z, float h);

        void respawn();
        void setSpawnLocation(Transform const& transform);
        void setSpawnLocation(float x, float y, float z, float h);
        void setSpawnDimension(rage::dimensionId_t dimension);
        Transform const& getSpawnLocation() const;
        rage::dimensionId_t getSpawnDimension() const;
        bool isAutoRespawnEnabled() const;
        void enableAutoRespawn(bool autoRespawn);

        void setMod(uint8_t index, uint8_t value);
        uint8_t getMod(uint8_t index) const;
        void setExtra(uint8_t id, bool enabled);
        bool getExtra(uint8_t id) const;

        void setWheelType(uint8_t type);
        uint8_t getWheelType() const;
        void setFrontWheel(uint8_t type, uint8_t mod);
        void setBackWheel(uint8_t type, uint8_t mod);

        void setLivery(uint8_t type);
        uint8_t getLivery() const;

        void setWindowTint(uint8_t type);
        uint8_t getWindowTint() const;

        bool isNeonEnabled() const;
        void enableNeon(bool toggle);
        void setNeonColour(Color const& col);
        void setNeonColour(uint8_t r, uint8_t g, uint8_t b);
        Color getNeonColour() const;

        Color getPrimaryColour() const;
        Color getSecondaryColour() const;
        void setPrimaryColour(Color const& color);
        void setPrimaryColour(uint8_t r, uint8_t g, uint8_t b);
        void setSecondaryColour(Color const& color);
        void setSecondaryColour(uint8_t r, uint8_t g, uint8_t b);
        uint8_t getWheelColour() const;
        void setWheelColour(uint8_t color);
        uint8_t getTrimColour() const;
        void setTrimColour(uint8_t color);
        uint8_t getDashboardColour() const;
        void setDashboardColour(uint8_t color);

        std::string const& getNumberPlate() const;
        void setNumberPlate(std::string const& plate);
        uint8_t getNumberPlateType() const;
        void setNumberPlateType(uint8_t type);

        void setHeading(float h);
        float getHeading();

        Transform getTransform();
        void setTransform(Transform const& transform);
        void setTransform(float x, float y, float z, float h);

        std::string const& getModelName() const;
        VehicleManager* getManager() const;

        bool isTrailerAttached() const;
        Vehicle* getAttachedTrailer() const;

        //Custom
        bool isBlinkerActive(uint8_t id) const;
        void setBlinkerActive(uint8_t id, bool toggle);
        bool isLeftTurnSignalActive() const;
        bool isRightTurnSignalActive() const;
        bool isEmergencySignalActive() const;
        void setLeftTurnSignalActive();
        void setRightTurnSignalActive();
        void setEmergencySignalActive();
        void disableAllTurnSignals();
    };

    class VehicleComponent
            : public EntityComponent {

        friend class Player;
        friend class Vehicle;

    protected:

        virtual void OnStartEnter(Player* vehicle, uint8_t seat) {};
        virtual void OnEnter(Player* vehicle, uint8_t seat) {};
        virtual void OnStartExit(Player* vehicle) {};
        virtual void OnExit(Player* vehicle) {};
        virtual void OnDeath(uint32_t hash, Player* killer) {}
        virtual void OnSirenToggle(bool toggle) {}
        virtual void OnHornToggle(bool toggle) {}
        virtual void OnTrailerAttached(Vehicle* trailer) {};
        virtual void OnTrailedDetached(Vehicle* trailer) {};
        virtual void OnDamage(float bodyHealthLoss, float engineHealthLoss) {};

    public:

        VehicleComponent(Vehicle* vehicle, Script* script);
        virtual ~VehicleComponent() = default;

        Vehicle* getVehicle() const;

    };
}