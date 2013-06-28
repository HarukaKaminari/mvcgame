
#include <mvcgame/view/TileMapView.hpp>
#include <mvcgame/view/Sprite.hpp>

namespace mvcgame {

    TileMapView::TileMapView() :
    _changed(false),
    _tileLayer(nullptr)
    {
    }

    TileMapView::TileMapView(std::shared_ptr<TileMap> tileMap, bool changeSize) :
    _changed(true),
    _tileLayer(nullptr)
    {
        setTileMap(tileMap, changeSize);
    }

    std::shared_ptr<TileMap> TileMapView::getTileMap()
    {
        return _tileMap;
    }

    void TileMapView::setTileLayer(unsigned int num, bool changeSize)
    {
        _tileLayer = &_tileMap->getLayers().at(num);
        _changed = true;
    }

    void TileMapView::setTileLayer(const std::string& name, bool changeSize)
    {
        _tileLayer = &_tileMap->getLayer(name);
        _changed = true;        
    }

    void TileMapView::setTileMap(std::shared_ptr<TileMap> tileMap, bool changeSize)
    {
        if(_tileMap != tileMap)
        {
            _tileMap = tileMap;
            if(!_tileLayer)
            {
                setTileLayer(0, changeSize);
            }
            _changed = true;
        }
    }

    void TileMapView::update()
    {
        View::update();
        if(!_changed)
        {
            return;
        }
        removeChildren();
        if(!_tileLayer)
        {
            return;
        }

        unsigned th = _tileLayer->getHeight();
        unsigned tw = _tileLayer->getWidth();
        Size mapSize = Size(_tileMap->getTileWidth()*tw, _tileMap->getTileHeight()*th);
        Scale sc = getFrame().size / mapSize;
        Point p(0, mapSize.height);
        for(unsigned ty=0; ty<th; ty++)
        {   
            for(unsigned tx=0; tx<tw; tx++)
            {
                auto& tile = _tileLayer->getTile(tx, ty);
                auto sheet = _tileMap->getSheetForTypeId(tile.getTypeId());
                auto sprite = std::make_shared<Sprite>(sheet);
                sprite->setAnchor(Anchor(0,1));  
                sprite->getFrame().origin = p*sc;
                sprite->setScale(sc);
                if(tx < tw-1)
                {
                    p.x += sprite->getFrame().size.width;
                }
                else
                {
                    p.x = 0;
                    p.y -= sprite->getFrame().size.height;
                }
                addChild(sprite);
            }
        }
        _changed = false;
    }

    void TileMapView::draw()
    {
        View::draw();
    }
}