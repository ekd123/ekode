#include <stdio.h>
#include <glib.h>
#include "dummyhospital.h"
   
gint
main ()
{
    char cbuf[100];
    char vbuf[100];
    int cmd = 0;
    
    g_type_init ();
    
    DummyHospital *hospital = g_object_new (DUMMY_TYPE_HOSPITAL, NULL);
    if (!hospital)
        return 1;
    
    while (cmd != 'q')
    {
        printf ("-> ");
        scanf ("%s", cbuf);
        cmd = cbuf[0];
        switch (cmd)
        {
        case 'h':
            puts ("z\tBe in our hospital\nc\tLeave\nq\tClose the gate\np\tPrint the list\n");
            break;
        case 'z':
            printf ("Who wants to be in our hospital? ");
            fgets(vbuf, sizeof (vbuf), stdin);
            dummy_hospital_be_in_hospital (hospital, vbuf);
            break;
        case 'c':
            printf ("Who wants to leave our hospital? ");
            fgets (vbuf, sizeof(vbuf), stdin);
            dummy_hospital_leave_hospital (hospital, vbuf);
            break;
        case 'q':
            break;
        case 'p':
            dummy_hospital_print_list (hospital);
            break;
        default:
            g_warning ("Unknown command. Ignoring...");
            break;
        }
    }
    
    g_object_unref (hospital);
    
    return 0;
}
