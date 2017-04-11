#include <stdio.h>
#include "..\libpng16\png.h"

//////////////////////////////////////////////////////////////////////////

#define        FILE_ERROR        -1

class PngLoader
{
public:
	static inline long ReadPngData( const char *szPath, int *pnWidth, int *pnHeight, unsigned char **cbData )
	{
		FILE *fp = NULL;
		long file_size = 0, pos = 0;
		int color_type = 0, x = 0, y = 0, block_size = 0;

		png_infop info_ptr;
		png_structp png_ptr;
		png_bytep *row_point = NULL;

		fp = fopen( szPath, "rb" );
		if( !fp )    return FILE_ERROR;            //�ļ��򿪴����򷵻� FILE_ERROR

		png_ptr  = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);        //����png��ȡ�ṹ
		info_ptr = png_create_info_struct(png_ptr);        //png �ļ���Ϣ�ṹ
		png_init_io(png_ptr, fp);                //��ʼ���ļ� I\O
		png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);                //��ȡpng�ļ�

		*pnWidth  = png_get_image_width( png_ptr, info_ptr );        //���ͼƬ���
		*pnHeight = png_get_image_height( png_ptr, info_ptr );        //���ͼƬ�߶�
		color_type = png_get_color_type( png_ptr, info_ptr );        //���ͼƬ��ɫ����
		file_size = (*pnWidth) * (*pnHeight) * 4;                    //������Ҫ�洢RGB(A)����������ڴ��С
		*cbData = (unsigned char *)malloc(file_size);            //����������ڴ�, ��������� cbData ָ��ָ��������������

		row_point = png_get_rows( png_ptr, info_ptr );            //��ȡRGB(A)����

		block_size = color_type == 6 ? 4 : 3;                    //�����Ƿ����aͨ���ж�ÿ����Ҫ��ȡ�����ݴ�С, ����Alphaͨ����ÿ�ζ�4�ֽ�, �����3�ֽ�

		//����ȡ����RGB(A)���ݰ��涨��ʽ����������ڴ���
		for( x = 0; x < *pnHeight; x++ )
		{
			for( y = 0; y < *pnWidth*block_size; y+=block_size )
			{
				(*cbData)[pos++] = row_point[x][y + 2];        //B
				(*cbData)[pos++] = row_point[x][y + 1];        //G
				(*cbData)[pos++] = row_point[x][y + 0];        //R
				(*cbData)[pos++] = row_point[x][y + 3];        //Alpha
			}
		}
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		fclose( fp );
		return file_size;
	}
};