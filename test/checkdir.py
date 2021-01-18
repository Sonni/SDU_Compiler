#
# usage:  python checkdir.py <compiler> <path>
#         where
#           o you have first moved to the directory with the check program
#           o <compiler> is the full path name for the executable file
#           o <path> is the path to the programs directory
#
# example:
#
# >>> cd /home/IMADA/courses/cc/
# >>> python checkdir.py SomeCompiler /home/IMADA/cources/cc
#
# Checking: IfThen
# Compiling....Done
# Assembling...Done
# Executing....Done
# Correct output
# >>>

from sys import argv, stdout
import os
import time
import getopt
import sys

ProgAndSolDir = ""

def fileempty(filename):
   file = open(filename, "r")
   text = file.read()
   file.close()
   return len(text) == 0
   
#
# get maximum number of open files
#
MAXFD = os.sysconf('SC_OPEN_MAX')

#
# run a program until completion or timeout.
#
def execvp_wait(path, args, timeout):
    global TimedOut
    # fork child, closing all file descriptors
    pid = os.fork()
    if pid==0: # child
        # close all open file descriptors except
        # stdin,stdout & stderr
        for fd in xrange(3,MAXFD):
            try: os.close(fd)
            except os.error: pass

        # TODO: connect stdin,stdout & stderr to
        # something reasonable
        # exec the child
        try:
            os.execvp("./" + path,args)
        except Exception,e:
            # print traceback if exception occurs
            import traceback
            traceback.print_exc(file=os.sys.stderr)
        # always exit
        os._exit(1)
    else: # parent

        t = timeout
        while t>=0:
            # wait for completion
            child_pid,status = os.waitpid(pid, os.WNOHANG)
            if child_pid==pid:
                if os.WIFSTOPPED(status) or \
                   os.WIFSIGNALED(status):
                    return None
                elif os.WIFEXITED(status):
                    return os.WEXITSTATUS(status)
            # wait for a second
            time.sleep(0.1)
            t -= 1

        # not completed within timeout seconds
        TimedOut = 1
        os.kill(pid, 9)
        os.kill(pid+1, 9)

from os import walk
#
# Test
#
if __name__=="__main__":
    TimedOut = 0
    compiler = argv[1]
    path = argv[2]
    user = str(os.getuid())
    numIncorrect = 0
    numCompilerErr = 0
    numTotal = 0
    numRuntimeTimeOut = 0
    numRuntimeError = 0
    numCorrect = 0



    stdout.write("\\begin{table}[H]\n")
    stdout.write("\\begin{tabular}[t]{l>{\\raggedright}p{0.2\linewidth}>{\\raggedright\\arraybackslash}p{0.5\linewidth}}\n")
    stdout.write("\\toprule\n")
    stdout.write("&	Output	& Showcases	\\\\\n")
    stdout.write("\\midrule\n")



    f = []
    for (dirpath, dirnames, filenames) in walk(path):
       f.extend(filenames)

    for index in range(len(filenames)):
       TimedOut = 0
       extension = os.path.splitext(filenames[index])[1][1:]
       filename = os.path.splitext(filenames[index])[0];
       if extension != "sk":
         continue
       numTotal += 1

       stdout.write(filename + " & ")
       sys.stdout.flush()

       code = execvp_wait("checkwrap",
                       ["checkwrap", compiler,
                        ProgAndSolDir + filename + ".sk", user], 60)
       if TimedOut:
           stdout.write("Compiler timeout & ")
       else:
           if code != 0:
               stdout.write("Compiler err " + str(code) + " & ")
               numCompilerErr += 1

       if not TimedOut and code == 0:
           code = execvp_wait("gccwrap", ["gccwrap", user], 60)
           if code != 0:
               stdout.write("Asm err " + str(code) + " & ")

           TimedOut = 0
           if filename != "Knapsack":
               code = execvp_wait("execwrap", ["execwrap", user], 60)
           else:
               code = execvp_wait("execwrap", ["execwrap", user], 10*60)
           if TimedOut:
               stdout.write("Runtime timeout & ")
               numRuntimeTimeOut += 1
           else:
               if code == 0:
                   os.system("diff /tmp/" + user + "-output " + \
                             ProgAndSolDir + filename + ".out > /tmp/" + \
                             user + "-difference")
                   if not fileempty("/tmp/" + user + "-difference"):
                      stdout.write("Incorrect & ")
                      numIncorrect += 1
                   else:
                      stdout.write("Correct & ")
                      numCorrect += 1
               else:
                   numRuntimeError += 1
                   stdout.write("\n")

       stdout.write("\\\\\n")


    stdout.write("\\bottomrule\n")
    stdout.write("\\end{tabular}\n")
    stdout.write("\\caption{" + os.path.basename(os.getcwd()) + "}\n")
    stdout.write("\\end{table}\n")

    stdout.write("\n\n")
    stdout.write("Total: " + str(numTotal) + "\n")
    stdout.write("Correct: " + str(numCorrect) + "\n")
    stdout.write("Incorrect: " + str(numIncorrect) + "\n")
    stdout.write("Runtime timeout: " + str(numRuntimeTimeOut) + "\n")
    stdout.write("Runtime error: " + str(numRuntimeError) + "\n")
    stdout.write("Compiler error: " + str(numCompilerErr) + "\n")



