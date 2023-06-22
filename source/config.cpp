#include "api.h"

void L5RP::INIFile::_loadData()
{

    _iniFile.LoadFile(this->_path.c_str());

}

void L5RP::INIFile::_saveData()
{

    _iniFile.SaveFile(this->_path.c_str());

}

L5RP::INIFile::INIFile(std::string const& path)
{

    this->_path = path;

    this->_loadData();

}

L5RP::INIFile::~INIFile()
{

    this->_saveData();

}

void L5RP::INIFile::save()
{
    this->_saveData();
}

bool L5RP::INIFile::doesValueExist(std::string const& section, std::string const& key)
{
    return ((void*) this->_iniFile.GetValue(section.c_str(), key.c_str(), NULL)) != nullptr;
}

std::string L5RP::INIFile::readValueAsString(std::string const& section, std::string const& key)
{
    return std::string(this->_iniFile.GetValue(section.c_str(), key.c_str(), ""));
}

double L5RP::INIFile::readValueAsDouble(std::string const& section, std::string const& key)
{
    return this->_iniFile.GetDoubleValue(section.c_str(), key.c_str(), 0);
}

bool L5RP::INIFile::readValueAsBool(std::string const& section, std::string const& key)
{
    return this->_iniFile.GetBoolValue(section.c_str(), key.c_str(), false);
}

long L5RP::INIFile::readValueAsLong(std::string const& section, std::string const& key)
{
    return this->_iniFile.GetLongValue(section.c_str(), key.c_str(), 0L);
}

void L5RP::INIFile::setValueAsString(std::string const& section, std::string const& key, std::string const& value)
{
    this->_iniFile.SetValue(section.c_str(), key.c_str(), value.c_str());
}

void L5RP::INIFile::setValueAsDouble(std::string const& section, std::string const& key, double value)
{
    this->_iniFile.SetDoubleValue(section.c_str(), key.c_str(), value);
}

void L5RP::INIFile::setValueAsBool(std::string const& section, std::string const& key, bool value)
{
    this->_iniFile.SetBoolValue(section.c_str(), key.c_str(), value);
}

void L5RP::INIFile::setValueAsLong(std::string const& section, std::string const& key, long value)
{
    this->_iniFile.SetLongValue(section.c_str(), key.c_str(), value);
}

void
L5RP::INIFile::setDefaultValueAsString(std::string const& section, std::string const& key, std::string const& value)
{

    if (!this->doesValueExist(section, key))
        this->setValueAsString(section, key, value);

}

void L5RP::INIFile::setDefaultValueAsDouble(std::string const& section, std::string const& key, double value)
{

    if (!this->doesValueExist(section, key))
        this->setValueAsDouble(section, key, value);

}

void L5RP::INIFile::setDefaultValueAsBool(std::string const& section, std::string const& key, bool value)
{

    if (!this->doesValueExist(section, key))
        this->setValueAsBool(section, key, value);

}

void L5RP::INIFile::setDefaultValueAsLong(std::string const& section, std::string const& key, long value)
{

    if (!this->doesValueExist(section, key))
        this->setValueAsLong(section, key, value);

}

std::string const& L5RP::INIFile::getPath() const
{
    return this->_path;
}

L5RP::Config::Config()
        : INIFile("config.ini")
{

}