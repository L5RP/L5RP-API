class CheckPointManager {

    currCheckpoint = null;
    currBlip = null;

    constructor() {

        setInterval(
            this.checkPointTick,
            10000
        );

        mp.events.add(
            "checkPointTick",
            this.checkPointTick
        );

    }

    checkPointTick = () => {

        if(this.currCheckpoint !== null) {
            let checkpoint = mp.checkpoints.at(this.currCheckpoint);

            if(checkpoint === null || !checkpoint.visible) {
                let blip = mp.blips.at(this.currBlip);
                blip.destroy();

                this.currCheckpoint = null;
                this.currBlip = null;
            }

            else {
                let blip = mp.blips.at(this.currBlip);

                blip.position = checkpoint.position;
            }
        }

        if(this.currCheckpoint === null) {
            mp.checkpoints.forEach(
                (checkpoint) => {
                    if(checkpoint.visible) {
                        let blip = mp.blips.new(
                            1,
                            checkpoint.position,
                            {
                                name: "Tikslas",
                                color: 1
                            }
                        );
                        blip.setRoute(true);

                        this.currCheckpoint = checkpoint.id;
                        this.currBlip = blip.id;
                    }
                }
            )
        }

    };

}

export default new CheckPointManager();