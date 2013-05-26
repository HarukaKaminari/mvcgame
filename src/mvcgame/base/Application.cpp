
#include <mvcgame/base/Application.hpp>
#include <mvcgame/platform/ApplicationBridge.hpp>
#include <mvcgame/view/RootView.hpp>

namespace mvcgame {

    Application::Application() :
    _bridge(new ApplicationBridge()),
    _root(*this), _assets(*this)
    {
        _bridge->setApplication(*this);
    }

    RootViewController& Application::getRoot()
    {
        return _root;
    }

    const RootViewController& Application::getRoot() const
    {
        return _root;
    }

    IApplicationBridge& Application::getBridge()
    {
        return *_bridge;
    }
    
    const IApplicationBridge& Application::getBridge() const
    {
        return *_bridge;
    }

    AssetsManager& Application::getAssetsManager()
    {
        return _assets;
    }

    const AssetsManager& Application::getAssetsManager() const
    {
        return _assets;
    }

    const Size& Application::getSize() const
    {
        return _root.getView().getSize();
    }

    void Application::run()
    {   
        _bridge->run();
    }

    void Application::update()
    {
        _root.emitUpdate();
    }
}