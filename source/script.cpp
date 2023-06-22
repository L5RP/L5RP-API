#include "api.h"

using namespace L5RP;

void ScriptManager::onTick() {

    for(auto pair : this->_scripts)
        pair.second->onServerTick();

}

void ScriptManager::onServerFullyInitialized() {

    for(auto pair : this->_scripts)
        pair.second->onServerFullInit();

}

L5RP::ScriptManager::ScriptManager(Plugin* plugin)
        : PluginAttachment(plugin)
{

    this->_scripts = std::map<ScriptType, Script*>();

    std::vector<Script*> scripts = GetScriptList(this);
    for (Script* script : scripts)
        this->registerScript(script);

    initScripts();

}

L5RP::ScriptManager::~ScriptManager()
{

    freeScripts();

}

L5RP::Script* L5RP::ScriptManager::getRawScript(ScriptType type) const
{
    Script* result = nullptr;

    try {
        result = this->_scripts.at(type);
    }
    catch (std::out_of_range const&) {
        result = nullptr;
    }

    return result;
}

void L5RP::ScriptManager::initScripts()
{

    for (std::pair<ScriptType, Script*> pair : this->_scripts)
        pair.second->onServerInit();

}

void L5RP::ScriptManager::freeScripts()
{

    for (std::pair<ScriptType, Script*> pair : this->_scripts)
        pair.second->onServerExit();

    for (std::pair<ScriptType, Script*> pair : this->_scripts)
        delete pair.second;

    this->_scripts.clear();

}

void L5RP::ScriptManager::registerScript(Script* script)
{
    this->_scripts[script->getType()] = script;
}

L5RP::Script::Script(ScriptManager* manager, ScriptType type)
{

    this->_manager = manager;
    this->_type = type;

}

L5RP::ScriptManager* L5RP::Script::getManager() const
{
    return this->_manager;
}

L5RP::ScriptType L5RP::Script::getType() const
{
    return this->_type;
}