


class KalmanFilterConstantValue {
  public:    
    int measuredValues = 0;
    float estimatedValue = 0;

    void updateState(float newMeasurement) {
      measuredValues++;
      estimatedValue += (newMeasurement - estimatedValue) / measuredValues;
    }
};



class KalmanFilterChangingValue {
  public:
    float alpha = 0.2;
    float beta = 0.2;

    float estimatedValue = 0;
    float estimatedChangingValue = 0;

    float lastTime;

    void updateState(float newMeasurement, float currentTime) {
      float timeDifference = currentTime - lastTime;

      estimatedChangingValue += beta * (newMeasurement - estimatedValue) / timeDifference;

      estimatedValue += alpha * (newMeasurement - estimatedValue);
    }
};