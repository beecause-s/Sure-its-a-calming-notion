#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

// Define the module setup inside a class for easier toggling
class FrameExtrapolation : public CCNode {
public:
    static bool isEnabled() {
        return Mod::get()->getSettingValue<bool>("enabled");
    }
};

class $modify(ExtrapolatedLayer, GJBaseGameLayer) {
    struct Fields {
        float m_accumulator = 0.0f;
        float m_physicsDelta = 0.0f;
        bool m_didTick = false;
    };

    float getModifiedDelta(float dt) {
        float pRet = GJBaseGameLayer::getModifiedDelta(dt);
        // pRet > 0 means the game actually processed a physics step
        if (pRet > 0) {
            m_fields->m_physicsDelta = pRet;
            m_fields->m_didTick = true;
        }
        return pRet;
    }

    void update(float dt) {
        GJBaseGameLayer::update(dt);

        if (!FrameExtrapolation::isEnabled()) return;
        if (!m_player1) return;

        // Reset timer when physics updates, otherwise track frame time since last tick
        if (m_fields->m_didTick) {
            m_fields->m_accumulator = 0.0f;
            m_fields->m_didTick = false;
        } else {
            m_fields->m_accumulator += dt;
        }

        // Avoid division by zero and clamp to 1.0
        if (m_fields->m_physicsDelta <= 0) return;
        float alpha = std::min(m_fields->m_accumulator / m_fields->m_physicsDelta, 1.0f);

        // Perform the visual slide
        auto applyExtrapolation = [&](PlayerObject* p) {
            if (!p) return;
            
            // On Android 2.208, velocity is typically in the m_velocity CCPoint
            float offX = p->m_velocity.x * m_fields->m_physicsDelta * alpha;
            float offY = p->m_velocity.y * m_fields->m_physicsDelta * alpha;

            p->setPosition(p->getPositionX() + offX, p->getPositionY() + offY);
        };

        applyExtrapolation(m_player1);
        if (m_player2) applyExtrapolation(m_player2);
    }
};
