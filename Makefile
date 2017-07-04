all:
	g++ -o wf wf.cpp
	cp -f wf /usr/local/bin/
	cp -r .wf ~/


test:
	echo "the blue green" | tr -cs "[a-z][A-Z][0-9]" "[\012*]" | wf -l 2
	echo "the blue green" | tr -cs "[a-z][A-Z][0-9]" "[\012*]" | wf -v -l 2

