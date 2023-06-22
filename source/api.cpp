#include "api.h"

RAGE_API rage::IPlugin* InitializePlugin(rage::IMultiplayer* mp)
{
    return new L5RP::Plugin(mp);
}

std::string UTF16_TO_UTF8(std::u16string const & text)
{

    std::string str = "";
	char cstr[3] = "\0";
	mbstate_t mbs;
	for (const auto& it : text) {
		memset(&mbs, 0, sizeof(mbs));
		memmove(cstr, "\0\0\0", 3);
		c16rtomb(cstr, it, &mbs);
		str.append(std::string(cstr));
	}
	return str;

}

std::u16string UTF8_TO_UTF16(std::string const & text)
{

    std::u16string wstr = u"";
	char16_t c16str[3] = u"\0";
	mbstate_t mbs;
	for (const auto& it : text) {
		memset(&mbs, 0, sizeof(mbs));
		memmove(c16str, u"\0\0\0", 3);
		mbrtoc16(c16str, &it, 3, &mbs);
		wstr.append(std::u16string(c16str));
	}
	return wstr;

}

uint32_t HASH_TO_GAME(std::string const& string)
{

    std::string _hash = string;
    std::transform(_hash.begin(), _hash.end(), _hash.begin(), ::tolower);

    size_t i = 0;
    uint32_t hash = 0;
    while (i != _hash.length()) {
        hash += _hash[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;

}

bool STRING_CONTAINS_IGNORE_CASE(std::string const& strHaystack, std::string const& strNeedle)
{

    auto it = std::search(
            strHaystack.begin(), strHaystack.end(),
            strNeedle.begin(), strNeedle.end(),
            [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
    );

    return (it != strHaystack.end());

}