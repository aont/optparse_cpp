#include <optparse.hpp>


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
