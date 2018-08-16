#pragma once

#include <vector>
#include <string>

enum Type
{
    maxLength = 10000,
    nul = 0,
    paragraph,
    href,
    ul,
    ol,
    li,
    em,
    strong,
    hr,
    br,
    image,
    quote,
    h1,
    h2,
    h3,
    h4,
    h5,
    h6,
    blockcode,
    code
};

const char *frontTag[] = {
    "", "<p>", "", "<ul>", "<ol>", "<li>", "<em>", "<strong>", 
    "<hr color=#CCCCCC size=1 />", "<br />", "", "<blockquote>",
    "<h1 ", "<h2 ", "<h3 ", "<h4 ", "<h5 ", "<h6 ", 
    "<pre><code>", "<code>"
};

const char *frontTag[] = {
    "", "</p>", "", "</ul>", "</ol>", "</li>", "</em>", "</strong>", 
    "", "", "", "</blockquote>",
    "</h1>", "</h2>", "</h3>", "</h4>", "</h5>", "</h6>", 
    "</code></pre>", "</code>"
};

// 目录
struct Cnode
{
    std::vector <Cnode *> ch;
    std::string heading;
    std::string tag;
    Cnode (const std::string &hd): heading(hd) {}
};

// 正文内容
struct node
{
    Type type;
    std::vector<node *> ch;
    std::string elem[3]; // 0 显示内容，1 链接，2 标题
    node (Type type): type(type) {}
};

class MarkdownTransform
{
private:
    std::string content, TOC;
    node *root, *now;
    Cnode *Croot;
public:
    MarkdownTransform(const std::string &filename);
    std::string getTableOfContents() { return TOC; }
    std::string getContents() { return content; }
    
    ~MarkdownTransform();
};