"""
if __name__=="__main__":
    TimedOut = 0
    compiler = argv[1]
    path = argv[2]
    user = str(os.getuid())
    numIncorrect = 0
    numCompilerErr = 0
    numTotal = 0
    numRuntimeTimeOut = 0
    numRuntimeError = 0
    numCorrect = 0
    f = []
    for (dirpath, dirnames, filenames) in walk(path):
       f.extend(filenames)

    for index in range(len(filenames)):
       TimedOut = 0
       extension = os.path.splitext(filenames[index])[1][1:]
       filename = os.path.splitext(filenames[index])[0];
       if extension != "sk":
         continue
       numTotal += 1

       stdout.write("\n")
       stdout.write("Checking: " + filename + "\n")
       code = execvp_wait("checkwrap",
                       ["checkwrap", compiler,
                        ProgAndSolDir + filename + ".sk", user], 60)
       if TimedOut:
           stdout.write("compiler TIMED OUT")
       else:
           if code != 0:
               stdout.write("[compiler ERROR code " + str(code) + "]")
               numCompilerErr += 1

       if not TimedOut and code == 0:
           code = execvp_wait("gccwrap", ["gccwrap", user], 60)
           if code != 0:
               stdout.write(" [assembler ERROR code " + str(code) + "]")

           TimedOut = 0
           if filename != "Knapsack":
               code = execvp_wait("execwrap", ["execwrap", user], 60)
           else:
               code = execvp_wait("execwrap", ["execwrap", user], 10*60)
           if TimedOut:
               stdout.write("runtime TIMED OUT")
               numRuntimeTimeOut += 1
           else:
               if code == 0:
                   os.system("diff /tmp/" + user + "-output " + \
                             ProgAndSolDir + filename + ".out > /tmp/" + \
                             user + "-difference")
                   if not fileempty("/tmp/" + user + "-difference"):
                      stdout.write("INCORRECT output")
                      numIncorrect += 1
                   else:
                      stdout.write("Correct output")
                      numCorrect += 1
               else:
                   stdout.write("[runtime ERROR code " + str(code) + "]")
                   numRuntimeError += 1
                   stdout.write("\n")

       stdout.write("\n")
    stdout.write("\n\n")
    stdout.write("Total: " + str(numTotal) + "\n")
    stdout.write("Correct: " + str(numCorrect) + "\n")
    stdout.write("Incorrect: " + str(numIncorrect) + "\n")
    stdout.write("Runtime timeout: " + str(numRuntimeTimeOut) + "\n")
    stdout.write("Runtime error: " + str(numRuntimeError) + "\n")
    stdout.write("Compiler error: " + str(numCompilerErr) + "\n")


"""