#define FRACTION (1<<14)

int int_sub_float(int i, int f) {
  return i*FRACTION - f;
}

int int_mul_float(int i, int f) {
  return i*f;
}

int float_add_int(int f, int i) {
  return f + i*FRACTION;
}

int float_mul_float(int f, int i) {
  // 오버플로우가 발생할 수 있으니 64bit 데이터를 중간 계산 과정에 사용
  int64_t temp = f;
  temp = temp * i / FRACTION;
  return (int)temp;
}

int float_div_float(int f1, int f2) {
  int64_t temp = f1;
  temp = temp * FRACTION / f2;
  return (int)temp;
}

int float_add_float(int f1, int f2) {
  return f1 + f2;
}

int float_sub_float(int f1, int f2) {
  return f1 - f2;
}

int float_div_int(int f, int i) {
  return f / i;
}