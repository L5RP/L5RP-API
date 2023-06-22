# [C++] L5RP Framework - RageMP

This framework contains some additional util features which are not present in cppsdk by default:

- Callable system for parsing command/event/console input/socket input arguments.
- Script system, which helps developers to split the code into seperate scripts.
- Some utilities such as Player and Vehicle components to fully utilize script system.
- Threaded CURL support (Could be used to call api calls in website to load/save server data)
- Threaded TCP Socket support (Could be used to integrate server with 3rd party applications such as discord bot, live players map and a lot more) (Currently only works on linux)
- Has been designed with an possibility in mind that rage-mp could die and scripts code could be lifted to other modifications. (Of course integration layer would be needed to be developed, but the thing is that once that would be developed scripts code wouldn't need to change at all)
- Command and Event system. (Currently its done in a bit weird way, where each player has his own set of commands. But Its not really good time for me to go and refactor one of the core parts of the framework)
- A lot of small utilites that makes developer's life easier whilst developing on cppsdk.
- Some bugfixes which are not present in ragemp.
- Timer/Promise support. (Promise is the same thing as a timer, but only executes once)
- Threaded Console Input/Output.
- Console Output being done through spdlog, having different severity levels and being logged into file.

Ideas for the future:

- Example gamemode utilizing functions from this framework.
- Support for multi-language scripts.
- Some kind of documentation for this framework.

All credits goes to @Kudze from RageMP : [Kudze](https://rage.mp/files/file/92-c-l5rp-framework/)
This code is 4 years old, it might be end in some issues, a guide how to use and compile will be added at some time.
