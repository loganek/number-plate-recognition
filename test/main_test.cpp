#include <regex>
#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <stdexcept>
#include <chrono>

#define LOG_E(str, throw_exc)			\
  std::cout << "[ERROR] " << str << std::endl;	\
  if (throw_exc)				\
    throw std::runtime_error (str)

#define LOG_E_EXCEPT(str)			\
  LOG_E(str, true)

#define LOG_E_NO_EXCEPT(str)			\
  LOG_E(str, false)


#define LOG_W(str)				\
  std::cout << "[WARN] " << str << std::endl

#define LOG_D(str)				\
  std::cout << "[DEBUG] " << str << std::endl

#define MEASURE_TIME(unit, ...)						\
  [&] {									\
    using namespace std::chrono;					\
    auto start = high_resolution_clock::now ();				\
    __VA_ARGS__;							\
    auto time = high_resolution_clock::now () - start;			\
    return duration_cast<unit> (time).count ();				\
  } ();

using namespace std;

typedef vector<pair<string, string>> description_array;

const char* get_number_plates (const char* filename)
{
  return "not implemented yet";
}

void run_processing (const string& filename, const string& expected)
{
  LOG_D("start processing " + filename);
  string result;
  auto duration = MEASURE_TIME(milliseconds, 
	       result = get_number_plates (filename.c_str ());
	       );
  LOG_D("Running time: " + to_string (duration));
  if (result == expected)
    LOG_D("OK");
  else
    LOG_W("whoops. result: " + result + "\n\texpected: " + expected);
  LOG_D("stop processing " + filename);
}

void run_tests (const description_array& description, const string& dirname, const string& filter)
{
  regex r_filter (filter);

  for (auto& entry : description)
    {
      if (regex_search (entry.first, r_filter))
	{
	  string filename = dirname + entry.first;
	  run_processing (entry.first, entry.second);
	}
      else
	LOG_D ("ignoring " + entry.first);
    }
}

description_array load_description (const string& filename)
{
  ifstream file (filename);

  if (!file.is_open ())
    LOG_E_EXCEPT("cannot open file " + filename);

  string line;
  description_array c;
  while (getline (file, line))
    {
      istringstream iss (line);
      vector<string> tokens {istream_iterator<string> {iss},
	  istream_iterator<string> {}};
      
      c.push_back (description_array::value_type (tokens.front (), tokens.back ()));
    }

  LOG_D ("loadded " + to_string (c.size ()) + " test cases");

  return c;
}


int main (int argc, char** argv)
{
  string filename, dirname, filter;

  if (argc < 2)
    {
      filename = "tests.txt";
      LOG_W ("used default description filename " + filename);
    }
  else
    filename = argv[1];

  if (argc < 3)
    {
      dirname = "./";
      LOG_W ("used default directory with images" + dirname);
    }
  else
    dirname = argv[2];

  if (argc < 4)
    {
      filter = "";
      LOG_W ("used default filter " + filter);
    }
  
  description_array description = load_description (filename);

  run_tests (description, dirname, filter);
  
  return 0;
}
