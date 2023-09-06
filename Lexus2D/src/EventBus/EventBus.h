#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "../Logger/Logger.h"
#include "Event.h"

#include <map>
#include <typeindex>
#include <functional>
#include <list>

class IEventCallback {
private:
    virtual void execute(Event& i_event) = 0;

public:
    virtual ~IEventCallback() = default;

    void Execute(Event& i_event) {
        execute(i_event);
    }
};

template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback {
private:
    typedef void (TOwner::* CallbackFunction)(TEvent&);

    TOwner* ownerInstance;
    CallbackFunction callbackFunction;

    virtual void execute(Event& i_event) override {
        std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(i_event));
    }

public:
    EventCallback(TOwner* i_ownerInstance, CallbackFunction i_callbackFunction) {
        ownerInstance = i_ownerInstance;
        callbackFunction = i_callbackFunction;
    }

    virtual ~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {
private:
    std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

public:
    EventBus() {
        Logger::Log("EventBus constructor called!");
    }

    ~EventBus() {
        Logger::Log("EventBus destructor called!");
    }

    void Reset() {
        subscribers.clear();
    }

    template <typename TEvent, typename TOwner>
    void SubscribeToEvent(TOwner* i_ownerInstance, void (TOwner::* i_callbackFunction)(TEvent&)) {
        if (!subscribers[typeid(TEvent)].get()) {
            subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
        }
        auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(i_ownerInstance, i_callbackFunction);
        subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
    }

    template <typename TEvent, typename ...TArgs>
    void DispatchEvent(TArgs&& ...args) {
        auto handlers = subscribers[typeid(TEvent)].get();
        if (handlers) {
            for (auto it = handlers->begin(); it != handlers->end(); it++) {
                auto handler = it->get();
                TEvent event(std::forward<TArgs>(args)...);
                handler->Execute(event);
            }
        }
    }
};

#endif
