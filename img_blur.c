#include <stdio.h>
#include <stdlib.h>
 
 
int main()
{
    FILE *fin, *fout;
    char path_in[64], path_out[64], **rev, px;
    int sum, width, height, read, i, j;
     
    printf("Input file name: ");
    scanf("%s", path_in);
    printf("Output file name: ");
    scanf("%s", path_out);
     
    printf("Width of image (in pixels): ");
    scanf("%d", &width);
    printf("Height of image (in pixels): ");
    scanf("%d", &height);
     
    fin = fopen(path_in, "rb");
    fout = fopen(path_out, "wb");
     
    rev = (char **)malloc(height * sizeof(char *));
    for(i = 0; i < height; i++)
        rev[i] = (char *)malloc(width * sizeof(char));
    //Store pixel values from image in a width x height unsigned char matrix
    for(i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            read = fread(&px, sizeof(char), 1, fin);
            rev[i][j] = px;
        }
    }
    //Blur image using average of neighboring pixels
    sum = 0;
    for(i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            //Top row of image
            if(i == 0)
            {
                if(j == 0)
                    sum = (rev[i][j] + rev[i][j + 1] + 
                            rev[i + 1][j] + rev[i + 1][j + 1]) / 4;
                else if(j == width - 1)
                    sum = (rev[i][j] + rev[i][j - 1] +
                            rev[i + 1][j] + rev[i + 1][j - 1]) / 4;
                else
                    sum = (rev[i][j] + rev[i][j - 1] + rev[i][j + 1] +
                            rev[i + 1][j] + rev[i + 1][j - 1] + rev[i + 1][j + 1]) / 6;
            }
            //Bottom row of image
            else if(i == height - 1)
            {
                if(j == 0)
                    sum = (rev[i][j] + rev[i][j + 1] + 
                            rev[i - 1][j] + rev[i - 1][j + 1]) / 4;
                else if(j == width - 1)
                    sum = (rev[i][j] + rev[i][j - 1] +
                            rev[i - 1][j] + rev[i - 1][j - 1]) / 4;
                else
                    sum = (rev[i][j] + rev[i][j - 1] + rev[i][j + 1] +
                            rev[i - 1][j] + rev[i - 1][j - 1] + rev[i - 1][j + 1]) / 6;
            }
            //Left side of image (excluding top or bottom row)
            else if(j == 0)
                sum = (rev[i][j] + rev[i - 1][j] + rev[i + 1][j] +
                        rev[i][j + 1] + rev[i - 1][j + 1] + rev[i + 1][j + 1]) / 6;
            //Right side of image (excluding top or bottom row)
            else if(j == width - 1)
                sum = (rev[i][j] + rev[i - 1][j] + rev[i + 1][j] + 
                        rev[i][j - 1] + rev[i - 1][j - 1] + rev[i + 1][j - 1]) / 6;
            rev[i][j] = (unsigned char)sum;
        }
    }
    //Write each pixel in rev to output file
    for(i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            if(j < width && i < height)
                fwrite(&rev[i][j], sizeof(char), 1, fout);
        }
    }
    //Close input and output file
    fclose(fout);
    fclose(fin);
     
    return 0;
}