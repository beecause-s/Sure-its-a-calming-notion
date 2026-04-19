#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

// Minimalistic fields to track the 10 TPS gap
class $modify(MyExtrapolatedLayer, GJBaseGameLayer) {
    struct Fields {
        float m_acc = 0.0f;
        float m_physDelta = 0.0f;
        bool m_ticked = false;
    };

    float getModifiedDelta(float dt) {
        float delta = GJBaseGameLayer::getModifiedDelta(dt);
        if (delta > 0) {
            m_fields->m_physDelta = delta;
            m_fields->m_ticked = true;
        }
        return delta;
    }

    void update(float dt) {
        GJBaseGameLayer::update(dt);

        if (!m_player1) return;

        // Track how far we are between the 100ms physics steps
        if (m_fields->m_ticked) {
            m_fields->m_acc = 0.0f;
            m_fields->m_ticked = false;
        } else {
            m_fields->m_acc += dt;
        }

        // Avoid division by zero
        if (m_fields->m_physDelta <= 0) return;

        float alpha = m_fields->m_acc / m_fields->m_physDelta;
        if (alpha > 1.0f) alpha = 1.0f;

        // Apply smooth slide
        auto extrapolate = [&](PlayerObject* p) {
            if (!p) return;
            float x = p->m_velocity.x * m_fields->m_physDelta * alpha;
            float y = p->m_velocity.y * m_fields->m_physDelta * alpha;
            p->setPosition(p->getPositionX() + x, p->getPositionY() + y);
        };

        extrapolate(m_player1);
        extrapolate(m_player2);
    }
};
