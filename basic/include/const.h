#pragma once
#include <cstdlib>
#include <cassert>
const double EPS = 1e-5;
const double PI = 3.1415926535898;
const double INF = 1e15;
// Halton sequence with reverse permutation
const int primes[61]={
  2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,
  83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,
  191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283
};

inline int rev(const int i,const int p) {
  if (i==0) return i; else return p-i;
}
inline double hal(const int b, long long j) {
  const int p = primes[b]; 
  double h = 0.0, f = 1.0 / (double)p, fct = f;
  while (j > 0) {
    h += rev(j % p, p) * fct; j /= p; fct *= f;
  }
  return h;
}
inline double ran(){return rand() * 1.0 / RAND_MAX;}