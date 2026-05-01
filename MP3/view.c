#include"operation.h"

void view(char *filename)
{
    //Declaring variable for each tag to store data
    char title[200] = "";
    char artist[200] = "";
    char album[200] = "";
    char year[50] = "";
    char genre[200] = "";
    char composer[300] = "";


    //open the mp3 file in read mode
    FILE *fptr=fopen(filename,"rb");
    if(fptr==NULL)
    {
      printf("File not found\n");
      return ;
    }
    unsigned char id[4];
    //Reading the Header id of 3 bytes
    fread(id,1,3,fptr);
    id[3]='\0';

    //checking the id is matching
    if(strcmp(id,"ID3")==0)
    {
        //Skipping the pointer from 3 bytes that are Version,Flag
        fseek(fptr,3,SEEK_CUR);

        //Get the header size of 4 bytes
        unsigned char size[4];
        fread(size,1,4,fptr);
        long int tag_size=((size[0] & 0x7f)<<21)|((size[1] & 0x7f)<<14)|((size[2] & 0x7f)<<7)|((size[3] & 0x7f));
        long int read_bytes = 0;

        //Read frames one by one
        while (read_bytes < tag_size)
        {
            char frame_id[5];
            fread(frame_id, 1, 4, fptr);
            frame_id[4] = '\0';

            if (frame_id[0] == 0)
                break;

            unsigned char fs[4];
            fread(fs, 1, 4, fptr);

            int frame_size =
                (fs[0] << 24) | (fs[1] << 16) | (fs[2] << 8) | fs[3];

            fseek(fptr, 2, SEEK_CUR); // skip frame flags

            unsigned char *data = malloc(frame_size);
            fread(data, 1, frame_size, fptr);

            char *text = (char *)(data + 1); // skip encoding byte

            if (!strcmp(frame_id, "TIT2"))
                strcpy(title, text);
            else if (!strcmp(frame_id, "TPE1"))
                strcpy(artist, text);
            else if (!strcmp(frame_id, "TALB"))
                strcpy(album, text);
            else if (!strcmp(frame_id, "TYER"))
                strcpy(year, text);
            else if (!strcmp(frame_id, "TCON"))
                strcpy(genre, text);
            else if (!strcmp(frame_id, "TCOM"))
                strcpy(composer, text); 

            free(data);

            read_bytes += 10 + frame_size;
        }
    }
    else{
        printf("ERROR:ID3 tag is not found\n");
    }
    fclose(fptr);

    // ---------- PRINT OUTPUT LIKE YOUR SCREENSHOT ----------
    printf("---------------------------------------------------------------\n");
    printf("----------------------SELECTED VIEW DETAILS--------------------\n");
    printf("---------------------------------------------------------------\n");
    printf("                MP3 TAG READER AND EDITOR FOR ID3v2\n");
    printf("---------------------------------------------------------------\n\n");

    printf("TITLE        : %s\n", title);
    printf("ARTIST       : %s\n", artist);
    printf("ALBUM        : %s\n", album);
    printf("YEAR         : %s\n", year);
    printf("MUSIC        : %s\n", genre);
    printf("COMPOSER     : %s\n", composer);

    printf("\n---------------------------------------------------------------\n");
    printf("---------------DETAILS DISPLAYED SUCCESSFULLY-------------------\n");
    printf("---------------------------------------------------------------\n");
}

void error()
{
    printf("------------------------------------------------------------------------------------\n");
    printf("-----------------------------------::ERROR::----------------------------------------\n");
    printf("------------------------------------------------------------------------------------\n");
    printf("ERROR: ./a.out : Invalid command line arguments\n");
    printf("USEAGE:\n");
    printf("To view please pass like: ./a.out -v <mp3_file_name>\n");
    printf("To edit please pass like: ./a.out -e -t/-a/-y/-A/....<changing_text> <mp3_file_name>\n");
    printf("To get help pass like:    ./a.out --help\n");
    printf("------------------------------------------------------------------------------------\n");
}

void help()
{
    printf("------------------------------------------------------------------------------------\n");
    printf("----------------------------------::HELP MENU::-------------------------------------\n");
    printf("------------------------------------------------------------------------------------\n");
    printf("1.-v to view mp3 file contents\n");
    printf("2.-e to edit mp3 file contents\n");
    printf("\t2.1 -t -> to edit song title\n");
    printf("\t2.2 -a -> to edit artist name\n");
    printf("\t2.3 -A -> to edit album name\n");
    printf("\t2.4 -y -> to edit year\n");
    printf("\t2.5 -c -> to edit content\n");
    printf("\t2.6 -C -> to edit comments\n");
    printf("------------------------------------------------------------------------------------\n");
}