#include "bullet/BulletWorld.h"
#include <iostream>

BulletWorld::BulletWorld(const glm::vec3& gravity) 
    : m_dynamicsWorld(nullptr)
    , m_dispatcher(nullptr)
    , m_broadphase(nullptr)
    , m_solver(nullptr)
    , m_collisionConfig(nullptr)
    , m_debugDrawEnabled(false)
{
    InitializeBulletComponents();
    SetGravity(gravity);
}

BulletWorld::~BulletWorld() {
    CleanupBulletComponents();
}

void BulletWorld::InitializeBulletComponents() {
    // Create collision configuration
    m_collisionConfig = new btDefaultCollisionConfiguration();
    
    // Create collision dispatcher
    m_dispatcher = new btCollisionDispatcher(m_collisionConfig);
    
    // Create broadphase (spatial partitioning)
    m_broadphase = new btDbvtBroadphase();
    
    // Create constraint solver
    m_solver = new btSequentialImpulseConstraintSolver();
    
    // Create dynamics world
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfig);
    
    // Set default parameters
    m_dynamicsWorld->setGravity(btVector3(0, -9.81, 0));
    m_dynamicsWorld->getSolverInfo().m_numIterations = 10;
    m_dynamicsWorld->getSolverInfo().m_solverMode = SOLVER_SIMD;
}

void BulletWorld::CleanupBulletComponents() {
    if (m_dynamicsWorld) {
        delete m_dynamicsWorld;
        m_dynamicsWorld = nullptr;
    }
    
    if (m_solver) {
        delete m_solver;
        m_solver = nullptr;
    }
    
    if (m_broadphase) {
        delete m_broadphase;
        m_broadphase = nullptr;
    }
    
    if (m_dispatcher) {
        delete m_dispatcher;
        m_dispatcher = nullptr;
    }
    
    if (m_collisionConfig) {
        delete m_collisionConfig;
        m_collisionConfig = nullptr;
    }
}

void BulletWorld::Update(float deltaTime, int maxSubSteps, float fixedTimeStep) {
    if (!m_dynamicsWorld) {
        std::cerr << "BulletWorld::Update: Dynamics world not initialized!" << std::endl;
        return;
    }
    
    // Step the simulation
    m_dynamicsWorld->stepSimulation(deltaTime, maxSubSteps, fixedTimeStep);
    
    // Handle collision callbacks
    HandleCollisions();
}

void BulletWorld::AddRigidBody(btRigidBody* body) {
    if (!m_dynamicsWorld || !body) {
        std::cerr << "BulletWorld::AddRigidBody: Invalid parameters!" << std::endl;
        return;
    }
    
    m_dynamicsWorld->addRigidBody(body);
}

void BulletWorld::RemoveRigidBody(btRigidBody* body) {
    if (!m_dynamicsWorld || !body) {
        std::cerr << "BulletWorld::RemoveRigidBody: Invalid parameters!" << std::endl;
        return;
    }
    
    m_dynamicsWorld->removeRigidBody(body);
}

void BulletWorld::SetGravity(const glm::vec3& gravity) {
    if (!m_dynamicsWorld) {
        std::cerr << "BulletWorld::SetGravity: Dynamics world not initialized!" << std::endl;
        return;
    }
    
    m_dynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

glm::vec3 BulletWorld::GetGravity() const {
    if (!m_dynamicsWorld) {
        return glm::vec3(0.0f);
    }
    
    btVector3 gravity = m_dynamicsWorld->getGravity();
    return glm::vec3(gravity.x(), gravity.y(), gravity.z());
}

void BulletWorld::SetCollisionCallback(std::function<void(btRigidBody*, btRigidBody*)> callback) {
    m_collisionCallback = callback;
}

void BulletWorld::SetDebugDrawEnabled(bool enabled) {
    m_debugDrawEnabled = enabled;
}

bool BulletWorld::IsDebugDrawEnabled() const {
    return m_debugDrawEnabled;
}

void BulletWorld::SetNumTasks(int numThreads) {
    if (!m_dynamicsWorld) {
        std::cerr << "BulletWorld::SetNumTasks: Dynamics world not initialized!" << std::endl;
        return;
    }
    
    m_dynamicsWorld->setNumTasks(numThreads);
}

void BulletWorld::HandleCollisions() {
    if (!m_collisionCallback || !m_dynamicsWorld) {
        return;
    }
    
    // Get number of manifolds (collision pairs)
    int numManifolds = m_dispatcher->getNumManifolds();
    
    for (int i = 0; i < numManifolds; i++) {
        btPersistentManifold* contactManifold = m_dispatcher->getManifoldByIndexInternal(i);
        
        // Get the two rigid bodies involved in the collision
        const btCollisionObject* objA = contactManifold->getBody0();
        const btCollisionObject* objB = contactManifold->getBody1();
        
        btRigidBody* bodyA = btRigidBody::upcast(const_cast<btCollisionObject*>(objA));
        btRigidBody* bodyB = btRigidBody::upcast(const_cast<btCollisionObject*>(objB));
        
        if (bodyA && bodyB) {
            // Check if there are contact points
            int numContacts = contactManifold->getNumContacts();
            if (numContacts > 0) {
                // Call the collision callback
                m_collisionCallback(bodyA, bodyB);
            }
        }
    }
}
