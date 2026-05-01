#include"operation.h"

void edit(char *frame_id, char *new_value, char *filename)
{
    //open the MP3 file to be edited
    FILE *fptr=fopen(filename,"rb");
    if(fptr==NULL)
    {
        printf("ERROR::file not found\n");
        return ;
    }

    //open or create a temp file to edit the required tag using it
    FILE *tptr=fopen("temp.mp3","wb");

    /*Here we need to to copy the all tags and header from the original to temp mp3 file
      and we need to replace the tag which we need to replace and by that we need to fix 
      the header size and remove original file and replace it with temp file by coping 
      the remaining all bits */

    //first copy the header of 10 bytes
    char header[10];
    fread(header,1,10,fptr);
    fwrite(header,1,10,tptr);

    int tag_size = ((header[6] & 0x7F) << 21) | ((header[7] & 0x7F) << 14) | ((header[8] & 0x7F) << 7 ) | (header[9] & 0x7F);
    //Now copy the tags one after the one and replace the tag data which we need to edit
    
    int processed = 0;  //To count how many tag bytes we have read so far
    int frame_found = 0; //To get wheather the tag found or not
    int new_tag_size = 0; //To get the new tag size after editing
    
    while (processed < tag_size)
    {
        //Read the Frame ID
        char id[5];
        if (fread(id, 1, 4, fptr) != 4)
            break;
        id[4] = '\0';

        // STOP when padding comes
        if (id[0] == 0)
            break;

        //Read the frame size
        unsigned char size_raw[4];
        fread(size_raw, 1, 4, fptr);

        //Converting the size into integer
        int frame_size = (size_raw[0] << 24) | (size_raw[1] << 16) | (size_raw[2] << 8 ) | size_raw[3];

        //Read the frame flags bytes
        unsigned char flags[2];
        fread(flags, 1, 2, fptr);

        //read the Frame data
        unsigned char *data = malloc(frame_size);
        fread(data, 1, frame_size, fptr);

        //update the no.of bytes we read upto now
        processed += 10 + frame_size;

        //checking the frame_id is matching with required Frame id to edit
        if (!strcmp(id, frame_id))
        {
            //if matches found=1
            frame_found = 1;

            //Creating new frame size
            int new_len = strlen(new_value);
            int new_frame_size = 1 + new_len;

            //write the ID in temp file
            fwrite(id, 1, 4, tptr);

            //converting the new size from integer to bytes
            unsigned char ns[4] = {
                (new_frame_size >> 24) & 0xFF,
                (new_frame_size >> 16) & 0xFF,
                (new_frame_size >> 8 ) & 0xFF,
                (new_frame_size      ) & 0xFF
            };
            //write the new frame size in temp file
            fwrite(ns, 1, 4, tptr);

            //write the flag in temp file
            fwrite(flags, 1, 2, tptr);

            //write encoding value and new value
            fputc(0x00, tptr);  
            fwrite(new_value, 1, new_len, tptr);

            //update the new tag size
            new_tag_size += 10 + new_frame_size;
        }
        else
        {
            //if ID not matches copy the frame one by one and update the size
            fwrite(id, 1, 4, tptr);
            fwrite(size_raw, 1, 4, tptr);
            fwrite(flags, 1, 2, tptr);
            fwrite(data, 1, frame_size, tptr);

            new_tag_size += 10 + frame_size;
        }

        free(data);
    }

    // Copy audio unchanged
    int ch;
    while ((ch = fgetc(fptr)) != EOF)
        fputc(ch, tptr);

    fclose(fptr);

    // -------- FIX TAG SIZE IN HEADER --------
    fseek(tptr, 6, SEEK_SET);  // Tag size starts at byte 6

    unsigned char ss[4] = {
        (new_tag_size >> 21) & 0x7F,
        (new_tag_size >> 14) & 0x7F,
        (new_tag_size >> 7 ) & 0x7F,
        (new_tag_size      ) & 0x7F
    };

    fwrite(ss, 1, 4, tptr);

    fclose(tptr);

    // Replace old file
    remove(filename);
    rename("temp.mp3", filename);

    if (!frame_found)
        printf("Frame %s not found\n", frame_id);
    else
        printf("Frame is found\n");
}
