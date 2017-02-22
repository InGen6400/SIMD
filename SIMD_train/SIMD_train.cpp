// SIMD_train.cpp : 13命令で底を8bit2進数から3進数へ
//10110010(2):178
//   ↓
//10110010(3):2514
#include "stdafx.h"
#include <immintrin.h>
#include <stdlib.h>

#define LEN 16

typedef unsigned short uint16;

//ビット配列を出力する
void printBit(uint16 *in) {
	char buf[LEN][1000];
	for (int i = LEN - 1; i >= 0; i--) {
		itoa(in[i], buf[i], 2);
		printf("%s,", buf[i]);
	}
	printf("\n");
}

//データの整形をする  (shift 7, set, and)
void setData(__m256i *ret, const unsigned char input1, const unsigned char input2) {
	//この部分はAVX2で高速化可能だがメインPCがAVX(1)なので断念
	*ret = _mm256_set_epi16(input2 >> 7, input2 >> 6, input2 >> 5, input2 >> 4, input2 >> 3, input2 >> 2, input2 >> 1, input2, 
							input1 >> 7, input1 >> 6, input1 >> 5, input1 >> 4, input1 >> 3, input1 >> 2, input1 >> 1, input1);
	//*ret = _mm256_set1_epi16(0x0001);
	*ret = _mm256_and_si256(*ret, _mm256_set1_epi16(0x0001));
}

int main()
{

	alignas(16) const uint16 pow_3[LEN] = { 0x1*2, 0x3*2, 0x9*2, 0x1b*2, 0x51*2, 0xf3*2, 0x2d9*2, 0x88b*2, 0x1, 0x3, 0x9, 0x1b, 0x51, 0xf3, 0x2d9, 0x88b };//(1,3,9,27,81,243,729,2187)*2 と1,3,9,27,81,243,729,2187
	alignas(16) uint16 y[LEN] = { 0 };
	alignas(16) uint16 z[LEN] = { 0 };
	int index;//計算結果のインデックス値

	//レジストリに登録
	__m256i *mmx = (__m256i *)pow_3;
	__m256i *mmy = (__m256i *)y;
	__m256i *mmz = (__m256i *)z;

	//データの整形
	setData(mmy, 0b00000000, 0b00000000);
	printBit(y);
	//それぞれの積の和
	*mmz = _mm256_madd_epi16(*mmx, *mmy);
	//16bitx16bit=32bit
	*mmz = _mm256_hadd_epi32(*mmz, *mmz);
	*mmz = _mm256_hadd_epi32(*mmz, *mmz);
	//0バイト目が出力になる
	index = z[0];

	printf("%d\n", index);
    return 0;


}

