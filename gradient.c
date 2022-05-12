#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lodepng.h"

struct myImage
{
  int height;
  int width;
  unsigned char **red;
  unsigned char **green;
  unsigned char **blue;
  unsigned char **opacity;
};

char *entree;
char *sortie;

struct myImage LireImage(const char *filename)
{
  struct myImage png;
  unsigned error;
  unsigned char *image;
  unsigned width, height;

  error = lodepng_decode32_file(&image, &width, &height, filename);
  if (error)
    printf("error %u: %s\n", error, lodepng_error_text(error));

  png.height = height;
  png.width = width;
  png.red = malloc(height * sizeof(char *));

  for (int i = 0; i < height; i++)
  {
    png.red[i] = malloc(width * sizeof(char));
  }
  png.green = malloc(height * sizeof(char *));
  for (int i = 0; i < height; i++)
  {
    png.green[i] = malloc(width * sizeof(char));
  }
  png.blue = malloc(height * sizeof(char *));
  for (int i = 0; i < height; i++)
  {
    png.blue[i] = malloc(width * sizeof(char));
  }
  png.opacity = malloc(height * sizeof(char *));
  for (int i = 0; i < height; i++)
  {
    png.opacity[i] = malloc(width * sizeof(char));
  }
  int sR = 0;
  int sG = 1;
  int sB = 2;
  int sO = 3;

  for (int i = 0; i < png.height; i++)
    for (int j = 0; j < png.width; j++)
    {
      png.red[i][j] = image[sR];
      png.green[i][j] = image[sG];
      png.blue[i][j] = image[sB];
      png.opacity[i][j] = image[sO];
      sR += 4;
      sG += 4;
      sB += 4;
      sO += 4;
    }

  free(image);
  return png;
}

void EcrireImage(struct myImage im, char *filename)
{

  unsigned char *image = malloc(im.width * im.height * sizeof(char *));

  int k = 0;
  for (int i = 0; i < im.height; i++)
  {
    for (int j = 0; j < im.width; j++)
    {
      image[k] = im.red[i][j];
      k++;
      image[k] = im.green[i][j];
      k++;
      image[k] = im.blue[i][j];
      k++;
      image[k] = im.opacity[i][j];
      k++;
    }
  }
  unsigned error = lodepng_encode32_file(filename, image, im.width, im.height);

  /*cas d erreur*/
  if (error)
    printf("error %u: %s\n", error, lodepng_error_text(error));
}
void DetecteurDeConteur(struct myImage im)
{
  int MR = 0;
  int MB = 0;
  int MV = 0;
  int mR = 255;
  int mB = 255;
  int mV = 255;
  int t1, t2, l1, l2;
  
  struct myImage png;
  
  png.height = im.height;
  png.width = im.width;
  png.red = malloc(im.height * sizeof(char *));

  for (int i = 0; i < im.height; i++)
  {
    png.red[i] = malloc(im.width * sizeof(char));
  }
  png.green = malloc(im.height * sizeof(char *));
  for (int i = 0; i < im.height; i++)
  {
    png.green[i] = malloc(im.width * sizeof(char));
  }
  png.blue = malloc(im.height * sizeof(char *));
  for (int i = 0; i < im.height; i++)
  {
    png.blue[i] = malloc(im.width * sizeof(char));
  }
  png.opacity = malloc(im.height * sizeof(char *));
  for (int i = 0; i < im.height; i++)
  {
    png.opacity[i] = malloc(im.width * sizeof(char));
  }
  
 
   for (int i = 0; i < im.height; i++)
  {
    for (int j = 0; j < im.width; j++)
    {

      for (t1 = -1; t1 < 2; t1++)
      {
        for (t2 = -1; t2 < 2; t2++)
        {

          l1 = i + t1;
          l2 = j + t2;
          if ((t1 != 0 && t2 != 0) && (l1 >= 0 && l2 >= 0) && (l1 < im.height && l2 < im.width))
          {
            /* RED */
            // printf("%d\t", im.red[l1][l2]);
            if (MR < im.red[l1][l2])
            {
              MR = im.red[l1][l2];
            }
            if (mR > im.red[l1][l2])
            {
              mR = im.red[l1][l2];
            }
            /* BLUE */
            if (MB < im.blue[l1][l2])
            {
              MB = im.blue[l1][l2];
            }
            if (mB > im.blue[l1][l2])
            {
              mB = im.blue[l1][l2];
            }
            /* GREEN */
            if (MV < im.green[l1][l2])
            {
              MV = im.green[l1][l2];
            }
            if (mV > im.green[l1][l2])
            {
              mV = im.green[l1][l2];
            }
          }
        }
      }
      
      png.red[i][j] = MR - mR;
      png.blue[i][j] = MB - mB;
      png.green[i][j] = MV - mV;
      png.opacity[i][j]=255;
      MR = 0;
      MB = 0;
      MV = 0;
      mR = 255;
      mB = 255;
      mV = 255;
    }
  }
   EcrireImage(png,sortie);
  //EcrireImage(im, sortie);
}
int main(int argc, char *argv[])
{
  
  struct myImage tmp2;
  if (argc == 3)
  {
    entree = argv[1];
    sortie = argv[2];
    tmp2 = LireImage(entree);
    DetecteurDeConteur(tmp2);
    return EXIT_SUCCESS;
  }
   else{
   printf("erreur");
   return EXIT_FAILURE;
   
   }
  
 }

 

