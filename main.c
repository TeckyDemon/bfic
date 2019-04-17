#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOCATION_ERROR 1
#define FILE_ERROR 2
#define OTHER_ERROR 3
#define TAPE_SIZE ((size_t)300000)

static inline void*
xmalloc(size_t size){
	void* pointer=malloc(size);
	if(pointer==NULL){
		fprintf(stderr,"Fatal: failed to allocate %zu bytes.\n",size);
		exit(ALLOCATION_ERROR);
	}
	return pointer;
}
static inline void*
xcalloc(size_t nitems,size_t size){
	void* pointer=calloc(nitems,size);
	if(pointer==NULL){
		fprintf(stderr,"Fatal: failed to allocate %zu bytes.\n",nitems*size);
		exit(ALLOCATION_ERROR);
	}
	return pointer;
}
static inline FILE*
xfopen(const char* filename){
	FILE* input_file=fopen(filename,"rb");
	if(input_file==NULL){
		fprintf(stderr,"Error: failed to open file %s\n",filename);
		exit(FILE_ERROR);
	}
	return input_file;
}
static inline unsigned char*
read_code(FILE* input_file){
	fseek(input_file,0,SEEK_END);
	size_t code_size=(size_t)ftell(input_file);
	fseek(input_file,0,SEEK_SET);
	unsigned char* code=xmalloc(code_size+1);
	if(fread(code,1,code_size,input_file)!=code_size){
		perror("Error: failed to read from file\n");
		exit(FILE_ERROR);
	}
	code[code_size]=0;
	return code;
}
static inline void
find_matching_bracket(unsigned char** tape_ptr,unsigned char** code_ptr){
	int is_right_bracket=']'==**code_ptr;
	if(is_right_bracket?**tape_ptr:!**tape_ptr){
		int depth=1;
		while(depth>0){
			is_right_bracket?--*code_ptr:++*code_ptr;
			if(**code_ptr=='[')
				is_right_bracket?--depth:++depth;
			if(**code_ptr==']')
				is_right_bracket?++depth:--depth;
		}
	}
}
static inline void
run(const char* filename){
	FILE* input_file=xfopen(filename);
	unsigned char* tape=xcalloc(TAPE_SIZE,1);
	unsigned char* tape_ptr=tape;
	unsigned char* code=read_code(input_file);
	unsigned char* code_ptr=code;
	fclose(input_file);
	for(;*code_ptr;++code_ptr){
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
			case ']':
				find_matching_bracket(&tape_ptr,&code_ptr);
				break;
		}
	}
	free(tape);
	free(code);
}
static inline void
parse_args(int argc){
	if(argc!=2){
		puts("Usage: bfic <source>");
		exit(OTHER_ERROR);
	}
}
int
main(int argc,char** argv){
	parse_args(argc);
	run(argv[1]);
}
