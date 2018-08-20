#include "core.hpp"
#include "ui.hpp"
#include "input.hpp"
#include "input-dispatcher.hpp"
#include "Player.hpp"


Player::Player(InputDispatcher& inputDispatcher) :
    m_input(inputDispatcher.createNew("Player"))
    {
        initInputContext();
    };
Player::~Player(){
}

void Player::initInputContext(){
    m_input->activate();
}

void Player::updateGraphic(float dt){
}
void Player::update(float dt){
    // mouseSampler->samplePosition = KeyState::mousePosition;

    // crosshair = m_vehicleEq.cameras[cameraId]->focusPoint;
    // if(not isLockedOnPoint) targetPointPosition = convert(crosshairSampler->position);

    // // m_vehicleEq.setTargetPoint(targetPointPosition, 0);
    // m_vehicleEq.sko->updateTarget(convert(targetPointPosition, 1));
    // m_vehicleEq.updateModules(dt);
    // m_vehicleEq.driveSystem->update(controlXValue, controlYValue, dt);
    // m_vehicleEq.compound->recalculateLocalAabb();

    // for(auto &it : m_vehicleEq.weapons) it.update(dt);

    // if(doFire) fire();
}
