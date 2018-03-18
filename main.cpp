#include "stdio.h"
#include <SDL2/SDL.h>



int main(int argc, char *argv[]){

	const char *result_filename="font.c";


	if(argc < 2){
		fprintf(stderr,"Not enough arguments\n");
		return -1;
	}

	const char * file = argv[1];
	int char_width=8;
	int char_height = atoi(argv[2]);
	FILE *fout = fopen(result_filename,"w");

	if(fout == NULL){
		fprintf(stderr,"Cannot open \"%s\" for write \n",result_filename);
		return -1;
	}

	/*if(char_width < 8 || char_width > 16){
		fprintf(stderr,"Char width out of bounds (%i). should be between 8-16pixels width\n",char_width);
		return -1;
	}*/

	char_width=8;

	if(char_height < 8 || char_height > 16){
		fprintf(stderr,"Char height out of bounds (%i). should be between 8-16pixels height\n",char_height);
		return -1;
	}


	SDL_Surface *srf=SDL_LoadBMP(file);
	if(srf!=NULL){

		int mWidth=srf->w;
		int mHeight=srf->h;
		int totalchars_x=mWidth/char_width;
		int totalchars_y=mHeight/char_height;
		int totalchars=totalchars_x*totalchars_y;

		if(totalchars > 256){
			fprintf(stderr,"Max chars reached (%i)\n",totalchars);
			SDL_FreeSurface(srf);
			fclose(fout);
			return -1;
		}


		unsigned char *pixels=NULL;//(unsigned int *)srf->pixels;
		//unsigned int offsetX=0;

		printf("Loaded font (%ix%i). Font size: %ix%i. wc:%i hc:%i total chars:%i BytePerPixel:%i\n",mWidth,mHeight,char_width,char_height,totalchars_x,totalchars_y,totalchars,srf->format->BytesPerPixel);

		fprintf(fout,"const char font_%ix%i[%i][%i]={",char_width,char_height,totalchars,char_height);

		for(int c=0; c < totalchars;c++){

			fprintf(fout,"\n\t");
			if(c>0){
				fprintf(fout,",");
			}
			fprintf(fout,"{");

			int row = c/totalchars_x;
			int col = c%totalchars_x;

			for(int y=0; y < char_height ;y++){

				unsigned int pm=0;

				pixels=((unsigned char *)srf->pixels + (row*char_height+y)*srf->pitch+col*char_width*srf->format->BytesPerPixel);
				for(int x=0; x < char_width ;x++){
					if(*pixels!=0){
						pm|=(0x1<<x);
					}

					//unsigned int pm=font_8x13[c][y];
					//if(pm & (0x1<<x)){
					//	*((unsigned int *)pixels)=RMASK32|GMASK32|BMASK32;
					//}
					pixels+=srf->format->BytesPerPixel;
				}

				if(y>0){
					fprintf(fout,",");
				}

				fprintf(fout,"0x%02x",pm);


				//pixels=pixels + srf->pitch - (4*char_width);
			}


			fprintf(fout,"}");

			if(32<=c && c <=128){
				fprintf(fout,"// '%c'",c);
			}

			//offsetX+=(char_width*4);
		}

		fprintf(fout,"\n};");


		SDL_FreeSurface(srf);

		fclose(fout);

	}
	else{
		fprintf(stderr,"SDL_LoadBMP:%s\n",SDL_GetError());
	}


	return 0;
}
