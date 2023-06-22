#pragma once

namespace L5RP {

    class INIFile {

        CSimpleIniA _iniFile;
        std::string _path;

        void _loadData();
        void _saveData();

    public:

        explicit INIFile(std::string const& path);
        ~INIFile();

        void save();

        bool doesValueExist(std::string const& section, std::string const& key);

        std::string readValueAsString(std::string const& section, std::string const& key);
        double readValueAsDouble(std::string const& section, std::string const& key);
        bool readValueAsBool(std::string const& section, std::string const& key);
        long readValueAsLong(std::string const& section, std::string const& key);

        void setValueAsString(std::string const& section, std::string const& key, std::string const& value);
        void setValueAsDouble(std::string const& section, std::string const& key, double value);
        void setValueAsBool(std::string const& section, std::string const& key, bool value);
        void setValueAsLong(std::string const& section, std::string const& key, long value);

        void setDefaultValueAsString(std::string const& section, std::string const& key, std::string const& value);
        void setDefaultValueAsDouble(std::string const& section, std::string const& key, double value);
        void setDefaultValueAsBool(std::string const& section, std::string const& key, bool value);
        void setDefaultValueAsLong(std::string const& section, std::string const& key, long value);

        std::string const& getPath() const;

    };

    class Config
            : public INIFile {

        Config();
        Config(Config const& copy);            // Not Implemented
        Config& operator=(Config const& copy); // Not Implemented

    public:

        static Config& getInstance()
        {
            static Config instance;
            return instance;
        }

    };

}