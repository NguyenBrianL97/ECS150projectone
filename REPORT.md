# Phase 0 - system()and Makefile

After copying over the skeleton C file, compiling and running we see that the file will print out the current data i.e. time. However this uses the function **system()**. In the next phase we will move away from the system() call to try to replace it with the **fork+exec+wait** method.

> **Note:** In the skeleton C file, it will return the date and print to stdout
> 
```
fprintf(stdout, "Return status value for '%s': %d\n", cmd, retval);
```

We also wrote a Makefile, to compile and clean the code. 

# Phase 1 - fork+exec+wait

We based the program on the skeleton code that was provided in class. 

> **Note:** In this updated version of the **skeleton C file**, it will return the date, but print to stderr this time.
```
fprintf(stderr, "Return status value for '%s': %d\n", cmd, retval);
```
# Phase 2 - Read commands 

**DATA STRUCTURE** String management

- The workspace synchronization will sync all your files, folders and settings automatically. This will allow you to fetch your workspace on any other device.
	> To start syncing your workspace, just sign in with Google in the menu.

- The file synchronization will keep one file of the workspace synced with one or multiple files in **Google Drive**, **Dropbox** or **GitHub**.
	> Before starting to sync files, you must link an account in the **Synchronize** sub-menu.


# Phase 3 - Additional arguments

**Parse command line**

- Markdown: publish the Markdown text on a website that can interpret it (**GitHub** for instance),
- HTML: publish the file converted to HTML via a Handlebars template (on a blog for example).

# Phase 4 - Builtin command support

After publishing, StackEdit keeps your file linked to that publication which makes it easy for you to re-publish it. Once you have modified your file and you want to update your publication, click on the **Publish now** button in the navigation bar.

> **Note:** The **Publish now** button is disabled if your file has not been published yet.
> 

We have "cd", "pwd" and "exit" commands. When we call "exit" command, we exit with status 0 followed by "Bye...". For the "cd" command, we use "chdir()"" or rather just "cd" to switch between directories. Lastly, for "pwd" command we get the current directory. 

After these commands are ran, we have a child - parent forking mechanism, where the parent waits for the child before printing the entire output. 



# Phase 5 and 6 - Input and output redirection
In order to handle the inputs and ouputs for redirection, it was necessary to get the "output" of the exec function. Whatever was printed in the terminal after a command in the shell was needed to be fed as an input into the next argument in the command. The way we were able to do this was by using popen(). popen essentially is a fork then an exec that writes to an I/O stream. We are then able to read this I/O stream which tracks STDOUT to record our "output". 



# Phase 7 - Piping commands
Once again using the popen() method for phase 5 and 6, we are able to save the "output" from each piping so that it can be used as input for the next pipe. 

Similar to how input redirection we would treat the output of the initial command as the **phase 5 file** that would be fed as input to the next command in the pipes. 



# Phase 8 - Background commands
Our code is able to recognize single unix commands including sleep. The program fails to have background processes because we have not implemented correctly a proper wait hirerachy where the child would finish the task but have the parent who has theoretically been waiting to print the completed background process completion message. 

In addition, we have not implemented a way to check the status of any background processes (children processes) so preventing exit while background processes are running is impossible.

# How did we test the program.  

We ran the script provided by the professor to check our program as a first defense. In addition to the script, we would use the sshell_ref executable to test any outputs we were unsure of that were not in the script as well as the project html file.  

# Sources. 

We got a majority of the understanding from the skeleton code provided in class during lecture. Everytime we hit a road block, we refered to stackover flow, piazza, various linux command websites (to understand the difference bewteen certain commands and what we could use effectively for desiered output).   