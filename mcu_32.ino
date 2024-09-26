#include <stdio.h>
#include <time.h>
#include <stdint.h>
#define N (624)
#define M (397)
#define K (0x9908B0DFU)
#define HighestBit(u) ((u) &  0x80000000U)
#define LowestBit(u) ((u) &  0x00000001U)
#define LowestBits(u) ((u) &  0x7FFFFFFFU)
#define MixBits(u, v) (HighestBit(u)|LowestBits(v))
static uint32_t state[N +  1];
static uint32_t *nextRand;
static int left = -1;

uint32_t startTime;
uint32_t elapsedTime;

void Seed(uint32_t seed) {
   register uint32_t x = (seed |  1U) &  0xFFFFFFFFU;
   register uint32_t *s = state;
   register int j;
   for (left =  0, *s++ = x, j = N; --j; *s++ = (x *=  69069U) &  0xFFFFFFFFU);
}

void Reload() {
   register uint32_t *p0 = state;
   register uint32_t *p2 = state +  2;
   register uint32_t *pM = state + M;
   register uint32_t s0;
   register uint32_t s1;
   register int j;
   if (left < -1) {
      Seed(4357U);
   }
   left = N -  1;
   nextRand = state +  1;
   for (s0 = state[0], s1 = state[1], j = N - M +  1; --j; s0 = s1, s1 = *p2++) {
      *p0++ = *pM++ ^ (MixBits(s0, s1) >>  1) ^ (LowestBit(s1) ? K :  0U);
   }
   for (pM = state, j = M; --j; s0 = s1, s1 = *p2++) {
      *p0++ = *pM++ ^ (MixBits(s0, s1) >>  1) ^ (LowestBit(s1) ? K :  0U);
   }
   s1 = state[0];
   *p0 = *pM ^ (MixBits(s0, s1) >>  1) ^ (LowestBit(s1) ? K :  0U);
   s1 ^= (s1 >>  11);
   s1 ^= (s1 <<  7) &  0x9D2C5680U;
   s1 ^= (s1 <<  15) &  0xEFC60000U;
}

uint32_t Random() {
   uint32_t y;
   if (--left <  0) {
      Reload();
   }
   y = *nextRand++;
   y ^= (y >>  11);
   y ^= (y <<  7) &  0x9D2C5680U;
   y ^= (y <<  15) &  0xEFC60000U;

   return (y ^ (y >>  18));
}

uint32_t MersenneTwisterRandom(uint32_t seed) {
   Seed(seed);
   return Random();
}

void MTR_1000() {
   uint32_t initial_seed =  15858U;
   uint32_t first_seed = MersenneTwisterRandom(initial_seed);
   for (uint32_t i = 1; i <= 1e6; i++) {
      first_seed = MersenneTwisterRandom(first_seed);
      //printf("random value no. %08d is -- %u \n", i, first_seed);     uncomment this if you need to display each number to serial but this slows everything down a lot
   }
}

void setup() {
  Serial.begin(115200);
}

void loop() {
   startTime = millis();
   MTR_1000();
   elapsedTime = millis() - startTime;
   Serial.print("time elapsed for calculating 1 Million 32-bit random numbers ");
   Serial.print(elapsedTime);
   Serial.println(" ms");
}