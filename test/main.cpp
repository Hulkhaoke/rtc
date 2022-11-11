#include <iostream>
#include "../src/packet/pack.h"
#include "../src/packet/packtype.h"

void HandleDataCallback(char* data, unsigned int size, PackUserParams* user_ptr)
{
   printf("\nGet:\n");
   for (int i = 0; i < size; i++)
   {
       printf("%c", data[i]);
   }
}

void HandlePacketsCallback(char* packet, unsigned int size, PackUserParams* user_ptr)
{
   PackUserParams params;
	params.packet_cb = HandlePacketsCallback;
	params.data_cb = HandleDataCallback;
	params.ptr1 = nullptr;
	params.ptr2 = nullptr;

   PackReceive(packet, size, &params);
}

int main() 
{
   PackUserParams params;
	params.packet_cb = HandlePacketsCallback;
	params.data_cb = HandleDataCallback;
	params.ptr1 = nullptr;
	params.ptr2 = nullptr;

   PackInit();

   std::string str1("AAAAAABBBBBB");
   printf("\nOrigin data size:%d\n", str1.length());
   printf("%s\n", str1.c_str());
   PackSend(str1.c_str(), str1.length(), &params);

	std::string str2("cccddd");
	printf("\nOrigin data size:%d\n", str2.length());
	printf("%s\n", str2.c_str());
	PackSend(str2.c_str(), str2.length(), &params);

	getchar();
   return 0;
}