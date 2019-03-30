#include <stdio.h>
#include <stdlib.h>

#define ALLOCATION_ERROR 1
#define FILE_ERROR 2
#define OTHER_ERROR 3
#define TAPE_SIZE 30000

int main(int argc,char** argv){
	if(argc!=2){
		puts("Usage: bfic <source>");
		exit(OTHER_ERROR);
	}
	unsigned char* tape=calloc(TAPE_SIZE,1);
	if(tape==NULL){
		fprintf(stderr,"Fatal: failed to allocate %zu bytes.\n",(size_t)TAPE_SIZE*1);
		exit(ALLOCATION_ERROR);
	}
	unsigned char* tape_ptr=tape;
	FILE* input_file=fopen(argv[1],"rb");
	if(input_file==NULL){
		fprintf(stderr,"Error: failed to open file %s\n",argv[1]);
		exit(FILE_ERROR);
	}
	fseek(input_file,0,SEEK_END);
	size_t code_size=(size_t)ftell(input_file);
	fseek(input_file,0,SEEK_SET);
	unsigned char* code=malloc(code_size+1);
	if(code==NULL){
		fprintf(stderr,"Fatal: failed to allocate %zu bytes.\n",code_size+1);
		exit(ALLOCATION_ERROR);
	}
	if(fread(code,1,code_size,input_file)!=code_size){
		fprintf(stderr,"Error: failed to read from file %s\n",argv[1]);
		exit(FILE_ERROR);
	}
	fclose(input_file);
	code[code_size]=0;
	for(unsigned char* code_ptr=code;*code_ptr;++code_ptr){
		switch(*code_ptr){
			case '>':
				++tape_ptr;
				break;
			case '<':
				--tape_ptr;
				break;
			case '+':
				++*tape_ptr;
				break;
			case '-':
				--*tape_ptr;
				break;
			case ',':
				*tape_ptr=(unsigned char)getchar();
				break;
			case '.':
				putchar(*tape_ptr);
				fflush(stdout);
				break;
			case '[':
				if(!*tape_ptr){
					int loop=1;
					while(loop){
						++code_ptr;
						if(*code_ptr=='[')
							++loop;
						if(*code_ptr==']')
							--loop;
					}
				}
				break;
			case ']':
				if(*tape_ptr){
					int loop=1;
					while(loop){
						--code_ptr;
						if(*code_ptr=='[')
							--loop;
						if(*code_ptr==']')
							++loop;
					}
				}
				break;
		}
	}
	free(tape);
	free(code);
}