#include <iostream>

using namespace std;

#include "KalmanFilter.h"


int main() {
  KalmanFilterConstantValue data;

  for (int i = 0; i < 11; i++) {
    data.updateState(i);
    printf("%.4lf  \n", data.estimatedValue);
  }

  for (int i = 0; i < 11; i++) {
    data.updateState(10);
    printf("%.4lf  \n", data.estimatedValue);
  }

  return 0;
}
