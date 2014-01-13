#include "GPMatrix.h"
#include "GPMatrixSum.h"

#include <vector>
#include <type_traits>

int main(int, char * []) {
  const unsigned int sz=10;
  std::vector<double> x(sz,1);
  GPVector vx(x);
  GPVector vy(std::vector<double>(sz,2));
  if (vx.eigen() != x) return 1;
  vx+vy;
  const std::vector<double> tmp((vx+vy).eigen());
  GPVector vsum_manual(tmp);
  GPVector vsum(vy+vx);
  if (vsum.eigen() != std::vector<double>(sz,3)) return 2;
  if ((vx+vy).eigen() != std::vector<double>(sz,3)) return 3;

  // traits classes
  static_assert(
      std::is_same<GPVector::result_type, std::vector<double> >::value,
      "wrong result_type");
  static_assert(std::is_same<GPVector::scalar_type, double>::value,
                "wrong scalar_type");
  static_assert(
      std::is_same<decltype(vsum.eigen()), std::vector<double> >::value,
      "wrong eigen() return type");
  GPMatrixSum<GPVector,GPVector> s(vx,vy); //type is defined
  if (s.eigen() != vsum.eigen()) return 4; // works as expected
  return 0;
}
