/**
 * compile: cc a.c `pkg-config --cflags --libs opencc` -lintl
 */

#include <opencc.h>

int main (int argc, 
          char *argv[])
{
    opencc_t converter;
    char strfrom[] = "这个天才刚才打算去那家鼠标公司面试！万幸的是那些家伙有眼不识泰山，没招他，我们过去吧！";
    char *result;
    converter = opencc_open (argv[1]);
    result = opencc_convert_utf8 (converter, strfrom, sizeof (strfrom)+1);
    puts (result);
    opencc_close (converter);
    free (result);
    return 0;
}
