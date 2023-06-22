#pragma once

#define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING

//C++
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <map>
#include <string>
#include <codecvt>
#include <sstream>
#include <cstring>
#include <fstream>
#include <cuchar>
#include <chrono>
#include <algorithm>
#include <cctype>
#include <exception>
#include <experimental/filesystem>
#include <functional>
#include <cmath>
#include <regex>

//Sockets
#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#pragma comment(lib, "Ws2_32.lib")

//This should solve compatibility issues
#undef max
#undef min

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#endif

//BCrypt
#include <hashlibpp.h>

//Rage
#include <rage.hpp>

//INI
#include <SimpleIni.h>

//JSON
#include <json.hpp>

//Spdlog
//Install using linux: apt-get install libspdlog-dev
#include <spdlog.h>

//xxHash
#include <xxhash.hpp>

//curl
#include <curl/curl.h>

//Not really sure why rage uses extern C.
//I'd like to be able to return vectors.
//And i don't really mind function names being decorated.
#ifdef _WIN32
#define L5RP_API __declspec(dllexport)
#else
#define L5RP_API
#endif

//L5RP
namespace L5RP {
    class Console;
    class Plugin;
    class PluginAttachment;
    class Entity;
    class EntityManager;
    class EntityComponent;
    class Player;
    class PlayerManager;
    class PlayerComponent;
    class PlayerEvent;
    class PlayerCommand;
    class Script;
    class ScriptManager;
    class IArgument;
    class PlayerEvent;
    class Timer;
    class TimerManager;
    class Vehicle;
    class VehicleManager;
    class VehicleComponent;
    class CurlManager;
    class CurlTask;
    class Console;
    class Clock;
    class TimePoint;
    class CheckPoint;
    class CheckPointManager;
    class SocketManager;
    class SocketListener;
    class Socket;
    class SocketCommand;
    class Exception;
    class ChatException;
    class Blip;
    class BlipManager;

    L5RP_API std::vector<L5RP::Script*> GetScriptList(ScriptManager* manager);
}

std::u16string UTF8_TO_UTF16(std::string const& str);
std::string UTF16_TO_UTF8(std::u16string const& str);
uint32_t HASH_TO_GAME(std::string const& string);
bool STRING_CONTAINS_IGNORE_CASE(std::string const& s1, std::string const& s2);

//API
#include "util.h"
#include "exception.h"
#include "callable.h"
#include "config.h"
#include "plugin.h"
#include "curl.h"
#include "entity.h"
#include "vehicle.h"
#include "player.h"
#include "checkpoint.h"
#include "blip.h"
#include "console.h"
#include "timer.h"
#include "clock.h"
#include "socket.h"
#include "script.h"