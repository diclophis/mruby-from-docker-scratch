#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/errno.h>


#include <mruby.h>
#include <mruby/array.h>
#include <mruby/irep.h>
#include <mruby/compile.h>
#include <mruby/string.h>
#include <mruby/error.h>


static void if_exception_error_and_exit(mrb_state* mrb, char *msg) {
  // check for exception, only one can exist at any point in time
  if (mrb->exc) {
    fprintf(stderr, "%s", msg);
    mrb_print_error(mrb);
    exit(2);
  }
}


int main(int argc, char** argv) {
  FILE *f = 0;
  f = fopen(".mirbrc", "r");
  if (0 == f) {
    return 1;
  }

  mrb_state *mrb;
  mrb_value ret;

  // initialize mruby
  if (!(mrb = mrb_open())) {
    fprintf(stderr,"%s: could not initialize mruby\n",argv[0]);
    return -1;
  }

  mrb_value args = mrb_ary_new(mrb);
  int i;

  // convert argv into mruby strings
  for (i=1; i<argc; i++) {
     mrb_ary_push(mrb, args, mrb_str_new_cstr(mrb,argv[i]));
  }

  mrb_define_global_const(mrb, "ARGV", args);

  //mrb_define_method(mrb, mrb->object_class, "business", business, MRB_ARGS_BLOCK());

  //eval_static_libs(mrb, detective, investigation, hterm, NULL);

  mrbc_context *detective_file = mrbc_context_new(mrb);
  mrbc_filename(mrb, detective_file, ".mirbrc");
  ret = mrb_load_file_cxt(mrb, f, detective_file);
  mrbc_context_free(mrb, detective_file);
  fclose(f);
  if_exception_error_and_exit(mrb, "Exception in .mirbrc\n");

  // cleanup mruby
  mrb_close(mrb);

  return 0;
}
