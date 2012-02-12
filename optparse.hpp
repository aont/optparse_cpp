#ifndef OPTPARSE_HPP
#define OPTPARSE_HPP

#include <cstdio>
#include <cstdlib>

#include <vector>
#include <string>

namespace option
{
  class parser;

  struct item
  {
    char short_char;
    std::string long_str;
    std::string desc_str;
    int num_args;
    int (parser::*parse)(const char* const argv[]);

    item() : short_char('-'), num_args(0), parse(NULL)
    { }

    item
    (
     char short_char, std::string long_str,
     std::string desc_str, int num_args,
     int (parser::*parse)(const char* const argv[])
     )
      : short_char(short_char), long_str(long_str)
      , desc_str(desc_str), num_args(num_args)
      , parse(parse)
    { }

  };



  class parser
  {
  public:
    typedef int (parser::*parse_func)(const char* const argv[]);

  protected:
    std::vector<option::item> items;

  protected:
    virtual void parse_value(const char* const argv)
    {
      fprintf(stderr, "action is not defined!");
      throw;
    };

  public:
    int parse(const int argc, const char* const argv[])
    {

      this->proc_name = argv[0];
      int idx = 1;

      option_parse_start:

      if(idx==argc)
	goto option_parse_end;

      if(idx>argc) {
	fprintf(stderr, "argv overread!");
	throw;
      }

	if(argv[idx][0]=='-') { // option

	  if(argv[idx][1]=='-') { // long

	    if(argv[idx][2]=='\0') {
	      this->parse_value(argv[idx]);
	    }
	    else {
	      
	      for(int i=0; i<items.size(); ++i) {
		if(items[i].long_str == &argv[idx][2]) {
		  ++idx;
		  idx += (this->*items[i].parse)(&argv[idx]);
		  goto option_parse_start;
		}
	      }
	      fprintf(stderr, "invalid option %s\n", argv[idx]);
	      throw;

	    }

	  } // end long
	  else { // short

	    const int num_opts = strlen(argv[idx]);
	    int s=0;
	  option_parse_short_start:
	    ++s;

	    const char c_opt = argv[idx][s];

	    for(int i=0; i<items.size(); ++i) {
	      if(items[i].short_char == c_opt) {
		if(s==num_opts-1) {
		  ++idx;
		  idx += (this->*items[i].parse)(&argv[idx]);
		  goto option_parse_start;
		} else {
		  (this->*items[i].parse)(NULL);
		  goto option_parse_short_start;
		}
 
	      }
	    }
	    fprintf(stderr, "invalid option %s\n", argv[idx]);
	    throw;

	  } // short

	}
	else { // normal
	  parse_value(argv[idx]);
	  ++idx;
	  goto option_parse_start;
	}
	
      option_parse_end:
	return 0;
    }

  protected:
    const char* proc_name;

  public:
    void usage(FILE* const fp = stderr) const
    {

      fprintf(fp, "Usage: %s [args]...\n", proc_name);
      for(int i=0; i<items.size(); ++i) {

	if(items[i].short_char!='-')
	  fprintf(fp, "  -%c, --%s", items[i].short_char, items[i].long_str.c_str());
	else
	  fprintf(fp, "      --%s", items[i].long_str.c_str());

	for(int p=0; p<items[i].num_args; ++p) {
	  fprintf(fp, " value[%d]", p+1);
	}
	fprintf(fp, "\t: %s\n", items[i].desc_str.c_str());
      }

    }


  };


}

#endif // OPTPARSE_HPP
