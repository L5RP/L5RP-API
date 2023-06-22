#pragma once

namespace L5RP
{
    class Player;
    class PlayerComponent;
    class PlayerEvent;
    class PlayerCommand;

    class PlayerManager
        :
            public SmartEntityManager,
            public rage::IEventHandler,
            public rage::IPlayerHandler,
            public rage::ICheckpointHandler
    {

    public:

        explicit PlayerManager(Plugin* plugin);
        virtual ~PlayerManager() = default;

        void registerDefaultComponent(std::function<PlayerComponent*(Player*)> fn);

        virtual IPlayerHandler* GetPlayerHandler();
        void OnPlayerJoin(rage::IPlayer* player) override;
        void OnPlayerReady(rage::IPlayer* player) override;
        void OnPlayerQuit(rage::IPlayer* player, rage::exit_t exitType, const char* reason) override;

        void OnPlayerCommand(rage::IPlayer* player, const std::u16string& SQLCommand) override;
        void OnPlayerChat(rage::IPlayer* player, const std::u16string& text) override;

        void OnPlayerDeath(rage::IPlayer* player, rage::hash_t reason, rage::IPlayer* killer) override;
        void OnPlayerSpawn(rage::IPlayer* player) override;

        void OnPlayerRemoteEvent(
            rage::IPlayer* player,
            uint64_t eventName,
            const rage::args_t& args
        ) override;

        void OnPlayerStartEnterVehicle(rage::IPlayer* player, rage::IVehicle* vehicle, uint8_t seatId) override;
        void OnPlayerEnterVehicle(rage::IPlayer* player, rage::IVehicle* vehicle, uint8_t seatId) override;
        void OnPlayerStartExitVehicle(rage::IPlayer* player, rage::IVehicle* vehicle) override;
        void OnPlayerExitVehicle(rage::IPlayer* player, rage::IVehicle* vehicle) override;

        ICheckpointHandler* GetCheckpointHandler() override;
        void OnPlayerEnterCheckpoint(rage::IPlayer* player, rage::ICheckpoint* checkpoint) override;
        void OnPlayerExitCheckpoint(rage::IPlayer* player, rage::ICheckpoint* checkpoint) override;

        void sendGlobalMessage(std::string const& message);

        std::vector<Player*> getPlayersInRange(Vector3 const& pos, float range);
        Player* getPlayer(uint16_t id) const; //If player is not found it returns nullptr
        Player* getPlayer(std::string const& name) const;
        std::map<uint16_t, Player*> const& getPlayers() const;
    };

    class PlayerComponent;

    struct ComponentVariation
    {
        uint16_t drawable;
        uint8_t texture;
        uint8_t pallete;

        ComponentVariation(uint16_t drawable, uint8_t texture, uint8_t pallete)
        {
            this->drawable = drawable;
            this->texture = texture;
            this->pallete = pallete;
        }
    };

    struct HeadOverlay
    {
        uint8_t value;
        uint8_t color;
        uint8_t secondaryColor;
        float opacity;

        HeadOverlay(uint8_t value, float opacity, uint8_t color, uint8_t secondaryColor)
        {
            this->value = value;
            this->color = color;
            this->opacity = opacity;
            this->secondaryColor = secondaryColor;
        }

    };

