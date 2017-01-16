

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <curl.h>
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "Document.h"
#include "Node.h"
#include <map>
#include <set>
#include <stack>
#include <algorithm>
#include <thread>   
#include <fstream>
#include "TREE.h"

//#include <unistd.h>
#define sleep(a) Sleep(a * 1)


#include <gumbo.h>
//#include "btree_node.h"
BTree t(15); // A B-Tree with minium degree 15


using namespace std;

CURL *curl;
CURLcode res;
vector<string> datos;
string dato;
CDocument doc;//SE ACTUALIZA CONSTANTEMENTE
map<string, char> all_data;


//cout << "Traversal of the constucted tree is ";
//t.traverse();

//int k = 6;
//(t.search(k) != NULL) ? cout << "\nPresent" : cout << "\nNot Present";



ofstream myfile("dataweb.html");

static size_t callback(void *data, size_t size, size_t nmemb, void *pointer) {
((string*)pointer)->append((char*)data, size * nmemb);
return size * nmemb;
}

int descargar_html(string url)
{
	//curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dato);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Dark Secret Ninja/1.0");
		//curl_easy_setopt(curl, CURLOPT_USERAGENT,
		//"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.90 Safari/537.36");
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			curl_easy_strerror(res);
			return 1;
		}
		curl_easy_cleanup(curl);
	}
	return 0;
	//doc.parse(dato.c_str());
}




void print() 
{
	std::map<string, char>::iterator it;
	for (it = all_data.begin(); it != all_data.end(); ++it)
		std::cout << it->first << " => " << it->second << '\n';
}


void extract_data()
{
doc.parse(dato.c_str());
CSelection img = doc.find("img");
CSelection audio = doc.find("audio");
CSelection video = doc.find("iframe");
if (myfile.is_open())
{
	for (int i = 0; i < img.nodeNum(); i++) {
		if (t.insert(img.nodeAt(i).attribute("src"))==true)
			myfile << "<img style = \'border-radius:35px; width:320px; height:180px;\' src =" << "\'" << img.nodeAt(i).attribute("src") << "\'" << "/>" << endl;
		
		//all_data.insert(make_pair(img.nodeAt(i).attribute("src"), 'i'));
		
	}
	for (int i = 0; i < audio.nodeNum(); i++) {
		if (t.insert(audio.nodeAt(i).attribute("src")) == true)
		     myfile << "<audio src =" << "\'" << audio.nodeAt(i).attribute("src") << "\'"
		     <<"controls>" << "</audio>" << endl;
		//all_data.insert(make_pair(audio.nodeAt(i).attribute("src"), 'a'));
	}

	for (int i = 0; i < video.nodeNum(); i++) {
		if (t.insert(video.nodeAt(i).attribute("src")) == true)
             myfile << "<iframe src =" << "\'" << video.nodeAt(i).attribute("src") << "\'" << "</iframe>" << endl;
		//all_data.insert(make_pair(video.nodeAt(i).attribute("src"), 'v'));
	}
}
//else cout << "Unable to open file";



//print();
}



void cleanup() {
	dato.clear();// all_data.clear();
}



int main(int argc, char *argv[])
{
	//(t.search(k) != NULL) ? cout << "\nPresent" : cout << "\nNot Present";

//	for (int n = 1; n < argc; n++)
		cout << "PROBANDO ARGV:" << argv[1] << '\n';


//MUSIC PAGE "http://www.bensound.com/royalty-free-music/rock"
//VIDEO PAGE "http://www.blogiswar.net/"	http://www.latam.discovery.com/

string pagina = "http://www.blogiswar.net";

vector<thread> threads;
//descargar_html(pagina);
set<string> hrefs;

if (descargar_html(pagina) == 0)//SI LA PAGINA PRINCIPAL EXISTE
{
	doc.parse(dato.c_str());
	CSelection href = doc.find("a");
	for (int i = 0; i < href.nodeNum(); i++) {
		hrefs.insert(href.nodeAt(i).attribute("href"));
	}
}
else 
{
	cout << "LA PAGINA PRINCIPAL NO EXISTE GG THE LIFE" << endl;
}

set<string>::iterator it;
if (myfile.is_open())
{
	myfile << "<!DOCTYPE html>\n<html>\n<head>\n<title>CRAWLER-AED</title></head><body>" << endl;
			
	for (it = hrefs.begin(); it != hrefs.end(); ++it)
	{
		cout << "VISITANDO LA PAGINA " << "    " << *it << endl;
		if (descargar_html(*it) == 0)
		{
			extract_data();
			dato.clear();
			//cleanup();
		}
		else
			cout << "ERROR!!!!!" << endl;
	}
}

myfile << "</body>\n</html>" << endl;
myfile.close();
cout << "COMPLETE!!!" << endl;

//print();

getchar();

}
