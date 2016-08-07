#ifndef REMEMBER_DENNIS
#define REMEMBER_DENNIS

struct body
{
    char alive;
};

struct dennis_ritchie
{
    struct body body;
    char *words;
    int   number;
} 
dennis_ritchie = 
{
    1,
    "goodbye, world",
    0
};

#undef remembering_dennis
void
remembering_dennis (void)
{
    extern struct dennis_ritchie dennis_ritchie;
    dennis_ritchie.alive = 0;
    /* his pointer is cast to void * */
    printf ("%s from 0x%X", dennis_ritchie.words, 
            (unsigned int)((void *) &dennis_ritchie));
    /* his life is term with exit code 0 */
    exit (dennis_ritchie.number);
}

#endif
