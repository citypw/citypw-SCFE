/* big: Bash Inner Guile */

/* See Makefile for compilation details. */

#include <config.h>

#if defined (HAVE_UNISTD_H)
#  include <unistd.h>
#endif
#include "bashansi.h"
#include <stdio.h>
#include <errno.h>
#include <libguile.h>

#include "builtins.h"
#include "shell.h"
#include "bashgetopt.h"

#if !defined (errno)
extern int errno;
#endif

extern char *strerror ();

static char *get_sexp(char *buf)
{
  char ch;
  int end = 0;
  int pair = 0;
  char *ptr = buf;
  
  while(!end)
    {
      switch(ch = getchar())
	{
	case '(':
	  pair++;
	  break;
	case ')':
	  pair--;
	  break;
	case '\n':
	  if(!pair)
	    {
	      end = 1;
	      *ptr = '\0';
	    }
	  else
	    {
	      *ptr = ' ';
	    }
	  goto next_char;
	case '\0':
	case EOF:
	  return NULL;
	}

      *ptr = ch;
    next_char:
      ptr++;
    }

  return buf;
}

static void repl()
{
  char *buf = (char*)scm_gc_malloc(1024 ,"input-sexp");
  SCM ret;

  printf("Bash inner Guile interpreter!\n");
  
  while((buf = get_sexp(buf)) != NULL)
    {
      ret = scm_eval_string(scm_from_locale_string(buf));
      ret = scm_object_to_string(ret ,SCM_UNDEFINED);
      printf("==> %s\n" ,scm_to_locale_string(ret));
    }
}

static void eval_once(char *buf)
{
  SCM ret;

  ret = scm_eval_string(scm_from_locale_string(buf));
  ret = scm_object_to_string(ret ,SCM_UNDEFINED);
  
  printf("%s" ,scm_to_locale_string(ret));
}

static void eval_exp(int argc ,char **argv)
{
  if(argc > 1)
    {
      return eval_once(argv[1]);
    }
  else
    {
     return repl();
    }
}

big_builtin (list)
     WORD_LIST *list;
{
  int argc;
  char **argv;
    
  argv = make_builtin_argv(list ,&argc);

  scm_init_guile();
  eval_exp(argc ,argv);
  free(argv);

  return EXECUTION_SUCCESS;
}

char *big_doc[] = {
	"Execute arguments as a Guile exp.",
	"",
	"Combine ARGs into a single string,"
	"use the result as input to the shell,\n"
	"and execute the resulting commands.\n\n"

	"Exit Status:\n"
	"Returns exit status of command or success if command is null.",

	(char *)NULL
};

struct builtin big_struct = {
	"big",			/* builtin name */
	big_builtin,		/* function implementing the builtin */
	BUILTIN_ENABLED,		/* initial flags for builtin */
	big_doc,			/* array of long documentation strings. */
	"big [exp]",			/* usage synopsis; becomes short_doc */
	0				/* reserved for internal use */
};
