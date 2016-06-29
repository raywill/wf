# Intro

Filter frequently used words using a user defined dictionary

# Manual Compile & Install
```
git clone https://github.com/raywill/wf
cd wf/
gcc -o wf wf.c
sudo mv wf /usr/local/bin/
cp -r .wf ~/
```

or, you can use 

# Auto Compile & Install
```
git clone https://github.com/raywill/wf
cd wf/
make
make test
```

# Basic Usage

```
echo "the" | ./wf
```

# Advanced Usage

```
# find all rare words in story.txt 
cat story.txt |tr -cs "[a-z][A-Z][0-9]" "[\012*]" | tr A-Z a-z |sort | ./wf -v -l 7 | uniq -c | sort
```


# Options

 -l N
 
 > N indecates word dict level, from 1 to 7, default is 1. see more in .wf directory
 

 -v
 
 > if -v specified, will output all un-matched words, otherwise output matched words


# Links

  http://blog.csdn.net/maray/article/details/51736233

