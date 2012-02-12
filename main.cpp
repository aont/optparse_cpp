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

	    for(int i=0; i<items.size(); ++i) {
	      if(items[i].short_char == argv[idx][1]) {
		++idx;

		idx += (this->*items[i].parse)(&argv[idx]);

		goto option_parse_start;
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

      fprintf(fp, "Usage: %s [options]... [values]...\n", proc_name);
      for(int i=0; i<items.size(); ++i) {

	if(items[i].short_char!='-')
	  fprintf(fp, "  -%c, --%s", items[i].short_char, items[i].long_str.c_str());
	else
	  fprintf(fp, "      --%s", items[i].long_str.c_str());

	for(int p=0; p<items[i].num_args; ++p) {
	  fprintf(fp, " value[%d]", p);
	}
	fprintf(fp, "\t: %s\n", items[i].desc_str.c_str());
      }

    }


  };


}



// Customized Class

class my_parser
  : public option::parser
{


public:
  my_parser()
  {
    this->items.push_back
      (option::item('-', "all", "show all", 0, (parse_func)&my_parser::all));
    this->items.push_back
      (option::item('h', "help", "help me", 0, (parse_func)&my_parser::help));
    this->items.push_back
      (option::item('1', "one", "test one", 3, (parse_func)&my_parser::test1));
  }


private:
  int all(const char* const argv[]) const
  {
    std::fprintf(stderr, "A\n");
    return 0;

  }

private:
  int help(const char* const argv) const
  {
    this->usage();
    exit(1);
    return 0;
  }

private:
  static const int test1_num_args = 3;
  int test1(const char* const argv[]) const
  {
    for(int i=0; i<test1_num_args; ++i) {
      if(argv[i][0]=='-')
	return i;
      else
	fprintf(stderr, "%s::value[%d]\t%s\n", __func__, i, argv[i]);
    }
    return test1_num_args;

  }

public:

  void parse_value(const char* const arg)
  {
    std::fprintf(stderr, "%s::value\t%s\n", __func__, arg);
  }

};


int main(const int argc, char* const argv[])
{
  my_parser opt;
  opt.parse(argc, argv);

  return 0;
}
