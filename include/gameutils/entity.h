#pragma once

#include <limits>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

/**
 * The EntitySystem namespace contains an implementation of an entity
 * management system. This is an early stage (simplistic) implementation that
 * does not make any particular effort to optimise memory layout, management
 * overhead, etc.
 *
 *
 * Entities
 * --------
 * An entity is a light-weight object, represented by an integer ID, whose
 * behaviour is defined by one or more components that are 'attached' to it
 * at runtime.
 *
 * An entity is created at runtime, by an instance of EntityManager. Entity
 * IDs are assigned automatically, since the IDs themselves are not intended
 * to possess any particular significance:
 *
 *     using namespace EntitySystem;
 *     EntityManager em;
 *     EntityId id = em.createEntity();
 *
 * Entities can be destroyed immediately using their ID:
 *
 *     em.destroyEntity(id);
 *
 *
 * Components
 * ----------
 * A component is a set of attributes that can be attached to an entity at
 * runtime. All components must subclass the Component struct:
 *
 *     struct PlayerComponent: public EntitySystem::Component {
 *         PlayerComponent() : health(100), lives(3) {}
 *         float health;
 *         int lives;
 *     };
 *
 * Subclassing the Component struct is mandatory, since it ensures that the
 * necessary type information is available at runtime. Ideally, the only
 * functions that should defined by subclasses of Component are constructors
 * and destructors.
 *
 * Components are created and attached to an entity at runtime. Components
 * must be provided to the attachComponent function via a shared_ptr:
 *
 *     using namespace EntitySystem;
 *     EntityManager em;
 *     EntityId id = em.createEntity();
 *     em.attachComponent(id, std::make_shared<PlayerComponent>());
 *
 * If an entity is known to have a particular Component, it can be accessed
 * using the getComponent template function:
 *
 *     std::shared_ptr<PlayerComponent> pPlayer =
 *         em.getComponent<PlayerComponent>(id);
 *
 *
 * Entity Nodes
 * ------------
 * It is easy to retrieve the set of entities that possess a particular
 * component:
 *
 *     std::shared_ptr<EntityNodes> nodes =
 *         em.getEntityNodes<PlayerComponent>();
 *
 * The EntityNodes type is a map of entity IDs to the components of the type
 * requested. You can iterate over the contents of this map, but in order to
 * access the Component, it will be need to be cast to its actual type:
 *
 *     for (EntityNodes::iterator itr = *nodes) {
 *         EntityId id = itr.first;
 *         std::shared_ptr<PlayerComponent> pPlayer =
 *             getComponentAs<PlayerComponent>(itr.second);
 *     }
 *
 * While iterating over an EntityNodes map, entities should not be destroyed,
 * since this would invalidate the map iterator. Instead, entities can be
 * marked for removal:
 *
 *     em.markForRemoval(id);
 *
 * Once iteration is complete, entities that have been marked for removal
 * can be destroyed all at once:
 *
 *     em.purge();
 *
 */
namespace gameutils {

struct Component
{
    virtual ~Component() = default;
};

typedef uint32_t EntityId;
typedef std::map< EntityId, std::shared_ptr<Component> > EntityNodes;

static const EntityId InvalidEntity = 0;

class EntityManager
{
public:
    EntityManager()
      : m_nextEntityId(std::numeric_limits<EntityId>::max()) { }

    /**
     *  Create an entity
     */
    EntityId createEntity()
    {
        const EntityId maxEntityId = std::numeric_limits<EntityId>::max();

        if (m_entities.size() == maxEntityId) {
            // Somehow, we have more than 4 billion entities...
            return InvalidEntity;
        }

        EntityId entityId = m_nextEntityId;

        // Find next available entity ID
        while (m_entities.find(entityId) != m_entities.end()) {
            if (entityId == 1) {
                entityId = maxEntityId;
            } else {
                entityId--;
            }
        }

        // Add entity to entity map
        auto result = m_entities.insert(Entities::value_type(entityId, ComponentNodes()));
        if (!result.second) {
            return InvalidEntity;
        }

        // Calculate starting entity ID for next time
        if (entityId == 1) {
            m_nextEntityId = maxEntityId;
        } else {
            m_nextEntityId = entityId - 1;
        }

        return entityId;
    }

    /**
     *  Destroy an entity with the specified name.
     */
    bool destroyEntity(EntityId entityId)
    {
        // Find the entity
        auto enIter = m_entities.find(entityId);
        if (enIter == m_entities.end()) {
            // Entity does not exist
            return false;
        }

        // For each component type attached to this entity
        auto &componentNodes = enIter->second;
        for (auto cmNode: componentNodes) {

            // Remove the associated entity nodes
            const auto &cmType = cmNode.first;
            auto cmIter = m_componentTypes.find(cmType);
            if (cmIter == m_componentTypes.end()) {
                throw std::runtime_error("Could not find expected component type in EM.");
            }

            auto &enNodes = cmIter->second;
            auto enNodeIter = enNodes->find(entityId);
            if (enNodeIter == enNodes->end()) {
                throw std::runtime_error("Could not find expected entity node in EM.");
            }

            // Remove the entity node (as well as the embedded shared_ptr to the component)
            enNodes->erase(enNodeIter);
        }

        // Finally, erase the entity and its component nodes
        m_entities.erase(enIter);

        return true;
    }

    /**
     *  Destroy all entities.
     */
    bool destroyAllEntities()
    {
        // Clearing both maps will destroy everything.
        m_entities.clear();
        m_componentTypes.clear();

        return true;
    }

