#include <memory>
#include <set>

#include "gameutils/entity.h"

#include "gtest/gtest.h"

using std::make_shared;
using std::set;

using gameutils::Component;
using gameutils::EntityId;
using gameutils::EntityManager;
using gameutils::InvalidEntity;
using gameutils::getComponentAs;

class TestEntity : public testing::Test
{

};

TEST_F(TestEntity, createEntity)
{
    set<EntityId> usedIDs;

    EntityManager em;

    for (int i = 0; i < 10; ++i) {
        EntityId id = em.createEntity();
        EXPECT_NE(InvalidEntity, id);
        EXPECT_EQ(usedIDs.end(), usedIDs.find(id));
        EXPECT_TRUE(usedIDs.insert(id).second);
    }
}

TEST_F(TestEntity, destroyEntity)
{
    struct TestDestructionComponent: public Component
    {
        TestDestructionComponent(bool *pComponentDestroyed)
        : pComponentDestroyed(pComponentDestroyed)
        {
            *pComponentDestroyed = false;
        }

        ~TestDestructionComponent()
        {
            *pComponentDestroyed = true;
        }

        bool *pComponentDestroyed;
    };

    bool componentDestroyed = false;

    EntityManager em;
    EntityId id = em.createEntity();
    auto pComponent = make_shared<TestDestructionComponent>(&componentDestroyed);
    EXPECT_TRUE(em.attachComponent(id, pComponent));

    // Make sure that the entity can only be destroyed once
    EXPECT_TRUE(em.destroyEntity(id));
    EXPECT_FALSE(em.destroyEntity(id));

    // Make sure that the component's destructor is not called
    // until the pointer has been released
    EXPECT_FALSE(componentDestroyed);

    // Make sure that the component's destructor is called
    // once the pointer has been released
    pComponent.reset();
    EXPECT_TRUE(componentDestroyed);
}

TEST_F(TestEntity, destroyAllEntities)
{
    EntityManager em;
    EntityId id1 = em.createEntity();
    EntityId id2 = em.createEntity();

    EXPECT_TRUE(em.destroyAllEntities());

    EXPECT_FALSE(em.destroyEntity(id1));
    EXPECT_FALSE(em.destroyEntity(id2));
}

TEST_F(TestEntity, markForRemoval_and_purge)
{
    set<EntityId> usedIDs;

    EntityManager em;

    for (int i = 0; i < 10; ++i) {
        EntityId id = em.createEntity();
        EXPECT_NE(InvalidEntity, id);
        EXPECT_EQ(usedIDs.end(), usedIDs.find(id));
        EXPECT_TRUE(usedIDs.insert(id).second);
    }

    for (auto id: usedIDs) {
        em.markForRemoval(id);
    }

    em.purge();

    // Make sure that the entities have been destroyed.
    for (auto id: usedIDs) {
        // If destroyEntity fails, then we can assume that the
        // entity has already been destroyed.
        EXPECT_FALSE(em.destroyEntity(id));
    }
}

struct AnonymousComponent1: public Component
{

};

struct AnonymousComponent2: public Component
{

};

TEST_F(TestEntity, attachComponent_and_getComponent)
{
    EntityManager em;
    EntityId id = em.createEntity();
    EXPECT_TRUE(em.attachComponent(id, make_shared<AnonymousComponent1>()));

    // Make sure that the Component has been attached.
    auto ptr = em.getComponent<AnonymousComponent1>(id);
    EXPECT_NE(nullptr, ptr);

    // Make sure that it is not possible to attach a second
    // Component of the same type
    EXPECT_FALSE(em.attachComponent(id, make_shared<AnonymousComponent1>()));
}

TEST_F(TestEntity, getComponentAs)
{
    EntityManager em;
    EntityId id = em.createEntity();
    EXPECT_TRUE(em.attachComponent(id, make_shared<AnonymousComponent1>()));

    auto entityNodes = em.getEntityNodes<AnonymousComponent1>();
    EXPECT_NE(nullptr, entityNodes);
    EXPECT_EQ(1, entityNodes->size());

    auto entityNodesItr = entityNodes->find(id);
    EXPECT_NE(entityNodes->end(), entityNodesItr);

    // Make sure that the component can be cast to its true type
    auto componentPtr = getComponentAs<AnonymousComponent1>(entityNodesItr->second);
    EXPECT_NE(nullptr, componentPtr);
}

TEST_F(TestEntity, getEntityNodes)
{
    EntityManager em;
    EntityId id1 = em.createEntity();
    EntityId id2 = em.createEntity();
    EXPECT_TRUE(em.attachComponent(id1, make_shared<AnonymousComponent1>()));
    EXPECT_TRUE(em.attachComponent(id2, make_shared<AnonymousComponent1>()));
    EXPECT_TRUE(em.attachComponent(id2, make_shared<AnonymousComponent2>()));

    // Make sure that there are two entity nodes for AnonymousComponent1
    auto entityNodesA = em.getEntityNodes<AnonymousComponent1>();
    EXPECT_NE(nullptr, entityNodesA);
    EXPECT_EQ(2, entityNodesA->size());
    EXPECT_NE(entityNodesA->end(), entityNodesA->find(id1));
    EXPECT_NE(entityNodesA->end(), entityNodesA->find(id2));

    // Make sure that there is just one entity node for AnonymousComponent2
    auto entityNodesB = em.getEntityNodes<AnonymousComponent2>();
    EXPECT_NE(nullptr, entityNodesB);
    EXPECT_EQ(1, entityNodesB->size());
    EXPECT_NE(entityNodesB->end(), entityNodesB->find(id2));
}
