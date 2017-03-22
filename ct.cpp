#include <iostream>
#include <math.h>
#include "MyBMP.h"


using namespace std;
char tar[50] , sour[50], result[50];

void Image_color(BMP Img_In, const char *OutputName);
void sdm(BMP , float *);
void RGBtolab();
//void MSE_PSNR(BMP ,BMP );

float targetMsd[6] = {0.0};
float sourceMsd[6] = {0.0};

int main(int argc, char** argv)		//Color Space Transfer
{
	
	sscanf(argv[1], "%s", sour);
	sscanf(argv[2], "%s", tar);
	sscanf(argv[3], "%s", result);
	
	strcat(sour, ".bmp");
	strcat(tar, ".bmp");
	strcat(result, ".bmp");
	
	BMP ImgIn, ImgTarget;
	ImgIn.ReadFromFile(sour);
	ImgTarget.ReadFromFile(tar);
    
	sdm(ImgIn, sourceMsd);
	sdm(ImgTarget, targetMsd);
	
	Image_color(ImgIn, result);
	
	return 0;
}
void sdm(BMP img, float *Msd)
{
	/*float r[img.TellWidth()*img.TellHeight()];
	float g[img.TellWidth()*img.TellHeight()];
	float b[img.TellWidth()*img.TellHeight()];*/

	for(int i=0;i<img.TellWidth();i++)
	{
		for(int j=0;j<img.TellHeight();j++)
		{
			RGBApixel NewPixel = img.GetPixel(i, j);
			// normalize RGB
			float R=(float)(NewPixel.Red)/255.0;
			float G=(float)(NewPixel.Green)/255.0;
			float B=(float)(NewPixel.Blue)/255.0;
			
			float L,M,S;
			float x,y,z;
			float r,g,b;
            //RGB->XYZ
            x = 0.5141*R+0.3239*G+0.1604*B;		
            y = 0.2651*R+0.6702*G+0.0641*B;
            z = 0.0241*R+0.1228*G+0.8444*B;
            // XYZ -> LMS
            L = 0.3897*x+0.6890*y+(-0.0787)*z;
            M = (-0.2298)*x+1.1834*y+0.0464*z;
            S = z;
			// Adjust the pixel's value
            if(L<=0) L = 0.00000001;
            	
            if(M<=0) M = 0.00000001;
            
            if(S<=0) S = 0.00000001;
			
            // LMS->lms
            L = log10(L);			
            M = log10(M);
            S = log10(S);
            // lms->lab
            r = (L+M+S)/sqrt(3);		
            g = (L+M-2*S)/sqrt(6);
            b = (L-M)/sqrt(2);
            
			Msd[0] = Msd[0] + r;
			Msd[1] = Msd[1] + g;
			Msd[2] = Msd[2] + b;
		}
		
	}

	// calculating mean of lab

	Msd[0] = Msd[0]/(float)(img.TellWidth()*img.TellHeight());
	Msd[1] = Msd[1]/(float)(img.TellWidth()*img.TellHeight());
	Msd[2] = Msd[2]/(float)(img.TellWidth()*img.TellHeight());

	
	for(int i=0;i<img.TellWidth();i++)
	{
		for(int j=0;j<img.TellHeight();j++)
		{
			RGBApixel NewPixel = img.GetPixel(i, j);
			float R=(float)(NewPixel.Red)/255.0;
			float G=(float)(NewPixel.Green)/255.0;
			float B=(float)(NewPixel.Blue)/255.0;
			
			float L,M,S;
			float x,y,z;
			float r,g,b;
            
            x = 0.5141*R+0.3239*G+0.1604*B;		//RGB->LMS
            y = 0.2651*R+0.6702*G+0.0641*B;
            z = 0.0241*R+0.1228*G+0.8444*B;
            
            L = 0.3897*x+0.6890*y+(-0.0787)*z;
            M = (-0.2298)*x+1.1834*y+0.0464*z;
            S = z;
            
            if(L<=0)
            	L = 0.00000001;
            	
            if(M<=0)
            	M = 0.00000001;
            
            if(S<=0)
            	S = 0.00000001;
            
            L = log10(L);			// LMS->lms
            M = log10(M);
            S = log10(S);
            
            r = (L+M+S)/sqrt(3);		// lms->lab
            g = (L+M-2*S)/sqrt(6);
            b = (L-M)/sqrt(2);
			
			Msd[3] = Msd[3]+(r - Msd[0])*(r - Msd[0]);
			Msd[4] = Msd[4]+(g - Msd[1])*(g - Msd[1]);
			Msd[5] = Msd[5]+(b - Msd[2])*(b - Msd[2]);
		}
	}

	// calculating standard deviation
	Msd[3] = sqrt(Msd[3]/(float)(img.TellWidth()*img.TellHeight()));
	Msd[4] = sqrt(Msd[4]/(float)(img.TellWidth()*img.TellHeight()));
	Msd[5] = sqrt(Msd[5]/(float)(img.TellWidth()*img.TellHeight()));
	
	/*printf(" R mean:%f\n",Msd[0]);
	printf(" G mean:%f\n",Msd[1]);
	printf(" B mean:%f\n",Msd[2]);
	printf(" R deviation:%f\n",Msd[3]);
	printf(" G deviation:%f\n",Msd[4]);
	printf(" B deviation:%f\n",Msd[5]);*/
}

