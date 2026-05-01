#include<string.h>
#include<stdio.h>
#include "operation.h"



int main(int argc,char *argv[])
{
    if(argc<2)
    {
        error();
        return 0;

    }
    if(strcmp(argv[1],"--help")==0)
    {
        help();
        return 0;
    }
    else if(strcmp(argv[1],"-v")==0)
    {
        if(argc==3)
        {
            if(strstr(argv[2],".mp3")==NULL)
            {
                printf("MP3 file must be end with ,mp3\n");
                return 0;
            }
            else{
                view(argv[2]); 
                return 1;
            }
        }
        else if (argc < 3 )
        {
            printf("Please provide MP3 file\n");
            return 0;
        }
        else
        {
            printf("invalid args");
            return 1;
        }
    }
    else if (!(strcmp(argv[1],"-e")))
    {
        char* arr1[6]={"-A","-t","-a","-y","-c","-C"};
        char* arr2[6]={"TPE1","TIT2","TALB","TYER","TCON","TCOM"};
        int found=0;
        for(int i=0;i<6;i++)
        {
            if(!strcmp(arr1[i],argv[2]))
            {
                if(strstr(argv[4],".mp3")==NULL)
                {
                    printf("MP3 file must be end with ,mp3\n");
                    return 0;
                }
                else
                {
                    found=1;
                    edit(arr2[i],argv[3],argv[4]);
                    printf("Successfully Edited\n");
                    break;
                }
            }
        }
        if(!found)
        {
            printf("ERROR:Invalid tag option\n");
            printf("Please check the help menu below\n");
            help();
        }
        return 0;
    }
    else if(strcmp(argv[1],"--help")!=0 && strcmp(argv[1],"-v")!=0 && strcmp(argv[1],"-e")!=0 )
    {
        error();
        return 0;
    }
    
    return 0;
}