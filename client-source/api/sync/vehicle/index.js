class VehicleSync {

    vehicleData = [];

    constructor() {
        mp.events.add(
            "entityStreamIn",
            this.entityStreamIn
        );

        mp.events.add(
            "entityStreamOut",
            this.entitiyStreamOut
        );

        mp.events.add(
            "updateBlinker",
            this.updateBlinker
        );

        mp.events.add(
            "updateCustomSyncedVehicleData",
            this.updateCustomSyncedData
        );
    }

    updateBlinker = (id, blinker, toggle) => {

        if (!this.isVehicleDataInitialized(id))
            this.initVehicleData(id);

        this.vehicleData[id].blinkers[blinker] = toggle;

        if (this.vehicleData[id].streamed)
            mp.vehicles.atRemoteId(id).setIndicatorLights(
                blinker,
                toggle
            );

    };

    updateCustomSyncedData = (id, data) => {

        if (!this.isVehicleDataInitialized(id))
            this.initVehicleData(id);

        this.vehicleData[id].data = JSON.parse(data);

        if (this.vehicleData[id].streamed)
            this.callCustomSyncedDataApplyEvent(
                mp.vehicles.atRemoteId(id),
                this.vehicleData[id].data
            );
    };

    callCustomSyncedDataApplyEvent = (ped, data) => {
        if (data !== null)
            mp.events.call(
                "onCustomVehicleDataApply",
                ped,
                data
            );
    };

    callCustomSyncedDataClearEvent = (ped, data) => {
        if (data !== null)
            mp.events.call(
                "onCustomVehicleDataClear",
                ped,
                data
            );
    };

    initVehicleData = (id) => {
        this.vehicleData[id] = {
            blinkers: [
                false,
                false
            ],
            streamed: false
        }
    };

    isVehicleDataInitialized = (id) => {
        return this.vehicleData[id] !== undefined;
    };

    entityStreamIn = (p) => {

        if (p.type === 'vehicle') {

            if (!this.isVehicleDataInitialized(p.remoteId))
                this.initVehicleData(p.remoteId);

            let vehicleData = this.vehicleData[p.remoteId];

            vehicleData.blinkers.forEach(
                (blinker, id) => {
                    p.setIndicatorLights(id, blinker);
                }
            );

            this.callCustomSyncedDataApplyEvent(
                p,
                vehicleData.data
            );

            vehicleData.streamed = true;
        }

    };

    entitiyStreamOut = (p) => {

        if (p.type === 'vehicle') {
            if (!this.isVehicleDataInitialized(p.remoteId))
                this.initVehicleData(p.remoteId);

            this.callCustomSyncedDataClearEvent(
                p,
                this.vehicleData[p.remoteId].data
            );

            this.vehicleData[p.remoteId].streamed = false;
        }

    };

}

export default new VehicleSync();