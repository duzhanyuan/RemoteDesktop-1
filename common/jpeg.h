#pragma once

extern "C"{
#include "jpeglib.h"
};

class JPEG
{
public:
	static bool Decompress(unsigned char* data, const unsigned int dataSize, BitmapInfo* pBitmapInfo, unsigned char** ppDestBuffer)
	{
		struct jpeg_decompress_struct cinfo;
		struct jpeg_error_mgr jerr;

		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);

		//���ڴ��ȡ
		jpeg_mem_src(&cinfo, data, dataSize);

		jpeg_read_header(&cinfo, TRUE);
		jpeg_start_decompress(&cinfo);

		int row_stride = cinfo.output_width * cinfo.output_components;

		ZeroMemory(&pBitmapInfo->bmiHeader, sizeof(BITMAPINFOHEADER));
		pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pBitmapInfo->bmiHeader.biWidth = cinfo.output_width;
		pBitmapInfo->bmiHeader.biHeight = cinfo.output_height;
		pBitmapInfo->bmiHeader.biPlanes = 1;
		pBitmapInfo->bmiHeader.biBitCount = 24;
		pBitmapInfo->bmiHeader.biCompression = BI_RGB;
		pBitmapInfo->bmiHeader.biSizeImage = row_stride * cinfo.output_height;

		*ppDestBuffer = new unsigned char[pBitmapInfo->bmiHeader.biSizeImage];

		unsigned char* buffer = new unsigned char[cinfo.output_width * cinfo.output_components];
		memset(buffer, 0, cinfo.output_width * cinfo.output_components);
		while (cinfo.output_scanline < cinfo.output_height)  
		{  
			int line = cinfo.output_scanline;//��ǰ����  
			jpeg_read_scanlines(&cinfo, &buffer, 1); //ִ�иò�����ȡ��line�����ݣ�cinfo.output_scanline����һ��ָ����һ��Ҫɨ�����  
			for (size_t i = 0; i < cinfo.output_width; i++)
			{
				(*ppDestBuffer)[line * row_stride + i * cinfo.output_components]= buffer[i * 3];
				(*ppDestBuffer)[line * row_stride + i * cinfo.output_components + 1] = buffer[i * 3 + 1];
				(*ppDestBuffer)[line * row_stride + i * cinfo.output_components + 2] = buffer[i * 3 + 2];
			}
		}
		delete[] buffer;

		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
		return true;
	}

	static bool Compress(unsigned int width, unsigned int height, WORD wPixelBytes, unsigned char* pSrc, int quality, unsigned char** pDest, unsigned int* pDestLength)
	{
		assert(wPixelBytes == 3);

		struct jpeg_compress_struct cinfo;
		struct jpeg_error_mgr jerr;
		JSAMPROW row_pointer[1];
		cinfo.err = jpeg_std_error(&jerr);  
		jpeg_create_compress (&cinfo);
		jpeg_mem_dest(&cinfo, pDest, (unsigned long*)pDestLength);

		// ����ѹ����������Ҫ������ͼ����ߡ�ɫ��ͨ��������������ͼ�񣬣���������
		// ɫ�ʿռ䣨JCS_GRAYSCALE��ʾ�Ҷ�ͼ��JCS_RGB��ʾ��ɫͼ�񣩣�ѹ������
		cinfo.image_width = width;
		cinfo.image_height =  height;
		cinfo.input_components = 3;
		cinfo.in_color_space = JCS_RGB;

		jpeg_set_defaults(&cinfo);
		jpeg_set_quality(&cinfo, quality, TRUE);
		jpeg_start_compress(&cinfo, TRUE);

		while (cinfo.next_scanline < cinfo.image_height)
		{
			row_pointer[0] = &pSrc[cinfo.next_scanline * width * wPixelBytes];
			jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}
		jpeg_finish_compress(&cinfo);
		jpeg_destroy_compress(&cinfo);
		return true;
	}
};
