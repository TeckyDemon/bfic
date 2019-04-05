#include <stdio.h>
#include <stdlib.h>

#define ALLOCATION_ERROR 1
#define FILE_ERROR 2
#define OTHER_ERROR 3
#define TAPE_SIZE ((size_t)300000)

static inline FILE* get_file_handle(const char* filename){
	FILE* input_file=fopen(filename,"rb");
	if(input_file==NULL){
		fprintf(stderr,"Error: failed to open file %s\n",filename);
		exit(FILE_ERROR);
	}
	return input_file;
}
static inline unsigned char* read_code(FILE* input_file){
	fseek(input_file,0,SEEK_END);
	size_t code_size=(size_t)ftell(input_file);
	fseek(input_file,0,SEEK_SET);
	unsigned char* code=malloc(code_size+1);
	if(code==NULL){
		fprintf(stderr,"Fatal: failed to allocate %zu bytes.\n",code_size+1);
		exit(ALLOCATION_ERROR);
	}
	if(fread(code,1,code_size,input_file)!=code_size){
		perror("Error: failed to read from file\n");
		exit(FILE_ERROR);
	}
	code[code_size]=0;
	return code;
}
static inline unsigned char* create_tape(){
	unsigned char* tape=calloc(TAPE_SIZE,1);
	if(tape==NULL){
		fprintf(stderr,"Fatal: failed to allocate %zu bytes.\n",TAPE_SIZE*1);
		exit(ALLOCATION_ERROR);
	}
	return tape;
}
static inline void find_matching_bracket(unsigned char** tape_ptr,unsigned char** code_ptr){
	int is_right_bracket=']'==**code_ptr;
	if(is_right_bracket?**tape_ptr:!**tape_ptr){
		int loop=1;
		while(loop){
			is_right_bracket?--*code_ptr:++*code_ptr;
			if(**code_ptr=='[')
				is_right_bracket?--loop:++loop;
			if(**code_ptr==']')
				is_right_bracket?++loop:--loop;
		}
	}
}
static inline void run(const char* filename){
	FILE* input_file=get_file_handle(filename);
	unsigned char* tape=create_tape();
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
static inline void parse_args(int argc){
	if(argc!=2){
		puts("Usage: bfic <source>");
		exit(OTHER_ERROR);
	}
}
int main(int argc,char** argv){
	parse_args(argc);
	run(argv[1]);
}