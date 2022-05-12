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
char *calque;
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
void Incrustation(struct myImage im, struct myImage im2)
{
   int m = 0;
  for (int i = 0; i < im.height; i++)
  {
    for (int j = 0; j < im.width; j++)
    {
      if ((im.red[i][j] == 255) && (im.green[i][j] == 255) && (im.blue[i][j] == 255))
      {
        m = (im2.red[i][j] + im2.green[i][j] + im2.blue[i][j]) / 3;
        m += ((rand() % 50) - 25);
     
       if(m<0){
       m=0;
       }
       
       else if(m>255){
       m=255;
       }
        
        im2.red[i][j] = m;
        im2.green[i][j] = m;
        im2.blue[i][j] = m;
      }
        
    }
  }

  EcrireImage(im2, sortie);
}
  int main(int argc, char *argv[])
{
 
  struct myImage tmp2,tmp;
  if (argc == 4 )
  {
  entree = argv[1];
  calque = argv[2];
  sortie = argv[3];
  tmp2 = LireImage(entree);
  tmp = LireImage(calque);
  Incrustation(tmp, tmp2);
  
  return EXIT_SUCCESS;
  }
  else{
   printf("erreur");
   return EXIT_FAILURE;
  
  }
  
}

