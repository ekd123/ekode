#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>
 
int main(int argc, char *argv[])
{
    unsigned int *insn = NULL;
     
    insn = mmap(NULL, 4096, PROT_READ | PROT_WRITE | PROT_EXEC,
                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(MAP_FAILED != insn)
    {
        printf("%p\n", insn);
        insn[0] = 0x00000000;
        insn[1] = 0x00000000;
        __asm__ volatile (
                          "jmp *%0 \n\t"
                          "nop \n\t"
                          ::"r"(insn));
        munmap(insn, 4096);
    }
    else
    {
        printf("%s\n", strerror(errno));
    }
     
}