    /**
     *  Attach a component of type <T> to the specified entity.
     */
    bool attachComponent(EntityId entityId, std::shared_ptr<Component> pComponent)
    {
        // Find the entity
        auto enIter = m_entities.find(entityId);
        if (enIter == m_entities.end()) {
            // Entity does not exist
            return false;
        }

        // Examine type of internal component pointer
        Component *pInner = pComponent.get();
        if (!pInner) {
            return false;
        }

        // Construct type index for <T>
        const std::type_index &cmType(typeid(*pInner));

        // Find the entity's component node for this type
        auto &cmNodes = enIter->second;
        auto cmNodeIter = cmNodes.find(cmType);
        if (cmNodeIter != cmNodes.end()) {
            // Entity already has a component of this type.
            return false;
        }

        if (!cmNodes.insert(ComponentNodes::value_type(cmType, pComponent)).second) {
            // Failed to add a component node
            return false;
        }

        // Errors beyond this point indicate that state of the EM has become
        // corrupt. This is essentially irreparable, so exceptions will be thrown.

        // Find the entity nodes associated with the component type
        auto cmIter = m_componentTypes.find(cmType);
        if (cmIter == m_componentTypes.end()) {
            // Create a new component type
            cmIter = m_componentTypes.insert(
                ComponentTypes::value_type(cmType, std::make_shared<EntityNodes>())).first;
        }

        if (cmIter == m_componentTypes.end()) {
            // Could not find or create a list of entity nodes for type <T>
            throw std::runtime_error("Could not find or create a list of entity nodes in EM.");
        }

        // Add a node for this component to the com
        auto &entityNodes = cmIter->second;
        entityNodes->insert(EntityNodes::value_type(entityId, pComponent));

        return true;
    }

    /**
     *  Detach the component of type <T> from the specified entity.
     */
    template<typename T>
    bool detachComponent(EntityId entityId)
    {
        // Find the entity
        auto enIter = m_entities.find(entityId);
        if (enIter == m_entities.end()) {
            // Entity does not exist
            return false;
        }

        // Construct type index for <T>
        const std::type_index &cmType(typeid(T));

        // Find the entity's component node for this type
        auto &cmNodes = enIter->second;
        auto cmNodeIter = cmNodes.find(cmType);
        if (cmNodeIter == cmNodes.end()) {
            // Entity does not have a component of this type
            return false;
        }

        // Remove the component node from the entity
        // Errors beyond this point indicate that state of the EM has become
        // corrupt. This is essentially irreparable, so exceptions will be thrown.
        cmNodes.erase(cmNodeIter);

        // Find the component
        auto cmIter = m_componentTypes.find(cmType);
        if (cmIter == m_componentTypes.end()) {
            // Component does not exist
            throw std::runtime_error("Missing component in EM.");
        }

        auto &enNodes = cmIter->second;
        auto enNodeIter = enNodes->find(entityId);
        if (enNodeIter == enNodes->end()) {
            // Entity node does not exist
            throw std::runtime_error("Missing entity node for component in EM.");
        }

        // Remove the entity node from the component
        enNodes->erase(enNodeIter);

        return true;
    }

    /**
     *  Get a specific component type attached to an entity
     */
    template<typename T>
    std::shared_ptr<T> getComponent(EntityId entityId)
    {
        // Find the entity
        auto enIter = m_entities.find(entityId);
        if (enIter == m_entities.end()) {
            // Entity does not exist
            return nullptr;
        }

        // Construct type index for <T>
        const std::type_index &cmType(typeid(T));

        // Find the component node for type <T>
        auto &cmNodes = enIter->second;
        auto cmNodeIter = cmNodes.find(cmType);
        if (cmNodeIter == cmNodes.end()) {
            // Entity does not have a component of this type
            return nullptr;
        }

        return std::static_pointer_cast<T>(cmNodeIter->second);
    }

    /**
     *  Returns the EntityNodes map for a given component type
     */
    template<typename T>
    std::shared_ptr<EntityNodes> getEntityNodes()
    {
        // Construct type index for <T>
        const std::type_index &cmType(typeid(T));

        // Check for existing component type entry
        auto cmIter = m_componentTypes.find(typeid(T));
        if (cmIter == m_componentTypes.end()) {
            // Create a new component type entry
            auto result = m_componentTypes.insert(
                ComponentTypes::value_type(cmType, std::make_shared<EntityNodes>()));
            if (!result.second) {
                return nullptr;
            }

            return result.first->second;
        }

        return cmIter->second;
    }

    void markForRemoval(EntityId entityId)
    {
        auto emItr = m_entities.find(entityId);
        if (emItr != m_entities.end()) {
            m_entitiesMarkedForRemoval.push_back(entityId);
        }
    }

    void purge()
    {
        for (auto entityId: m_entitiesMarkedForRemoval) {
            destroyEntity(entityId);
        }
    }

private:
    typedef std::unordered_map<std::type_index, std::shared_ptr<Component>> ComponentNodes;

    typedef std::unordered_map<EntityId, ComponentNodes> Entities;
    Entities m_entities;

    typedef std::vector<EntityId> EntitiesMarkedForRemoval;
    EntitiesMarkedForRemoval m_entitiesMarkedForRemoval;

    typedef std::unordered_map<std::type_index, std::shared_ptr<EntityNodes>> ComponentTypes;
    ComponentTypes m_componentTypes;

    EntityId m_nextEntityId;
};

template<typename T>
static std::shared_ptr<T> getComponentAs(std::shared_ptr<Component> pComponent)
{
    Component *pInner = pComponent.get();
    if (!pInner) {
        throw std::runtime_error("Attempted to cast component from null pointer.");
    }

    if (typeid(*pInner) == typeid(T)) {
        return std::static_pointer_cast<T>(pComponent);
    }

    throw std::runtime_error("Attempted to cast component to incompatible type.");
}

}   // end namespace gameutils