void Image_color(BMP Img_In, const char *OutputName)
{
    printf("Color Transfer...\n");
    BMP Output;
    Output.SetSize(Img_In.TellWidth() ,Img_In.TellHeight());
    Output.SetBitDepth(24);
    
    for(int i=0; i<Img_In.TellHeight(); i++)
    {
        for(int j=0; j<Img_In.TellWidth(); j++)
        {
            RGBApixel NewPixel = Img_In.GetPixel(i, j);  //讀取單一個像素結構。P.S.也可以自行改成先讀取成(R,G,B)陣列後，再做應用。 

            //----------------------------在這部分做像素的修改--------------------------------//
            
            float R=(float)(NewPixel.Red)/255.0;
			float G=(float)(NewPixel.Green)/255.0;
			float B=(float)(NewPixel.Blue)/255.0;
			
			float L,M,S;
			float x,y,z;
			float r,g,b;
            //RGB->LMS
            x = 0.5141*R+0.3239*G+0.1604*B;		
            y = 0.2651*R+0.6702*G+0.0641*B;
            z = 0.0241*R+0.1228*G+0.8444*B;
            
            L = 0.3897*x+0.6890*y+(-0.0787)*z;
            M = (-0.2298)*x+1.1834*y+0.0464*z;
            S = z;
            
            if(L<=0)
            	L = 0.00000001;
            	
            if(M<=0)
            	M = 0.00000001;
            
            if(S<=0)
            	S = 0.00000001;
            // LMS->lms
            L = log10(L);			
            M = log10(M);
            S = log10(S);
            
            //printf("%f %f %f\n",L,M,S);
            // lms->lab
            r = (L+M+S)/sqrt(3);		
            g = (L+M-2*S)/sqrt(6);
            b = (L-M)/sqrt(2);
            // 轉換 
            r = targetMsd[0]+(targetMsd[3]/sourceMsd[3])*(r-sourceMsd[0]);		
            g = targetMsd[1]+(targetMsd[4]/sourceMsd[4])*(g-sourceMsd[1]);
            b = targetMsd[2]+(targetMsd[5]/sourceMsd[5])*(b-sourceMsd[2]);
            // lab->lms
            L = (r/sqrt(3))+(g/sqrt(6))+(b/sqrt(2));	
            M = (r/sqrt(3))+(g/sqrt(6))-(b/sqrt(2));
            S = (r/sqrt(3))-2*(g/sqrt(6));
            // lms->LMS
            L = pow(10, L);			
            M = pow(10, M);
            S = pow(10, S);
            // LMS->RGB 
            R = L*4.4679+M*(-3.5873)+S*0.1193;	
            G = L*(-1.2186)+M*2.3809+S*(-0.1624);
            B = L*0.0497+M*(-0.2439)+S*1.2045;
            // handling overflow
            if(floor(255*R)>255)
            	NewPixel.Red = 255;
            else if( floor(255*R)<0)
            	NewPixel.Red = 0;
            else
            	NewPixel.Red = floor(255*R);
            	
            if(floor(255*G)>255)
            	NewPixel.Green = 255;
            else if( floor(255*G)<0)
            	NewPixel.Green = 0;
            else
            	NewPixel.Green = floor(255*G);
            
            if(floor(255*B)>255)
            	NewPixel.Blue = 255;
            else if( floor(255*B)<0)
            	NewPixel.Blue = 0;
            else
            	NewPixel.Blue = floor(255*B);
            //----------------------------修改影像結束--------------------------------//

            Output.SetPixel(i, j, NewPixel);  //儲存單一個像素結構
            
        }
        
    }
    Output.WriteToFile(OutputName);  //儲存的圖檔名字
}