    class Player
        : public SmartEntity
    {

        friend class PlayerManager;
        friend class CheckPoint;

        //Event Functions.
        void OnReady();
        void OnDisconnect(rage::exit_t exitType, const char* reason);
        void OnDeath(rage::hash_t reason, Player* killer);
        void OnSpawn();
        void OnMessage(const std::string& text);
        void OnStartEnterVehicle(Vehicle* vehicle, uint8_t seat);
        void OnEnterVehicle(Vehicle* vehicle, uint8_t seat);
        void OnStartExitVehicle(Vehicle* vehicle);
        void OnExitVehicle(Vehicle* vehicle);
        void OnRemoteEvent(uint64_t eventName, std::vector<std::string> const& args);
        void OnCommand(std::string const& cmd, std::string const& args);
        void OnEnterCheckPoint(CheckPoint* checkPoint);
        void OnExitCheckPoint(CheckPoint* checkPoint);

        void initSyncedData();

        void setHeadBlend(uint8_t faceShape, float skinColour);

        //I hate workarounds.
        float skinColour = 0;
        bool isConnected = true;

        //ComponentVariaton workarounds. HATING SHITTY LINUX RAGEMP BUILDS.
        std::array<ComponentVariation, 12> componentVariations = {
            ComponentVariation(0, 0, 0),
            ComponentVariation(0, 0, 0),
            ComponentVariation(0, 0, 0),
            ComponentVariation(0, 0, 0),
            ComponentVariation(0, 0, 0),
            ComponentVariation(0, 0, 0),
            ComponentVariation(0, 0, 0),
            ComponentVariation(0, 0, 0),
            ComponentVariation(0, 0, 0),
            ComponentVariation(0, 0, 0),
            ComponentVariation(0, 0, 0),
            ComponentVariation(0, 0, 0)
        };
        std::array<HeadOverlay, 13> headOverlays = {
            HeadOverlay(255, 1, 0, 0),
            HeadOverlay(255, 1, 0, 0),
            HeadOverlay(255, 1, 0, 0),
            HeadOverlay(255, 1, 0, 0),
            HeadOverlay(255, 1, 0, 0),
            HeadOverlay(255, 1, 0, 0),
            HeadOverlay(255, 1, 0, 0),
            HeadOverlay(255, 1, 0, 0),
            HeadOverlay(255, 1, 0, 0),
            HeadOverlay(255, 1, 0, 0),
            HeadOverlay(255, 1, 0, 0),
            HeadOverlay(255, 1, 0, 0),
            HeadOverlay(255, 1, 0, 0)
        };

        nlohmann::json customData;

    public:

        rage::IPlayer* getRageMPHandle() const;

        enum Gender
        {
            FEMALE = 0,
            MALE = 1,
            BIMALE = 2
        };

        Player(PlayerManager* manager, rage::IPlayer* player);

        std::string const& getHWID() const;
        std::string const& getSocialClubName() const;

        // --- Appearance.
        void setHeadOverlay(uint8_t id, HeadOverlay const& overlay);
        void setHeadOverlay(uint8_t id, uint8_t value, uint8_t color, float opacity = 1);
        void setHeadOverlay(uint8_t id, uint8_t value, uint8_t color, uint8_t secondaryColour, float opacity = 1);
        uint8_t getHeadOverlay(uint8_t id) const;
        float getHeadOverlayOpacity(uint8_t id) const;
        uint8_t getHeadOverlayColor(uint8_t id) const;
        uint8_t getHeadOverlaySecondaryColor(uint8_t id) const;

        void setPropertyVariation(uint8_t id, uint8_t drawable, uint8_t texture);
        uint8_t getPropertyDrawable(uint8_t id) const;
        uint8_t getPropertyTexture(uint8_t id) const;

        void setComponentVariation(uint8_t id, uint16_t drawable, uint8_t texture, uint8_t palette);
        void setComponentVariation(uint8_t id, ComponentVariation const& data);
        uint16_t getComponentDrawable(uint8_t id) const;
        uint8_t getComponentTexture(uint8_t id) const;
        uint8_t getComponentPalette(uint8_t id) const;
        ComponentVariation const& getComponentVariation(uint8_t id) const;
        std::array<ComponentVariation, 12> const& getComponentVariations() const;

        nlohmann::json& getCustomDataReference();
        void updateCustomData() const;

        void setGender(Gender gender);
        Gender getGender() const;

        void setFaceShape(uint8_t faceShape);
        uint8_t getFaceShape() const;

        void setSkinColour(float skinColour);
        float getSkinColour() const;

        void setEyeColour(uint8_t id);
        uint8_t getEyeColour() const;

        void setHaircut(uint8_t id);
        uint8_t getHaircut() const;

        void setHairColour(uint8_t color);
        uint8_t getHairColour() const;

        void setHairHighlightColour(uint8_t color);
        uint8_t getHairHighlightColour() const;

        void setEyeBrows(uint8_t id);
        uint8_t getEyeBrows() const;

        void setEyeBrowsColour(uint8_t color);
        uint8_t getEyeBrowsColour() const;

        void setBeard(uint8_t id);
        uint8_t getBeard() const;

        void setBeardColour(uint8_t color);
        uint8_t getBeardColour() const;

        void setFaceBlemishes(uint8_t id);
        uint8_t getFaceBlemishes() const;

        void setFaceAge(uint8_t id);
        uint8_t getFaceAge() const;

        void setFaceComplexion(uint8_t id);
        uint8_t getFaceComplexion() const;

        void setFaceFreckles(uint8_t id);
        uint8_t getFaceFreckles() const;

        void setFaceSunDamage(uint8_t id);
        uint8_t getFaceSunDamage() const;

        void setChestHair(uint8_t id);
        uint8_t getChestHair() const;

        void setChestHairColour(uint8_t color);
        uint8_t getChestHairColour() const;

        void setBodyBlemishes(uint8_t id);
        uint8_t getBodyBlemishes() const;

        void setDenseBodyBlemishes(uint8_t id);
        uint8_t getDenseBodyBlemishes() const;

        void setFaceFeature(uint8_t id, float value);
        float getFaceFeature(uint8_t id) const;

        void kick(std::string const& reason);
        void ban(std::string const& reason); // <--- Lets not use this one since we will have proper admin system which handles it.

        void sendMessage(std::string const& message);
        void sendLocalMessage(std::string const& message, float range);
        void sendGlobalMessage(std::string const& message);

        void sendNotification(std::string const& message);

        std::string const& getName() const;
        void setName(std::string const& name);

        void requestIPL(std::string const& ipl);
        void removeIPL(std::string const& ipl);

        float getHeading() const;
        void setHeading(float heading);

        Transform getTransform();
        void setTransform(Transform const& transform);
        void setTransform(float x, float y, float z, float h);

        float getHealth() const;
        void setHealth(float health);
        bool isDead() const;

        float getArmour() const;
        void setArmour(float armour) const;

        void revive(Vector3 const& pos, float h);
        void revive(float x, float y, float z, float h);

        bool isInVehicle() const;
        Vehicle* getVehicle() const;
        int8_t getSeat() const;
        bool isInDriverSeat() const;
        void setVehicle(Vehicle* vehicle, int8_t seat = -1);
        void removeFromVehicle();

        void connectVoiceTo(Player* player);
        void disconnectVoiceFrom(Player* player);
        std::vector<Player*> getConnectedPlayersByVoice() const;

        std::string getIp() const;
        int getPing() const;
        float getPacketLoss() const;

        uint32_t getActiveWeapon() const;
        void setActiveWeapon(uint32_t weapon);
        uint16_t getActiveWeaponAmmo() const;
        void setActiveWeaponAmmo(uint16_t ammo);

        uint16_t getWeaponAmmo(uint32_t weapon) const;
        void setWeaponAmmo(uint32_t weapon, uint16_t ammo);

        std::map<uint32_t, uint16_t> getWeapons() const;
        void giveWeapon(uint32_t weapon, uint16_t ammo);
        void removeWeapon(uint32_t weapon);
        void removeWeapons() const;

        template <typename ...Args>
        void callClientEvent(const std::string& eventName, Args&& ... args)
        {
            const int count = sizeof...(Args);

            if(eventName != "npc_sync_ped" && eventName != "npc_sync_veh")
                std::cout << "Executing " << eventName << " for Player: " << this->getName() << std::endl;

            if constexpr (count == 0)
                this->getRageMPHandle()->_Call(eventName);
            else {
                rage::arg_t arguments[count] = {rage::arg_t(std::forward<Args>(args))...};
                this->getRageMPHandle()->_Call(eventName, arguments, count);
            }
        }

        void callServerEvent(
            std::string const& eventName,
            std::string const& args
        );
        void callServerEvent(
            std::string const& eventName,
            std::vector<std::string> const& args
        );
        void callServerEvent(
            std::string const& eventName,
            std::vector<ArgumentValue> const& args
        );

        std::vector<Player*> findClosestPlayers(float maxDist);
        Player* findClosestPlayer(float maxDist = 999.0f);

        PlayerManager* getManager() const;
    };

