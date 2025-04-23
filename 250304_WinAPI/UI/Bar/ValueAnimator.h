#pragma once

class ValueAnimator {
private:
    float currentValue = 0.0f;
    float goalValue = 0.0f;
    float velocity = 0.0f;
    float smoothTime = 0.3f; // seconds

public:
    void SetGoal(float goal) { goalValue = goal; }
    void SetInstant(float value) { currentValue = goalValue = value; velocity = 0.0f; }
    float Update(float deltaTime);
    float GetValue() const { return currentValue; }
    float GetTarget() const { return goalValue; }
};

inline float ValueAnimator::Update(float deltaTime) {
    float omega = 2.0f / smoothTime;
    float x = omega * deltaTime;
    float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
    float change = currentValue - goalValue;
    float temp = (velocity + omega * change) * deltaTime;
    velocity = (velocity - omega * temp) * exp;
    float output = goalValue + (change + temp) * exp;

    if ((goalValue - currentValue > 0.0f) == (output > goalValue)) {
        output = goalValue;
        velocity = 0.0f;
    }
    currentValue = output;
    return currentValue;
}