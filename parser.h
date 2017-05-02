struct option_manager
{
  char** options;
  struct options* opts;
};

struct options
{
  char* name;
  char** arguments;
  struct options *next;
};

struct options *push_back(struct options* opt, char* name);
void free_options(struct options* opt);
struct options *add_argument(struct options* opt, char *name, char *args);
int str_compare(char* str1, char* str2);
char** find_arguments(struct options* opt, char* name);
int is_option(struct options* opt, char* name);
struct option_manager* add_options(struct option_manager*, int count, ...);
struct option_manager* parse(int argc, char** argv);