    class PlayerComponent
        : public EntityComponent
    {

        friend class Player;
        friend class PlayerEvent;
        friend class PlayerCommand;

        std::map<uint64_t, PlayerEvent*> events;
        std::map<std::string, PlayerCommand*> commands;

        void registerEvent(PlayerEvent* event);
        void registerCommand(PlayerCommand* command, std::string const& name);

    protected:

        virtual void OnInitialized() {};

        //This one is called always
        virtual void OnDisconnect() {};

        //This one is called before onDisconnect and only if player timeouts
        virtual void OnTimeout() {};

        // Same as timeout but only if player gets kicked.
        virtual void OnKicked(std::string const& reason) {};

        virtual void OnDeath(rage::hash_t reason, Player* killer) {};
        virtual void OnSpawn() {};
        virtual void OnMessage(std::string const& text) {};
        virtual void OnStartEnterVehicle(Vehicle* vehicle, uint8_t seat) {};
        virtual void OnEnterVehicle(Vehicle* vehicle, uint8_t seat) {};
        virtual void OnStartExitVehicle(Vehicle* vehicle) {};
        virtual void OnExitVehicle(Vehicle* vehicle) {};
        virtual void OnEnterCheckPoint(CheckPoint* checkPoint) {};
        virtual void OnExitCheckPoint(CheckPoint* checkPoint) {};

    public:

        PlayerComponent(Player* player, Script* script);
        virtual ~PlayerComponent() = default;

        Player* getPlayer() const;
        std::map<uint64_t, PlayerEvent*> const& getEvents() const;
        std::map<std::string, PlayerCommand*> const& getCommands() const;

    };

