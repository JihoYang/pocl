/* OpenCL built-in library: printf() for CUDA

   Copyright (c) 2016 James Price

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

#include <stdarg.h>

void _cl_va_arg(va_list ap, long data[], int num_words);

int
_cl_printf(__attribute__((address_space(3))) char* restrict format, ...)
{
  // TODO: Might need more than 2 words for (e.g.) vectors
  long arg_data[1];

  va_list ap;
  va_start(ap, format);
  char ch = *format;
  while (ch) {
    if (ch == '%') {
      ch = *++format;

      if (ch == '%') {
        vprintf("%%", arg_data); // literal %
        ch = *++format;
      } else {
        // TODO: other format specifiers
        switch (ch) {
          case 'd':
          {
            _cl_va_arg(ap, arg_data, 1);
            vprintf("%d", arg_data);
            break;
          }
          case 'f':
          {
            _cl_va_arg(ap, arg_data, 2);
            vprintf("%lf", arg_data);
            break;
          }
          default: goto error;
        }
        ch = *++format;
      }
    }
    else {
      vprintf("%c", &ch);
      ch = *++format;
    }
  }

  va_end(ap);
  return 0;

  error:
  va_end(ap);
  vprintf("(printf format string error)", &ch);
  return -1;
}
