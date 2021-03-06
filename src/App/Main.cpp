#include "core.hpp"
#include <GLFW/glfw3.h>
#include "App.hpp"
#include "Audio/Audio.hpp"
#include "DefaultResourcePaths.hpp"
#include "GameEvents.hpp"
#include "Logging.hpp"
#include "PerfTimers.hpp"

void initialEvent(){
    auto startGame = event<StartGame>();
}

int main(int argc, char** argv){
    // std::atexit(pmk::dumpLogBufferTofile);
    // std::atexit(GpuTimerScoped::writeToFile);
    // std::atexit(CpuTimerScoped::writeToFile);

    CpuTimerScoped::printRecords = false;
    CpuTimerScoped::saveRecords = true;

    resolvePaths(argv[0]);

    Audio::Device device;
    if(not device.init()) return -1;


    App app;
    if(not app.initialize()){
        error("Failed to initialize app");
        return -1;
    }

    app.setCommonCallbacks();

    app.loadResources();

    initialEvent();
    app.run();

    app.finish();

    return 0;
}