    class PlayerEvent
        : public Callable<void>
    {

        PlayerComponent* component;
        std::string eventName;
        std::function<void(std::vector<ArgumentValue> const&)> function;

    public:

        PlayerEvent(
            PlayerComponent* component,
            std::string const& eventName,
            std::function<void(std::vector<ArgumentValue> const&)> const& function,
            std::vector<IArgument*> const& mandatoryArgs,
            std::vector<IArgument*> const& optionalArgs
        );

        void onExecute(std::string const& args);
        void onExecute(std::vector<std::string> const& args);
        void onExecute(std::vector<ArgumentValue> const& args) override;

        PlayerComponent* getComponent() const;
        std::string const& getName() const;

    };

    class PlayerCommand
        : public Callable<void>
    {

    public:

        class Argument
        {

            IArgument* argument;
            std::string name;
            std::string desc;

        public:

            Argument(
                IArgument* argument,
                std::string const& name,
                std::string const& desc
            );

            IArgument* getArgument() const;
            PlayerComponent* getComponent() const;
            std::string const& getName() const;
            std::string const& getDescription() const;

        };

    private:

        PlayerComponent* component;
        std::string desc;
        std::vector<std::string> names;
        std::vector<Argument*> mandatoryArgs;
        std::vector<Argument*> optionalArgs;
        std::function<void(std::vector<ArgumentValue> const&)> function;

    protected:

        void throwArgsCountException() override;

    public:

        PlayerCommand(
            PlayerComponent* component,
            std::function<void(std::vector<ArgumentValue> const&)> const& function,
            std::initializer_list<std::string> const& names,
            std::initializer_list<Argument*> const& mandatoryArgs,
            std::initializer_list<Argument*> const& optionalArgs,
            std::string const& desc
        );
        virtual ~PlayerCommand();

        void onExecute(std::string const& args) override;
        void onExecute(std::vector<std::string> const& args) override;
        void onExecute(std::vector<ArgumentValue> const& args) override;

        virtual bool canExecute() const { return true; }

        PlayerComponent* getComponent() const;
        std::string const& getDescription() const;
    };

}

std::vector<L5RP::IArgument*> ARGUMENT_CONVERTER(std::initializer_list<L5RP::PlayerCommand::Argument*> args);