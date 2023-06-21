#ifndef ECS_HPP
#define ECS_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include "../Object.h"
#include "../../thirdparty/glm/glm.hpp"
#include "string.h"
#include "../Signals/Signal.hpp"

class Component;
class Entity;

enum ComponentType
{
    CT_GRAPHICS,
    CT_TRANSFORM,
    CT_CUSTOM,
    CT_SPHERE_COLLIDER,
    CT_AABB_COLLIDER,
    CT_CAPSULE_COLLIDER,
    CT_MESH_COLLIDER,
    CT_BOX_COLLIDER,
    CT_RIGIDBODY,
    CT_STATE_MANAGER,
    CT_COLLIDER_SET,
    CT_PARTICLE_SYSTEM
};

using ComponentID = std::size_t;

inline ComponentID getComponentTypeID()
{
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getComponentTypeID();
    return typeID;
}

class Component
{
public:
    Entity *entity;
    ComponentType componentType;
    bool enable;

    virtual void init() {}
    virtual void run(float dt) {}

    Component(ComponentType _componentType) : componentType(_componentType) {
        enable = true;
    }

    virtual ~Component() {}
};

constexpr std::size_t maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component *, maxComponents>;

class Entity : public Object
{
private:
    bool active = true;

    std::vector<std::unique_ptr<Component>> components;

    ComponentArray componentArray;
    ComponentBitSet componentBitSet;

public:
    std::string tag;
    Signal <Entity*> onCollisionEnter;
    Signal <Entity*> onTriggerEnter;

    Entity() {
        tag = "";
    }

    void Update(float dt)
    { 
        for (auto &c : components)
            if( c->enable )  c->run(dt);

        UserUpdate(dt);
    }

    void UserUpdate(float dt) {}

    bool isActive() const { return active; }
    void destroy() { active = false; }

    template <typename T>
    bool hasComponent() const
    {
        return componentBitSet[getComponentTypeID<T>()];
    }

    template <typename T, typename... TArgs>
    T &addComponent(TArgs &&...mArgs)
    {
        T *c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this;

        std::unique_ptr<Component> uPtr{c};
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentTypeID<T>()] = c;
        componentBitSet[getComponentTypeID<T>()] = true;

        c->init();
        return *c;
    }

    template <typename T>
    T &getComponent() const
    {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T *>(ptr);
    }

    virtual ~Entity()
    {
        components.clear();
        onCollisionEnter.disconnect_all();
        onTriggerEnter.disconnect_all();
    }
};

class Manager
{
private:
    std::vector<std::unique_ptr<Entity>> entities;

public:
    void Update(float dt)
    {
        for (auto &e : entities)
            e->Update(dt);
        refresh();
    }

    void refresh()
    {
        entities.erase(std::remove_if(std::begin(entities), std::end(entities),
                                      [](const std::unique_ptr<Entity> &mEntity)
                                      {
                                          return !mEntity->isActive();
                                      }),
                       std::end(entities));
    }

    Entity &addEntity()
    {
        Entity *e = new Entity();
        std::unique_ptr<Entity> uPtr{e};
        entities.emplace_back(std::move(uPtr));
        return *e;
    }

    void addEntity(Entity *e)
    {
        std::unique_ptr<Entity> uPtr{e};
        entities.emplace_back(std::move(uPtr));
    }

    void removeAll()
    {
        for (auto &e : entities)
            e->destroy();
        refresh();
    }
};
#endif