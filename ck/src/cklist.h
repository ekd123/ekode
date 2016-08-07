#ifndef __CK_LIST_H__
#define __CK_LIST_H__
void _ck_list_init (void);
unsigned int _ck_list_get_length (void);
int _ck_list_find (pid_t pid);
void _ck_list_del (pid_t pid);
pid_t *_ck_list_get (void);
void _ck_list_destroy (void);

#endif 
