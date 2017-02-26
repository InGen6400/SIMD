// SIMD_train.cpp : 13命令で底を8bit2進数から3進数へ
//10110010(2):178
//   ↓
//10110010(3):2514
#include "stdafx.h"
#include <immintrin.h>
#include <intrin.h>
#include <stdlib.h>

#define LEN 16

typedef unsigned short uint16;

//ビット配列を出力する
void printBit(uint16 *in) {
	char buf[LEN][1000];
	for (int i = LEN - 1; i >= 0; i--) {
		itoa(in[i], buf[i], 2);
		if ((i+1) % 8 == 0) {
			printf(" ");
		}
		printf("%s,", buf[i]);
	}
	printf("\n");
}

unsigned char delta_swap(unsigned char bits, unsigned char mask, int delta) {
	unsigned char x = (bits ^ (bits >> delta)) & mask;
	return bits ^ x ^ (x << delta);
}

unsigned char getReverse(unsigned char in) {
	unsigned char data;
	data = ((in & 0x55) << 1) | ((in & 0xAA) >> 1);
	data = ((data & 0x33) << 2) | ((data & 0xCC) >> 2);
	data = ((data & 0x0F) << 4) | ((data & 0xF0) >> 4);
	return data;
}

//データの整形をする  (shift 7, set, and)
void setData(__m256i *ret, const unsigned char me, const unsigned char opp) {
	
#pragma region AVX2

	__m256i mme = _mm256_set1_epi16(me);
	__m256i mopp = _mm256_set1_epi16(opp);
	const __m256i shifter = _mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0);

	*ret = _mm256_unpackhi_epi16(mme, mopp);

	*ret = _mm256_srlv_epi32(*ret, shifter);

	*ret = _mm256_and_si256(*ret, _mm256_set1_epi16(0x0001));

#pragma endregion

#pragma region AVX1
	/*
	//この部分はAVX2で高速化可能だがメインPCがAVX(1)なので断念
	*ret = _mm256_set_epi16(me >> 7, me >> 6, me >> 5, me >> 4, me >> 3, me >> 2, me >> 1, me,
		opp >> 7, opp >> 6, opp >> 5, opp >> 4, opp >> 3, opp >> 2, opp >> 1, opp);
	*ret = _mm256_and_si256(*ret, _mm256_set1_epi16(0x0001));
	*/
#pragma endregion
}

int main()
{

	alignas(16) const uint16 pow_3[LEN] = { 0x1,  0x1 * 2,  0x3,  0x3 * 2,  0x9,   0x9 * 2,   0x1b,  0x1b * 2,
											0x51, 0x51 * 2, 0xf3, 0xf3 * 2, 0x2d9, 0x2d9 * 2, 0x88b, 0x88b * 2 };//(1,3,9,27,81,243,729,2187)*2 と1,3,9,27,81,243,729,2187
	alignas(16) uint16 y[LEN] = { 0 };
	alignas(16) uint16 z[LEN] = { 0 };
	int index;//計算結果のインデックス値

	//レジストリに登録
	//__m256i *mmx = (__m256i *)pow_3;
	//__m256i *mmy = (__m256i *)y;
	//__m256i *mmz = (__m256i *)z;

	//データの整形
	//setData(mmy, 0b00000000, 0b11111111);
	/*
	//それぞれの積の和
	*mmz = _mm256_madd_epi16(*mmx, *mmy);
	//16bitx16bit=32bit
	*mmz = _mm256_hadd_epi32(*mmz, *mmz);
	*mmz = _mm256_hadd_epi32(*mmz, *mmz);
	//0バイト目と8バイト目がそれぞれの出力になる
	index = z[0]+z[8];
	*/
	//z[0] = getReverse(0b00110010);

	unsigned char in = 0b11001101;


	z[0] = getReverse(in);

	z[0] = delta_swap(z[0], 0b00100000, 2);
	z[0] = delta_swap(z[0], 0b00000100, 2);
	z[0] = delta_swap(z[0], 0b00000001, 1);

	printBit(z);
	//printf("%d\n", index);
    return 0;
}

