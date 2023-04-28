#include "Controller.h"

Controller::Controller()
{
}

Controller::~Controller()
{
}

bool Controller::ChaeckInputKey(KeyID id)
{
    return controllerData_[id][static_cast<int>(ControllerAge::Now)] &&
        !controllerData_[id][static_cast<int>(ControllerAge::Old)];
}

bool Controller::ChaeckLongInputKey(KeyID id)
{
    return controllerData_[id][static_cast<int>(ControllerAge::Now)];
}
