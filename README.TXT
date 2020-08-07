Jytter
Version 5
December 15, 2019
http://jytter.blogspot.com

Jytter, a Userspace True Random Number Generator Without IO

For theory, performance whitepapers, and contacting me please visit:

http://jytter.blogspot.com

This document is only concerned with installing and running Jytter.

For security reasons, prebuilt binaries are no longer provided.

Here is how to build the demo:

The presumed Linux compiler is GCC (http://gcc.gnu.org). On Windows,
it's MinGW (http://www.mingw.org). On Mac, it's clang.

You will also need NASM, the Netwide Assembler (http://www.nasm.us).
Due to its nature, Jytter can only be written in assembly language.

Under Windows, only 32-bit (IA32) builds are supported. No 64-bit release is
planned. However, it should be a straightforward adaptation of the 64-bit Linux
code for experienced assembly language programmers, given the Wikipedia link
below.

Under Linux, both 32-bit (IA32 and X32) and 64-bit (AMD64) work fine. Note that
Jytter needs no memory apart from a few bytes of stack, so it will not benefit
from 64-bit addressing at all. The 64-bit support is simply provided for build
convenience.

On Mac, only 64-bit compilation is supported.

Want to link Jytter into a different compiler? Then see
http://en.wikipedia.org/wiki/X86_calling_conventions for more information,
which explains the stack and register protocol for invoking assembly language
code from C. (You may need to modify jytter.asm.) Alternatively, use your
compiler's internal inline assembly language facility (sometimes via "_asm")
to implement Jytter amidst other code.

RELEASE NOTES

Added RDSEED instruction, when available, for the integration of hardware
entropy.

INSTALLATION NOTES

WARNING:

Jytter must not be run on more than one core of the same physical system in
such a way that it overlaps with other Jytter instances. Otherwise, the quality
of randomness could be severely reduced. However, it may be run on different
cores at completely orthogonal times. It may, however, be run on identical
computers which do not share the same memory bus, for example, neighboring
blade servers attached to the same PCI Express bus.

Atomic operations introduced in C11/C++11 could help to guarantee compliance
with the above requirement.

You might think that you would get better randomness by running multithreaded
Jytter, then adding all the results together. Probably not, as doing so would
only increase the pseudorandom complexity of Jytter's behavior, which could be
mistaken for true randomness.

If you're really concerned about randomness quality, then you can increase
UNIQUE_EVENT_DURATION_COUNT in jytter.asm, at the cost of higher execution
time. Based on my own analysis, both theoretical and empirical, I see no
reason to do that, mainly because Jytter is hypersensitive to the order in
which various events occur.

CONTENTS

  jytter.asm -- The smallest, coolest TRNG you've ever seen!

  timestamp.asm -- Bonus code that gives you easy C-level access to the CPU
   timestamp counter (TSC).

  linux_ia32_build.sh -- Linux IA32 (X86) build script for GCC.

  linux_x32_build.sh -- Linux X32 build script for GCC.

  linux_amd64_build.sh -- Linux AMD64 build script for GCC.

  mac64_build.sh -- Mac AMD64 build script for clang.

  win32_build.sh -- Windows build batch file for MinGW.

LINUX INSTALLATION

Try typing "gcc" and "nasm" in a terminal window. If you don't have gcc, do:

  sudo apt-get install gcc

or better yet get another Linux distro that has it preinstalled.

If you don't have nasm, go to their webiste at http://www.nasm.us and get the
latest build under the "DOWNLOAD" link, in the "linux" folder. You want the
"i386" RPM file. Uncompress it. If you don't want a full installation, you just
need to copy the executable, which is /usr/bin/nasm to the "real" /usr/bin"
folder on your harddrive:

  sudo cp your_nasm_folder/usr/bin/nasm /usr/bin

Or sometimes the following "just works" like it's supposed to:

  sudo apt-get install nasm

In the jytter folder, run the following script, which tries to delete a few
preexisting build files, so read it first if you're paranoid:

  /bin/sh ./linux_ia32_build.sh
  
  or
  
  /bin/sh ./linux_x32_build.sh
  
  or
  
  /bin/sh ./linux_amd64_build.sh

That's all! You can now run the demo:

  ./demo

MAC INSTALLATION

  You're going to need nasm (http://nasm.us) and some familiarity with using
  the command line. After nasm has been installed, do:

  /bin/sh ./mac64_build.sh

That's all! You can now run the demo:

  ./demo

WINDOWS INSTALLATION

Make sure you have NASM and MinGW installed. You need to make sure that gcc and
nasm are in your system path. Close all command (CMD) windows and go to:

  Windows XP:

    Start -> My Computer -> Right click -> Properties -> Advanced ->
     Environment Variables -> System variables -> Path

  Windows 7:

    Start -> Computer -> Right click -> Advanced system settings ->
     Advanced -> Environment Variables -> System variables -> Path

Add ";c:\program files\mingw\bin" to the Path variable. You might need double
quotes around the addendum and after the semicolon. Or just use "progra~1"
instead of "program files" so you don't need the quotes, which should work
unless you have another folder in the root directory starting with "progra".

You may also need to add the NASM path in a similar manner, unless you want to
copy nasm.exe into the "mingw\bin" folder.

Now reopen a new CMD window:

  Start -> Run (or just type in the dialog) -> cmd

Go to the Jytter folder:

  c: (or d: or whatever drive letter)
  cd \your_jytter_path

Build it:

Run the following batch file, which tries to delete a few preexisting build
files, so read it first if you're paranoid:

  win32_build

Run it:

  demo

Enjoy!
