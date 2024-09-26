#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <inttypes.h>
#define N (624)
#define M (397)
#define K (0x9908B0DFU)
#define HighestBit(u) ((u) &   0x8000000000000000ULL)
#define LowestBit(u) ((u) &   0x0000000000000001ULL)
#define LowestBits(u) ((u) &   0x7FFFFFFFFFFFFFFFULL)
#define MixBits(u, v) (HighestBit(u)|LowestBits(v))
static uint64_t state[N +   1];
static uint64_t *nextRand;
static int left = -1;

uint32_t startTime;
uint32_t elapsedTime;

void Seed(uint64_t seed) {
   register uint64_t x = (seed |   1ULL) &   0xFFFFFFFFFFFFFFFFULL;
   register uint64_t *s = state;
   register int j;
   for (left =   0, *s++ = x, j = N; --j; *s++ = (x *=   69069ULL) &   0xFFFFFFFFFFFFFFFFULL);
}

void Reload() {
   register uint64_t *p0 = state;
   register uint64_t *p2 = state +   2;
   register uint64_t *pM = state + M;
   register uint64_t s0;
   register uint64_t s1;
   register int j;
   if (left < -1) {
      Seed(4357ULL);
   }
   left = N -  1;
   nextRand = state +  1;
   for (s0 = state[0], s1 = state[1], j = N - M +  1; --j; s0 = s1, s1 = *p2++) {
      *p0++ = *pM++ ^ (MixBits(s0, s1) >>  1) ^ (LowestBit(s1) ? K :  0ULL);
   }
   for (pM = state, j = M; --j; s0 = s1, s1 = *p2++) {
      *p0++ = *pM++ ^ (MixBits(s0, s1) >>  1) ^ (LowestBit(s1) ? K :  0ULL);
   }
   s1 = state[0];
   *p0 = *pM ^ (MixBits(s0, s1) >>  1) ^ (LowestBit(s1) ? K :  0ULL);
   s1 ^= (s1 >>  11);
   s1 ^= (s1 <<  7) &  0x9D2C5680ULL;
   s1 ^= (s1 <<  15) &  0xEFC60000ULL;
}

uint64_t Random() {
   uint64_t y;
   if (--left <  0) {
      Reload();
   }
   y = *nextRand++;
   y ^= (y >>  11);
   y ^= (y <<  7) &  0x9D2C5680ULL;
   y ^= (y <<  15) &  0xEFC60000ULL;

   return (y ^ (y >>  18));
}

uint64_t MersenneTwisterRandom(uint64_t seed) {
   Seed(seed);
   return Random();
}

void MTR_1000() {
   uint64_t initial_seed =  28378U;
   uint64_t first_seed = MersenneTwisterRandom(initial_seed);
   for (uint32_t i = 1; i <= 1e6; i++) {
      first_seed = MersenneTwisterRandom(first_seed);
      //printf("random value no. %08d is -- %" PRIu64 "\n", i, first_seed);      uncomment this if you need to display each number to serial but this slows everything down a lot
   }
}

void setup() {
  Serial.begin(115200);
}

void loop() {
   startTime = millis();
   MTR_1000();
   elapsedTime = millis() - startTime;
   Serial.print("time elapsed for calculating 1k 64-bit random numbers ");
   Serial.print(elapsedTime);
   Serial.println(" ms");
}