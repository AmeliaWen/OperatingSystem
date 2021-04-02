struct PARTITION {
    int total_blocks;
    int block_size;
}Partition;

struct FAT {
    char * filename;
    int file_length;
    int blockPtrs[10];
    int current_location;
}fat[20];

void initIO();
int partition (char* name, int blocksize, int totalblocks);
int mountFS (char* name);
FILE* active_file_table[5];
int openfile(char * name);
int readBlock(int file);
int writeBlock(int file, char* data);
