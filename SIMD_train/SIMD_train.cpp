// SIMD_train.cpp : 13���߂Œ��8bit2�i������3�i����
//10110010(2):178
//   ��
//10110010(3):2514

#include "stdafx.h"
#include <immintrin.h>
#include <stdlib.h>

#define LEN 8

typedef unsigned short uint16;

//�r�b�g�z����o�͂���
void printBit(uint16 *in) {
	char buf[LEN][1000];
	for (int i = LEN - 1; i >= 0; i--) {
		itoa(in[i], buf[i], 2);
		printf("%s,", buf[i]);
	}
	printf("\n");
}

//�f�[�^�̐��`������  (shift 7, set, and)
void setData(__m128i *ret, const unsigned char input) {
	//���̕�����AVX2�ō������\�������C��PC��AVX(1)�Ȃ̂Œf�O
	*ret = _mm_set_epi16(input>>7, input >> 6, input >> 5, input >> 4, input >> 3, input >> 2, input >> 1, input);
	//*ret = _mm_srlc_epi32
	*ret = _mm_and_si128(*ret, _mm_set1_epi16(0x0001));
}

int main()
{

	alignas(16) const uint16 pow_3[LEN] = { 0x1, 0x3, 0x9, 0x1b, 0x51, 0xf3, 0x2d9, 0x88b };//1,3,9,27,81,243,729,2187
	alignas(16) uint16 y[LEN] = { 0 };
	alignas(16) uint16 z[LEN] = { 0 };
	int index;//�v�Z���ʂ̃C���f�b�N�X�l

	//���W�X�g���ɓo�^
	__m128i *mmx = (__m128i *)pow_3;
	__m128i *mmy = (__m128i *)y;
	__m128i *mmz = (__m128i *)z;

	//�f�[�^�̐��`
	setData(mmy, 0b00000000);

	//���ꂼ��̐ς̘a
	*mmz = _mm_madd_epi16(*mmx, *mmy);
	//16bitx16bit=32bit
	*mmz = _mm_hadd_epi32(*mmz, *mmz);
	*mmz = _mm_hadd_epi32(*mmz, *mmz);
	//0�o�C�g�ڂ��o�͂ɂȂ�
	index = _mm_extract_epi16(*mmz, 0);

	printf("%d\n", index);
    return 0;


}

