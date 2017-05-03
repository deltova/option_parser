#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "parser.h"


int str_compare(char* str1, char* str2)
{
  int i = 0;
  for (; str1[i] != 0 && str2[i] != 0; ++i)
  {
    if (str1[i] != str2[i])
      return 0;
  }
  return str1[i] == 0 && str2[i] == 0;
}

struct options *push_back(struct options* lst, char *name, int nb_args)
{
  if (is_option(lst, name))
    return lst;
  struct options* save = lst;
  while (lst && lst->next)
  {
    lst = lst->next;
  }
  if (!lst)
  {
    lst = malloc(sizeof (struct options));
    lst->name = name;
    lst->nb_opt = nb_args;
    lst->arguments = NULL;
    lst->next = NULL;
    return lst;
  }
  else
  {
    lst->next = malloc(sizeof (struct options));
    lst->next->nb_opt = nb_args;
    lst->next->name = name;
    lst->next->arguments = NULL;
    lst->next->next = NULL;
    return save;
  }
}
int str_size(char* str)
{
  int i = 0;
  for (; str[i]; i++)
    continue;
  return i;
}

int size_strr(char** str)
{
  int i = 0;
  for (; str + i && str[i]; ++i)
    continue;
  return i;
}

struct options* add_argument(struct options* opt, char *name, char *args)
{
  struct options* save  = opt;
  while (str_compare(opt->name, name) == 0)
  {
    opt = opt->next;
  }
  if (opt->nb_opt != -1 && size_strr(opt->arguments) == opt->nb_opt)
  {
    printf("FIXME too much arg for an option");
    exit(1);
  }
  if (!opt->arguments)
  {
    opt->arguments = malloc(sizeof (char*) * 2);
    opt->arguments[0] = args;
    opt->arguments[1] = NULL;
    return save;
  }
  int size = size_strr(opt->arguments);
  opt->arguments = realloc(opt->arguments, (size + 2) * sizeof (char*));
  opt->arguments[size] = args;
  opt->arguments[size + 1] = NULL;
  return save;
}

char** find_arguments(struct options* opt, char* name)
{
  for (struct options* tmp = opt; tmp; tmp = tmp->next)
  {
    if (str_compare(name, tmp->name))
      return tmp->arguments;
  }
  return NULL;
}

int is_option(struct options* opt, char* name)
{
  for (struct options* tmp = opt; tmp; tmp = tmp->next)
  {
    if (str_compare(name, tmp->name))
      return 1;
  }
  return 0;
}

struct option_manager* add_options(struct option_manager* opt, int count, ...)
{
  va_list ap;
  va_start(ap, count);
  opt->options = malloc(sizeof (char*) * (count / 2));
  opt->nb_param = malloc(sizeof (int) * (count / 2));
  int pair = 0;
  int j = 0;
  int i = 0;
  for (; i  + j < count;) 
  {
    if (pair == 0)
    {
      opt->options[i] = va_arg(ap, char*);
      i++;
    }
    else
    {
      opt->nb_param[j] = va_arg(ap, int);
      j++;
    }
    pair = !pair;
  }
  opt->options[i] = NULL;
  return opt;
}

static int inside(char** vec, char* str)
{
  for (int i = 0; vec[i]; ++i)
    if (str_compare(vec[i], str) == 1)
      return i;
  return -1;
}


void print(struct option_manager* opt)
{
  for (struct options* tmp = opt->opts; tmp; tmp = tmp->next) 
  {
    printf(tmp->name);
    printf(" ");
    for (int i = 0; tmp->arguments[i]; ++i)
    {
      printf(tmp->arguments[i]);
      printf(" ");
    }
    printf("\n");
  }
}

int main(int argc, char** argv)
{
  print(parse(argc, argv));
}

struct option_manager* parse(int argc, char** argv)
{
  char* last_opt = NULL;
  struct option_manager* manager = malloc(sizeof (struct option_manager));
  manager = add_options(manager, 6, "-a\0", 1, "-T\0", -1,"-p\0", -1);
  manager->opts = NULL;
  for (int i = 1; i < argc; ++i)
  {
    if (argv[i][0] == '-')
    {
      int index = inside(manager->options, argv[i]);
      if (index  == -1)
      {
        printf("FIXME\n");
         exit(1);
      }
      last_opt = argv[i];
      manager->opts = push_back(manager->opts, last_opt, manager->nb_param[index]);
    }
    else
    {
      if (!last_opt)
      {
        printf("FIXME");
        exit(1);
      }
      manager->opts = add_argument(manager->opts, last_opt, argv[i]);
    }
  }
  return manager;
}
