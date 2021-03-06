
#include <mvcgame/font/FontAtlasPage.hpp>

namespace mvcgame {

    FontAtlasPage::FontAtlasPage()
    {
    }

    std::shared_ptr<const Texture> FontAtlasPage::getTexture() const
    {
        return _texture;
    }

    void FontAtlasPage::setTexture(std::shared_ptr<const Texture> texture)
    {
        _texture = texture;
    }

    void FontAtlasPage::addRegion(const Region& region)
    {
        _regions.push_back(region);
    }

    FontAtlasPage::RegionList& FontAtlasPage::getRegions()
    {
        return _regions;
    }

    const FontAtlasPage::RegionList& FontAtlasPage::getRegions() const
    {
        return _regions;
    }

    FontAtlasPage::RegionList FontAtlasPage::getRegions(const std::string& name) const
    {
        RegionList regions;
        for(const Region& region : getRegions())
        {
            if(region.name == name)
            {
                regions.push_back(region);
            }
        }
        return regions;
    }

    std::ostream& operator<<(std::ostream& os, const FontAtlasPage& t)
    {
        os << "FontAtlasPage( " << std::endl;
        for(const FontAtlasPage::Region& region : t.getRegions())
        {
            os << region << std::endl;
        }

        os << ")";
        return os;
    }

}
