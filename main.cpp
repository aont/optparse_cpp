#include <optparse.hpp>


// Customized Class

class my_parser
  : public option::parser
{

public:
  my_parser()
  {
    this->items.push_back
      (option::item('a', "all", "show all", false,
		    (parse_func)&my_parser::all));
    this->items.push_back
      (option::item('h', "help", "help me", false,
		    (parse_func)&my_parser::help));
    this->items.push_back
      (option::item('1', "one", "test one", true,
		    (parse_func)&my_parser::test1));
  }


private:
  int all(const char* const argv) const
  {
    (void)argv;

    std::fprintf(stderr, "A\n");
    return 0;

  }

private:
  int help(const char* const argv) const
  {
    (void)argv;

    this->usage();
    //exit(1);
    return 0;
  }

private:
  int test1(const char* const argv) const
  {
    if(argv==NULL||argv[0]=='-')
      return 0;
    else {
      fprintf(stderr, "%s::value\t%s\n", __func__, argv);
      return 1;
    }

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
