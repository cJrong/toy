

#include <sb/container/sb_string.h>
#include <sb/io/sb_console.h>

#include <sb/traits/sb_args.h>
#include <sb/memory/sb_strops.h>
#include <sb/container/sb_hash_func.h>

#include <time.h>
#include <assert.h>
#include <string.h>



void split(void)
{
    char str[] = "HMSET yiibai name \"redis tutorial\" description \"redis basic commands";
    char *find = str;
    char *last = str;

    while (true) {
        find = sb_strstr(find, " ");
        if (!find) {
            break;
        }
        *find = '\0';
        printf("%s\n", last);
        find += 1;
        last = find;
    }
}

int main(void)
{
#ifdef _WIN32
    //{_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); }
#endif
    
    //sb_string_t str2 = sb_string_create("CJrong");
    /*
    sb_string_t pattern = sb_string_create("abc");
    sb_string_t to = sb_string_create("abcabc");
    int i = 0;
    time_t start = time(NULL);
    for (; i < 1000000; i++) {
        sb_string_t str1 = sb_string_create("abcabc");
        str1 = sb_string_replace_s(str1, pattern, to, -1);
        
        sb_string_destroy(str1);
    }
    printf("%d\n", time(NULL) - start);
    */
  
    char buff[1024] = {0};
    int i = 0;
    time_t start = time(nullptr);
    volatile char * buf;
    split();

    for (; i < 10000000; i++){
       buf = sb_strrpl(buff, 1024, "abcabc", 6, 
            "abc", 3, "abcabc", 6, -1);
    }

    printf("%d\n", time(nullptr) - start);
    printf("%d, %p\n", i, buf);

    //sb_string_t str1 = sb_string_create("HelloWorldzz");
    //printf("%d\n", sb_string_index(str1, "rld", 3));

    //printf("%s, %p\n", str1, sb_string_of(str1));
    //str1 = sb_string_replace(str1, "abc", "abcabc", -1);
    //printf("%s, %p\n", str1, sb_string_of(str1));
    //printf("%s, %p\n", str2, sb_string_of(str2));
    //sb_string_swap(&str1, &str2);
    //printf("%s, %p\n", str1, sb_string_of(str1));
    //printf("%s, %p\n", str2, sb_string_of(str2));
    
    /*
    sdsll2str(buf, 0xf000000080000000ull);
    printf("%s\n", buf);

    printf("%s\n", sb_string_from_llong(0x7000000080000000ull, 16));
    printf("%zu\n", sb_string_length(sb_string_from_llong(0xf000000080000000ull, 2)));
    str = sb_string_resize(str, 5);
    
    str = sb_string_range(str, 1, 3);

    printf("%s\n", str);
   */

   
    
    //sb_string_destroy(str2);
 
    return 0;
}
