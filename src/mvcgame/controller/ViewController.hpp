#ifndef mvcgame_ViewController_hpp
#define mvcgame_ViewController_hpp

#include <mvcgame/action/ActionRunner.hpp>
#include <mvcgame/event/IResponder.hpp>

namespace mvcgame {

    class ViewController : public IResponder
    {
	public:
		typedef std::vector<std::unique_ptr<ViewController>> Children;
   	private:
        Children _children;
        ActionRunner _actions;        
   		ViewController* _parent;
   		View* _view;
    protected:

        void moveChildren(View& view);
        /**
         * called after the controller is added to a parent controller
         */
        virtual void controllerAdded();
    public:
    	ViewController();
    	virtual ~ViewController();

        const View& getView() const;
        View& getView();
		virtual void setView(std::unique_ptr<View> view);

        const ViewController& getParent() const;
        ViewController& getParent();
        virtual void setParent(ViewController& parent);
        void removeFromParent();

        void addChild(std::unique_ptr<ViewController> child);
        std::unique_ptr<ViewController> removeChild(const ViewController& child);
		Children::iterator findChild(const ViewController& child);

        const Children& getChildren() const;

        void runAction(std::unique_ptr<IAction> action, const Duration& duration);
        void updateActions(UpdateEvent& event);

        void clearChildren();
        void clearActions();

        virtual bool respondToTouchPoint(const Point& p, const TouchEvent& event);        
    };

}

#endif