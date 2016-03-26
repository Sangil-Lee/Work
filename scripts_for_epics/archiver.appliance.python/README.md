* Compile js by using closure-compiler 

Download and compile compiler


```bash
jhlee@kaffee:~$ mkdir github
jhlee@kaffee:~$ cd github/
jhlee@kaffee:~/github$ git clone https://github.com/google/closure-compiler.git
Cloning into 'closure-compiler'...
WARNING: gnome-keyring:: couldn't connect to: /home/jhlee/.cache/keyring-IHUQJH/pkcs11: No such file or directory
remote: Counting objects: 63063, done.
remote: Compressing objects: 100% (201/201), done.
remote: Total 63063 (delta 68), reused 21 (delta 9)
Receiving objects: 100% (63063/63063), 89.15 MiB | 4.16 MiB/s, done.
Resolving deltas: 100% (32177/32177), done.
jhlee@kaffee:~/github$ ls
closure-compiler/
jhlee@kaffee:~/github$ cd closure-compiler/
jhlee@kaffee:~/github/closure-compiler$ ls
build.xml  CONTRIBUTORS  externs/  lib/          pom.xml    src/
contrib/   COPYING       gen/      pom-main.xml  README.md  test/


jhlee@kaffee:~/github/closure-compiler$ ant jar
Buildfile: /home/jhlee/github/closure-compiler/build.xml

relversion:

compile:
    [mkdir] Created dir: /home/jhlee/github/closure-compiler/build/classes
    [javac] Compiling 12 source files to /home/jhlee/github/closure-compiler/build/classes
    [javac] Compiling 553 source files to /home/jhlee/github/closure-compiler/build/classes
    [javac] /home/jhlee/github/closure-compiler/src/com/google/javascript/jscomp/DefaultPassConfig.java:279: warning: [deprecation] checkRequires in CompilerOptions has been deprecated
    [javac]     if (options.checkRequires.isOn()
    [javac]                ^
    [javac] /home/jhlee/github/closure-compiler/src/com/google/javascript/jscomp/WarningLevel.java:58: warning: [deprecation] checkRequires in CompilerOptions has been deprecated
    [javac]     options.checkRequires = CheckLevel.OFF;
    [javac]            ^
    [javac] /home/jhlee/github/closure-compiler/src/com/google/javascript/jscomp/ant/CompileTask.java:614: warning: [unchecked] unchecked conversion
    [javac]     Iterator<Resource> iter = rc.iterator();
    [javac]                                          ^
    [javac]   required: Iterator<Resource>
    [javac]   found:    Iterator
    [javac] 3 warnings
    [mkdir] Created dir: /home/jhlee/github/closure-compiler/build/classes/rhino_ast/java/com/google/javascript/rhino
     [copy] Copying 1 file to /home/jhlee/github/closure-compiler/build/classes/rhino_ast/java/com/google/javascript/rhino
     [copy] Copying 1 file to /home/jhlee/github/closure-compiler/build/classes/com/google/javascript/jscomp/parsing
[propertyfile] Updating property file: /home/jhlee/github/closure-compiler/build/classes/com/google/javascript/jscomp/parsing/ParserConfig.properties
    [mkdir] Created dir: /home/jhlee/github/closure-compiler/build/classes/com/google/javascript/jscomp/js
     [copy] Copying 3 files to /home/jhlee/github/closure-compiler/build/classes/com/google/javascript/jscomp/js

jar:
      [zip] Building zip: /home/jhlee/github/closure-compiler/build/externs.zip
      [jar] Building jar: /home/jhlee/github/closure-compiler/build/compiler.jar

BUILD SUCCESSFUL
Total time: 9 seconds
```

* Compile timeplot.js to create timeplot_c.js


```bash
jhlee@kaffee:~/scripts_for_epics/archiver.appliance.python$ java -jar ~/github/closure-compiler/build/compiler.jar --js_output_file=www/timeplot_c.js www/timeplot.js 
```
