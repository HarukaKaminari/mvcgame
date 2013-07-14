#ifndef mvcgame_ViewController_hpp
#define mvcgame_ViewController_hpp

#include <mvcgame/action/ActionRunner.hpp>
#include <mvcgame/controller/BaseViewController.hpp>

namespace mvcgame {

    class BaseView;
    class Application;
    class RootViewController;

    class ViewController : public BaseViewController
    {
	public:
		typedef std::vector<std::unique_ptr<ViewController>> Children;
   	private:
        ActionRunner _actions;        
   		ViewController* _parent;
        RootViewController* _root;
   		std::shared_ptr<View> _view;
    protected:

        void moveChildren(View* view);
        BaseView* getParentView();
    public:
    	ViewController();
    	virtual ~ViewController();

        std::shared_ptr<const View> getView() const;
        std::shared_ptr<View> getView();
		virtual void setView(std::shared_ptr<View> view);

        bool hasRoot() const;
        void setRoot(RootViewController& root);
        const RootViewController& getRoot() const;
        RootViewController& getRoot();

        const Application& getApp() const;
        Application& getApp();

        bool hasParent() const;
        const ViewController& getParent() const;
        ViewController& getParent();
        virtual void setParent(ViewController& parent);
        void removeFromParent();
        
        void addChild(std::unique_ptr<ViewController> child);

        void runAction(std::unique_ptr<IAction> action, View& view);
        void runAction(std::unique_ptr<IAction> action, View& view, const Duration& duration);

        void updateActions(const UpdateEvent& event);

        void clearChildren();
        void clearActions();

        virtual bool respondToTouchPoint(const Point& p, const TouchEvent& event);
        virtual void respondOnUpdate(const UpdateEvent& event);        

        /**
         * called after the controller is added to a parent controller
         */
        virtual void controllerAdded();     
    };

}

#endif