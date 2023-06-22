class BlipManager {

    constructor() {

        mp.events.add(
            "updateBlipData",
            this.blipUpdate
        );

        mp.events.add(
            "initBlipData",
            this.blipInit
        );

    }

    blipInit = (_data) => {
        let data = JSON.parse(_data);

        data.blips.forEach(
            (blipData) => {
                this.blipUpdate(
                    blipData.id,
                    blipData.visible
                );
            }
        )
    };

    blipUpdate = (id, visible) => {
        let blip = mp.blips.atRemoteId(id);

        if (blip === null || blip === undefined)
            setTimeout(
                this.blipUpdate(id, visible),
                1000
            );

        else {
            mp.blips.atRemoteId(id).setDisplay(
                visible ? 2 : 5
            );
        }
    }

}

export default new BlipManager();