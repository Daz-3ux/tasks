#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stddef.h>
#include<assert.h>//作用是如果它的条件返回错误，则终止程序执行

struct strbuf {
    int len;     //当前缓冲区（字符串）长度
    int alloc;   //当前缓冲区（字符串）容量,也就是我们每次动态分配的数组大小
    char *buf;   //缓冲区（字符串）
};

void strbuf_init(struct strbuf *sf, size_t alloc);
void strbuf_attach(struct strbuf *sf, void *str, size_t len, size_t alloc);
void strbuf_reset(struct strbuf *sf);
void strbuf_release(struct strbuf *sf);
void strbuf_grow(struct strbuf *sb, size_t extra);
void strbuf_add(struct strbuf *sb, const void *data, size_t len);
void strbuf_addch(struct strbuf *sb, int c);
void strbuf_addstr(struct strbuf *sb, const char *s);
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2);
static inline void strbuf_setlen(struct strbuf *sb, size_t len);
static inline size_t strbuf_avail(const struct strbuf *sb);
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len);

void strbuf_rtrim(struct strbuf *sb);
//去除 sb 缓冲区左端的所有 空格，tab, '\t'。
void strbuf_ltrim(struct strbuf *sb);
//去除 sb 缓冲区右端的所有 空格，tab, '\t'。
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len);
//删除 sb 缓冲区从 pos 坐标长度为 len 的内容。





int main(int argc, char *grgv[])
{
    struct strbuf sb;
    strbuf_init(&sb, 10);
    strbuf_attach(&sb, (void*)"   xiyou", 8, 10);
    assert(strcmp(sb.buf, "   xiyou") == 0);
    printf("sb: %s\n", sb.buf);

    strbuf_add(&sb,(char*)"    tessssssssssssst     '\t'",25);
    printf("sb: %s\n", sb.buf);

    strbuf_ltrim(&sb);//删除左侧空白
    printf("sb: %s\n", sb.buf);

    strbuf_rtrim(&sb);//删除右侧空白
    printf("sb: %s\n", sb.buf);

    strbuf_remove(&sb,2,4);//删除 sb 缓冲区从 pos 坐标长度为 len 的内容。
    printf("sb: %s\n", sb.buf);
}

/**
 *  Strip whitespace from the beginning (`ltrim`), end (`rtrim`), or both side
 *  (`trim`) of a string.
 */
void strbuf_rtrim(struct strbuf *sb)//去除 sb 缓冲区右端的所有 空格，tab, '\t'。
{
    while (sb->len > 0 && isspace(sb->buf[sb->len - 1])){
		sb->len--;//栈
    }
	sb->buf[sb->len] = '\0';//有效字符串后面的一位为'\0'
}

void strbuf_ltrim(struct strbuf *sb)//去除 sb 缓冲区左端的所有 空格，tab, '\t'。
{
    char *temp = sb->buf;
	while (sb->len > 0 && isspace(*temp)) {
		temp++;
		sb->len--;
	}
	memmove(sb->buf,temp,sb->len);//避免重叠部分出错
}

void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)//删除 sb 缓冲区从 pos 坐标长度为 len 的内容。
{   
    pos -= 1;
    len -=1;
    char* temp = (char*)malloc(sizeof(char)*sb->len);
    if(temp == NULL){
        printf("在remove函数分配内存失败");
    }
    memcpy(temp,sb->buf,pos);
    memmove(temp+pos,sb->buf+len,sb->len - len);
    memmove(sb->buf, temp,strlen(temp));
}







void strbuf_grow(struct strbuf *sb, size_t extra)//将sb的长度扩大extra
{
    strbuf_setlen(sb, extra);
}

void strbuf_add(struct strbuf *sb, const void *data, size_t len)//向sb追加长度为len的数据 data。
{
    strbuf_setlen(sb,len);
    while(sb->len > sb->alloc){
        sb->alloc *= 2;
        sb->buf = (char*)realloc(sb->buf,sizeof(char) * sb->alloc + 1);
    }
    strcat(sb->buf,(char*)data);
}

void strbuf_addch(struct strbuf *sb, int c)//向sb追加一个字符c。
{
    if(sb->len+1 > sb->alloc){
        sb->alloc *= 2;
        sb->buf = (char*)realloc(sb->buf,sizeof(char) * sb->alloc + 1);
    }
    memset(sb->buf+ sb->len,(char)c,sizeof(char));
    strbuf_setlen(sb,1);
}

void strbuf_addstr(struct strbuf *sb, const char *s)//向sb追加一个字符串s。
{
    int len = strlen(s);
    strbuf_setlen(sb,len);
    while(sb->len > sb->alloc){
        sb->alloc *= 2;
        sb->buf = (char*)realloc(sb->buf,sizeof(char) * sb->alloc + 1);
    }
    strcat(sb->buf,s);
}

void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)//向一个sb追加另一个strbuf的数据。
{
    strbuf_grow(sb , sb2->len);
    while(sb->len > sb->alloc){
        sb->alloc *= 2;
        sb->buf = (char*)realloc(sb->buf,sizeof(char) * sb->alloc + 1);
    }
    strcat(sb->buf,sb2->buf);
}

static inline void strbuf_setlen(struct strbuf *sb, size_t len)//设置sb的长度len。
{
    sb->len += len;
}

static inline size_t strbuf_avail(const struct strbuf *sb)//计算sb目前仍可以向后追加的字符串长度。
{
    return sb->alloc - sb->len;
}

void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)//向sb内存坐标为pos位置插入长度为len的数据data。
{
    while(sb->len > sb->alloc){
        sb->alloc *= 2;
        sb->buf = (char*)realloc(sb->buf,sizeof(char) * sb->alloc + 1);
    }
        char *temp = (char*)malloc(sizeof(char)*sb->len);
        strncpy(temp,sb->buf,pos);
        strcat(temp,(char*)data);
        strcat(temp,sb->buf+pos+len);
        strcpy(sb->buf,temp);
    //}
}


void strbuf_init(struct strbuf *sb, size_t alloc)//初始化sf结构体，容量为 alloc。
{
    sb -> len = 0;
    sb -> alloc = alloc;
    sb -> buf = (char*)malloc(sizeof(char) * alloc);
    if(sb -> buf == NULL){
        printf("Error:初始化失败");
    }
}

void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc)//将字符串填充到sf中，长度为len,容量为alloc。
{
    if(len < alloc){
        sb -> len = len;
        strcpy(sb->buf, (char*)str);
    }else{
        printf("Error:当前容量不足");
    }
}

void strbuf_reset(struct strbuf *sb)//清空 sb。
{
    sb -> len = 0;
    sb -> buf = NULL;
}
