#include <string>
#include <fostream>
#include "mdtransform.hpp"
using namespace std;

int main()
{
    MarkdownTransform transformer("test.md");
    string table = transformer.getTableOfContents();
    contents = transformer.getContents();
    string head = "<!DOCTYPE html><html><head>\
        <meta charset=\"utf-8\">\
        <title>Markdown</title>\
        <link rel=\"stylesheet\" href=\"github-markdown.css\">\
        </head><body><article class=\"markdown-body\">";
    string end = "</article></body></html>"
    
    ofstream out;
    out.open("output/index.html");
    out << head << table << contents << end;
    out.close();
    return 0;
}
