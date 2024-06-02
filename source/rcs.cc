#include "../include/rcs.h"

std::map<std::string, sf::Texture> Util::Manager::resources;

Util::Utility::Utility() : filename{"../resourses/resource.txt"},
                           fs{filename},
                           v{std::istream_iterator<std::string>(fs), std::istream_iterator<std::string>()}
{
}

void Util::Utility::open()
{
    for (auto i : v)
    {
        std::cout << i << std::endl;
    }
}

void Util::Utility::close()
{
    fs.close();
    std::ofstream ofs{filename};
    copy(v.begin(), v.end(), std::ostream_iterator<std::string>(ofs, "\n"));
    ofs.close();
}

void Util::Randomizer::update_seed()
{
    int new_seed{stoi(v[1])};
    v[1] = std::to_string(++new_seed);
}
int Util::Randomizer::get_random(int from, int to)
{
    unsigned int seed{static_cast<unsigned int>(stoi(v[1]))};
    srand(seed);
    update_seed();
    int number{rand() % to};
    if (number < from)
        return number + from;
    else
        return number;
}

sf::Texture& Util::Manager::load(std::string const &file)
{
    auto it{resources.find(file)};
    if (it == end(resources))
    {
        sf::Texture res{load_from_file(file)};
        res.setRepeated(true); // Tillagd av Benim
        it = resources.emplace(make_pair(file, res)).first;
    }
    return it->second;
}


sf::Texture Util::Manager::load_from_file(std::string const &file)
{
    sf::Texture t{};
    if(!t.loadFromFile(file))
    {
        std::cerr << "Could not load texture from file: " << file << std::endl;
    }
    return t;
}
