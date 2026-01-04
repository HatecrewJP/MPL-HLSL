#include <stdio.h>
#include <malloc.h>

void BGRXtoRGBA(int *data,unsigned int iterations){
	
	for(unsigned int i = 0; i<iterations; i++){
	unsigned char *AsChar = (unsigned char*)data;
	unsigned char tmp = AsChar[0];
	AsChar[0] = AsChar[3];
	AsChar[0] = AsChar[2];
	AsChar[2] = tmp;
	AsChar[3] = 0xff;	
	data++;
	}
}
struct FileHeader{
	char Sig1;
	char Sig2;
	int BMPSize;
	short reserved1;
	short reserved2;
	unsigned int ImageDataOffset;
};

struct BitMapInfoHeader40{
	int HeaderSize;
	int Width;
	int Height;
	short ColorPlanes;
	short BitsPerPixel;
	int CompressionMethod;
	unsigned ImageSize;
	int HorizontalResolution;
	int VerticalResolution;
	unsigned ColorsInPalette;
	unsigned ImportantColors;
};

void* ParseBMPFile(char *Filename){
	FileHeader fh;
	char HeaderBuffer[14];
	FILE* f = fopen(Filename, "r");
	ASSERT(f);
	size_t CharsRead = fread(HeaderBuffer,1,14,f);
	fh.Sig1 = HeaderBuffer[0];
	fh.Sig2 = HeaderBuffer[1];
	fh.BMPSize = *(int*)(&HeaderBuffer[2])-14;
	fh.reserved1 = *(short*)(&HeaderBuffer[6]);
	fh.reserved2 = *(short*)(&HeaderBuffer[8]);
	fh.ImageDataOffset = *(unsigned int*)(&HeaderBuffer[10]) - 14;
	
	char* ImageBuffer = (char*)malloc(fh.BMPSize);
	BitMapInfoHeader40 ih;
	ASSERT(ImageBuffer != nullptr);
	CharsRead = fread(ImageBuffer,1,fh.BMPSize,f);
	memcpy(&ih,ImageBuffer,sizeof(ih)); 	
	
	ASSERT(ih.CompressionMethod == 0);

	char* data = (char*) malloc(ih.ImageSize);
	ASSERT(data);
	memcpy(data, ImageBuffer + fh.ImageDataOffset, ih.ImageSize);
	
	
	int PixelCount = ih.ImageSize / ih.BitsPerPixel * 8;
	size_t PixelArraySize = (PixelCount + 4 ) * sizeof(int);
	void *PixelArray = malloc(PixelArraySize);
	memset(PixelArray, 0, PixelCount * sizeof(int));
	
	int* PixelArrayAlias = (int*) PixelArray;
	PixelArrayAlias[0] = (int)PixelArraySize;
	PixelArrayAlias[1] = ih.Width;
	PixelArrayAlias[2] = ih.Height;
	PixelArrayAlias+=4;
	for (int i = 0; i < PixelCount; i++) {
		memcpy(((int*)PixelArrayAlias)+i, (data+i*3), 3);
	}
	
	BGRXtoRGBA((int*)PixelArrayAlias,PixelCount);
	
	free(ImageBuffer);
	free(data);
	fclose(f);
	return PixelArray;
}	