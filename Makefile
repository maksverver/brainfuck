CFLAGS=-O2
FILES=interpreter interpreter2 bf2c woorden

all: $(FILES)

%: %.bf
	./bf2c <"$<" | cc -Os -xc -o"$@" -

woorden-kort.bf: woorden.bf
	echo `sed 's/[^][,.<>+-]//g' < woorden.bf` | sed 's/ \+//g' | sed 's/<>\|><//g' | fold >woorden-kort.bf

clean:
	rm -f $(FILES)

.PHONY: woorden-kort.bf
