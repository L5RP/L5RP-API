class PlayerSync {

    playerData = [];

    constructor() {
        mp.events.add(
            "entityStreamIn",
            this.entityStreamIn
        );

        mp.events.add(
            "playerQuit",
            this.entitiyStreamOut
        );

        mp.events.add(
            "entityStreamOut",
            this.entitiyStreamOut
        );

        mp.events.add(
            "setComponentVariation",
            this.setComponentVariation
        );

        mp.events.add(
            "setHeadOverlay",
            this.setHeadOverlay
        );

        mp.events.add(
            "updateCustomSyncedPlayerData",
            this.updateCustomSyncedData
        );
    }

    setComponentVariation = (id, component, drawable, texture, pallete) => {

        if (!this.isPlayerDataInitialized(id))
            this.initPlayerData(id);

        this.playerData[id].components[component] = {
            drawable: drawable,
            texture: texture,
            pallete: pallete
        };

        if (this.playerData[id].streamed)
            mp.players.atRemoteId(id).setComponentVariation(component, drawable, texture, pallete);

    };

    setHeadOverlay = (id, overlay, value, opacity, color, secondaryColor) => {

        if (!this.isPlayerDataInitialized(id))
            this.initPlayerData(id);

        this.playerData[id].overlays[overlay] = {
            value: value,
            opacity: opacity,
            color: color,
            secondaryColor: secondaryColor
        };

        if (this.playerData[id].streamed)
            mp.players.atRemoteId(id).setHeadOverlay(
                overlay,
                value,
                opacity,
                color,
                secondaryColor
            );

    };

    updateCustomSyncedData = (id, data) => {

        if (!this.isPlayerDataInitialized(id))
            this.initPlayerData(id);

        this.playerData[id].data = JSON.parse(data);

        if (this.playerData[id].streamed)
            this.callCustomSyncedDataApplyEvent(
                mp.players.atRemoteId(id),
                this.playerData[id].data
            );
    };

    callCustomSyncedDataApplyEvent = (ped, data) => {
        if (data !== null)
            mp.events.call(
                "onCustomPlayerDataApply",
                ped,
                data
            );
    };

    callCustomSyncedDataClearEvent = (ped, data) => {
        if (data !== null)
            mp.events.call(
                "onCustomPlayerDataClear",
                ped,
                data
            );
    };

    initPlayerData = (id) => {
        let local = (mp.players.local.remoteId === id);

        this.playerData[id] = {
            components: [
                {drawable: 0, texture: 0, pallete: 0},
                {drawable: 0, texture: 0, pallete: 0},
                {drawable: 0, texture: 0, pallete: 0},
                {drawable: 0, texture: 0, pallete: 0},
                {drawable: 0, texture: 0, pallete: 0},
                {drawable: 0, texture: 0, pallete: 0},
                {drawable: 0, texture: 0, pallete: 0},
                {drawable: 0, texture: 0, pallete: 0},
                {drawable: 0, texture: 0, pallete: 0},
                {drawable: 0, texture: 0, pallete: 0},
                {drawable: 0, texture: 0, pallete: 0},
                {drawable: 0, texture: 0, pallete: 0}
            ],
            overlays: [
                {value: 255, opacity: 0, color: 0, secondaryColor: 0},
                {value: 255, opacity: 0, color: 0, secondaryColor: 0},
                {value: 255, opacity: 0, color: 0, secondaryColor: 0},
                {value: 255, opacity: 0, color: 0, secondaryColor: 0},
                {value: 255, opacity: 0, color: 0, secondaryColor: 0},
                {value: 255, opacity: 0, color: 0, secondaryColor: 0},
                {value: 255, opacity: 0, color: 0, secondaryColor: 0},
                {value: 255, opacity: 0, color: 0, secondaryColor: 0},
                {value: 255, opacity: 0, color: 0, secondaryColor: 0},
                {value: 255, opacity: 0, color: 0, secondaryColor: 0},
                {value: 255, opacity: 0, color: 0, secondaryColor: 0},
                {value: 255, opacity: 0, color: 0, secondaryColor: 0},
                {value: 255, opacity: 0, color: 0, secondaryColor: 0}
            ],
            data: {
                item: null,
            },
            streamed: local,
            local: local
        }
    };

    isPlayerDataInitialized = (id) => {
        return this.playerData[id] !== undefined;
    };

    entityStreamIn = (p) => {

        if (p.type === 'player') {

            if (!this.isPlayerDataInitialized(p.remoteId))
                this.initPlayerData(p.remoteId);

            let playerData = this.playerData[p.remoteId];

            playerData.components.forEach(
                (component, index) => {
                    p.setComponentVariation(index, component.drawable, component.texture, component.pallete);
                }
            );

            playerData.overlays.forEach(
                (component, index) => {
                    p.setHeadOverlay(index, component.value, component.opacity, component.color, component.secondaryColor);
                }
            );

            p.setHelmet(false);

            this.callCustomSyncedDataApplyEvent(
                p,
                playerData.data
            );

            playerData.streamed = true;
        }

    };

    entitiyStreamOut = (p) => {

        if (p.type === 'player') {
            if (!this.isPlayerDataInitialized(p.remoteId))
                this.initPlayerData(p.remoteId);

            this.callCustomSyncedDataClearEvent(
                p,
                this.playerData[p.remoteId].data
            );

            this.playerData[p.remoteId].streamed = false;
        }

    };

}

export default new PlayerSync();