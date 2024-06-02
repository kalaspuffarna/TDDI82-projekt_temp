#ifndef RCS_H
#define RCS_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <string>
#include <iterator>
#include <map>
#include <iostream>
#include <string>

namespace Util
{
    class Utility
    {
    public:
        Utility();
        void open();
        void close();

    protected:
        std::string filename;
        std::fstream fs;
        std::vector<std::string> v;
    };
    class Randomizer : public Utility
    {
    public:
        void update_seed();
        int get_random(int, int);
    };

    class Manager
    {
    public:
        static sf::Texture& load(std::string const &file);
        static sf::Texture load_from_file(std::string const &file);
    private:
        static std::map<std::string, sf::Texture> resources;
    }; 
    class Saver
    {
    };
    class Loader
    {
    };
}

#endif