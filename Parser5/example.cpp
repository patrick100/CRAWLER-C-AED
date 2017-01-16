#include <iostream>
#include <cstring>
#include <curl/curl.h>
#include <vector>
#include <unistd.h>
#include "gumbo.h"
 
using namespace std;
 
CURL *curl;
CURLcode res;
 
int total = 0, current = 0;
 
string data;
 
vector<string> jokes;
 
static size_t callback(void *data, size_t size, size_t nmemb, void *pointer) {
    ( (string*)pointer)->append((char*)data, size * nmemb);
    return size * nmemb;
}
 
void extract_links( GumboNode* node )
{
    GumboAttribute* detail;
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }
    if( node->v.element.tag == GUMBO_TAG_DIV && (detail = gumbo_get_attribute(&node->v.element.attributes, "class" ) ) )
    {
        if( strstr( detail->value, "joke-box-upper") != NULL )
        {
            GumboNode* child = static_cast<GumboNode*>(node->v.element.children.data[0]);
 
            if( child->v.text.text != NULL ) {
                jokes.push_back( child->v.text.text );
            }
        }
    }
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        extract_links(static_cast<GumboNode*>(children->data[i]));
    }
}
 
int initialize_curl( const char* url )
{
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        curl_easy_setopt(curl, CURLOPT_USERAGENT,
            "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.90 Safari/537.36");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt");
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            curl_easy_strerror(res);
            return 1;
        }
        curl_easy_cleanup(curl);
    }
    return 0;
}
 
void cleanup() {
    data.clear(); jokes.clear(); current = 0; }
 
int main (int argc, char *argv[])
{
    string out;
 
    while( total < 235 )
    {
        char* buffer = new char[512];
 
        sprintf( buffer, "http://www.joke-db.com/c/all/clean/page:%i/sort:score/direction:desc", total);
 
        if( initialize_curl( buffer) == 0 )
        {
            out = data;
 
            GumboOutput* output = gumbo_parse(out.c_str());
 
            extract_links(output->root);
 
            for(vector<int>::size_type i = 0; i != jokes.size(); i++) {
                printf("Current joke: %s\r\n", jokes[current].c_str() );
                current++;
            }
 
            out.clear();
 
            delete[] buffer;
 
            cleanup();
        }
 
        total++;
 
        printf("Going to page... %i\r\n", total );
 
        sleep(1);
    }
 
    printf("Complete!");
 
    getchar();
 
    return 0;
}