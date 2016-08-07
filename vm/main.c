#include <stdlib.h>
#include <stdio.h>

unsigned int datareg[5] = { 0 };
unsigned int pc = 0;

enum INSTRUCTION
{
    NOP, MOV, ADD, SUB, MUL, DIV, JMP, LOOP, DIE
};

struct command
{
    unsigned char in;
    unsigned char reg;
    unsigned int data;
};

void debuginfo()
{
    int i;
    printf("--- instruction begin ---\n");
    printf("pc=%d\n", pc-1);
    for(i=0; i<5; i++)
    {
	printf("e%cx=%d\n", 'a'+i, datareg[i]);
    }
    printf("--- instruction end ---\n");
}

void interpret(struct command cmd)
{
    switch(cmd.in)
    {
    case MOV:
	datareg[cmd.reg-1]=cmd.data;
	pc++;
	break;
    case ADD:
	datareg[0]=datareg[cmd.reg-1]+datareg[cmd.data-1];
	pc++;
	break;
    case SUB:
	datareg[0]=datareg[cmd.reg-1]-datareg[cmd.data-1];
	pc++;
	break;
    case MUL:
	datareg[0]=datareg[cmd.reg-1]*datareg[cmd.data-1];
	pc++;
	break;
    case DIV:
	datareg[0]=datareg[cmd.reg-1]/datareg[cmd.data-1];
	pc++;
	break;
    case JMP:
	pc = cmd.data;
	break;
    case LOOP:
	if(datareg[cmd.reg-1]>0)
	{
	    datareg[cmd.reg-1]--;
	    pc = cmd.data;
	}
	else
	    pc++;
	break;
    case DIE:
	printf("You want to commit a suicide?!\n");
	exit(0);
	break;
    case NOP:
    default:
	break;
    }
    debuginfo();
}

void next(FILE *file, struct command *out)
{
    int offset;
    offset = pc*6;
    fseek(file, offset, SEEK_SET);
    /* here we read by char to avoid alignment problem
       unless we switch to a higher standard we must act in this way
       NOTE: DO NOT fread HERE! */
    out->in=fgetc(file);
    out->reg=fgetc(file);
    fread(&out->data, sizeof(int), 1, file);
}

void new_vm(FILE *file)
{
    struct command cmd;
    while(!feof(file))
    {
	next(file, &cmd);
	interpret(cmd);
    }
    printf("EOF\n");
}

int main(int argc, char *argv[])
{
    FILE *file;
    if (argc < 2)
    {
	fprintf(stderr, "Feed me something.");
	return 1;
    }
    file = fopen(argv[1], "r");
    if(!file)
    {
	fprintf(stderr, "Feed me something GOOD.");
	return 1;
    }
    rewind(file);
    new_vm(file);
    return 0;
}
