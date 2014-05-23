#include <cctype>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <memory.h>

static const int MAX_PHONE_NUM = 10000000; // 000-0000 to 999-9999
static int NUM_NUMBERS = 10;
static const int SIZEOF_CHAR = sizeof(char);
static bool PRINT_IT = false;

void fillPhoneNumbers(std::vector<int>&);
void setBitAt(char* buf, int bufByteSize, int bitPosition, bool value);
void ensureMatching(char*, int, std::vector<int>&);
void printPhoneNumbers(const std::vector<int>&, const std::vector<int>&);
int parseArgs(int, char**);

int main(int argc, char** argv)
{
   // parse args
   if (parseArgs(argc, argv) != 0)
      return 0;

   // initialize random seed
   srand(unsigned int(time(NULL)));

   // create some random phone numbers
   std::vector<int> phoneNumbers;
   fillPhoneNumbers(phoneNumbers);

   // allocate enought bits for 1 per phone number
   size_t chars = MAX_PHONE_NUM/(SIZEOF_CHAR*8);
   char* bits = new char[chars]; // one bit for each phone number
   memset(bits, 0, chars);

   // convert phone numbers to bits
   for (size_t i=0; i<phoneNumbers.size(); ++i)
      setBitAt(bits, chars, phoneNumbers[i], true);

   ensureMatching(bits, chars, phoneNumbers);

   // Wait
   std::cin.ignore();
   
   return 0;
}

void fillPhoneNumbers(std::vector<int>& v)
{
   // initialize the array: O(MAX_PHONE_NUM)
   std::vector<int> arr;
   arr.resize(MAX_PHONE_NUM);
   for (int i = 0; i < MAX_PHONE_NUM; ++i)
      arr[i] = i;

   // Pick unique random values: O(NUM_NUMBERS)
   int max = MAX_PHONE_NUM - 1;
   for (int i = 0; i < NUM_NUMBERS; ++i, --max)
   {
      int pick = rand() % (max+1);
      v.push_back(arr[pick]);
      std::swap(arr[pick], arr[max]);
   }
}

void setBitAt(char* buf, int bufByteSize, int bitPosition, bool value)
{
   assert(bitPosition>>3 < bufByteSize);

   if (value)
      buf[bitPosition>>3] |=  (1<<(bitPosition & 0x7));
   else
      buf[bitPosition>>3] &= ~(1<<(bitPosition & 0x7));
}

void ensureMatching(char* bits, int bitsByteSize, std::vector<int>& v)
{
   assert((MAX_PHONE_NUM-1)>>3 < bitsByteSize);
   
   std::vector<int> w;
   w.reserve(v.size());

   for (int i=0; i<MAX_PHONE_NUM; ++i)
      if ((bits[i>>3] & (1<<(i & 0x7))) != 0)
         w.push_back(i);

   if (w.size() != v.size())
      std::cout << "hmmm, your algorithm is not as good as you think..." << std::endl;

   if (PRINT_IT)
      printPhoneNumbers(v, w);
}

void printPhoneNumbers(const std::vector<int>& unsorted, const std::vector<int>& sorted)
{
   std::cout << "Unsorted:" << std::endl;
   for (size_t i=0; i<unsorted.size(); ++i)
   {
      std::cout << "\t" << unsorted.at(i) << std::endl;
   }
   std::cout << std::endl;

   std::cout << "Sorted:" << std::endl;
   for (size_t i=0; i<sorted.size(); ++i)
   {
      std::cout << "\t" << sorted.at(i) << std::endl;
   }
}

int parseArgs(int argc, char** argv)
{
   bool usage = false; 

   for (int i=1; i<argc && !usage; ++i)
   {
      std::string s(argv[i]);
      if (s.compare("-n") == 0)
      {
         if (i+1 < argc && isdigit(*argv[i+1]))
            NUM_NUMBERS = atoi(argv[++i]);
         else
            usage = true;
      }
      else if (s.compare("-p") == 0)
      {
         PRINT_IT = true;
      }
      else
         usage = true;
   }

   usage = NUM_NUMBERS <= 0 || NUM_NUMBERS > MAX_PHONE_NUM ? true : usage;

   if (usage)
   {
      std::string progName(argv[0]);
      std::cout << "Usage: " << progName << " [-n <num_values>] [-p]" << std::endl <<
      "Optional Arguments:" << std::endl <<
      "\t-n\tThe number of values to generate and sort [1, 10000000]" << std::endl <<
      "\t-p\tPrint the unsorted and sorted values" << std::endl;
   }

   return usage ? -1 : 0;
}
