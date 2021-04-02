#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include "DISK_driver.h"
char *block_buffer;
char fileName[80];
int filesFAT[5]; 
FILE *originalFile;
int partitionMounted = 0;
void initIO (){
    Partition.total_blocks = 0; 
    Partition.block_size = 0;
    for(int x = 0; x<20; x++){
        strcpy(fat[x].filename, "");
        fat[x].file_length = 0;
        
        for(int i = 0; i<10; i++){
            fat[x].blockPtrs[i] = 0;
        }

        fat[x].current_location = 0;
    }

    block_buffer = NULL; 
    
    for(int x = 0; x<5; x++){
        active_file_table[x] = NULL;
        filesFAT[x] = -1;
    } 
}
int partition(char* name, int blocksize, int totalblocks){
    Partition.total_blocks = totalblocks;
    Partition.block_size = blocksize;
 
    DIR* dir = opendir("PARTITION");

    if(dir){
    }else if(ENOENT == errno){
        system("mkdir PARTITION");
    }else{
        printf("DIRECTORY ERROR\n"); 
    }

    char createFile[80]; //create new file

    strcpy(fileName, "PARTITION/"); strcat(fileName, name); strcat(fileName, ".txt");
    strcpy(createFile, "touch "); strcat(createFile, fileName);
    system(createFile);

    printf("FILE NAME %s\n", fileName);

    FILE *openFile = fopen(fileName, "wb");
    if(openFile){
        fwrite(&Partition, sizeof(struct PARTITION), 1, openFile);
        for(int x=0; x<20;x++)
            fwrite(&fat[x], sizeof(struct FAT), 1, openFile);


        fprintf(openFile, "\n");
        char result[10];
        for(int x = 0; x<blocksize*totalblocks; x++){
 
            fprintf(openFile, "0");  
        }
    }else{
        printf("ERROR WITH FILE\n");
    }
    fclose(openFile);
    return 1;
}
int openfile (char* name){
    int ind = -1;
    for (int i = 0; i<20; i++){
        if (strcmp(name, fat[i].filename)== 0){
            ind = i;
            break;
        }
    }
    printf("IND: %d\n", ind);
    if (ind != -1){

    }
}
int mountFS (char *name){
    strcpy(fileName, "PARTITION/"); strcat(fileName, name); strcat(fileName, ".txt");
    originalFile = fopen(fileName, "rb+");
    if(originalFile){
        fread(&Partition, sizeof(Partition), 1, originalFile);
        for(int x=0; x<20;x++)
            fread(&fat[x], sizeof(struct FAT), 1, originalFile);

    }else{
        printf("ERROR READING FILE\n");
        return 0;
    }

    block_buffer = (char*) malloc(Partition.block_size*sizeof(char));

    printf("PARTITION %d, %d\n", Partition.block_size, Partition.total_blocks);
        
    for(int x=0; x<20; x++){
        printf("part %s\n", fat[x].filename);
        fat[x].current_location = 0; 
    }
        
    partitionMounted = 1;
    return 1;
}