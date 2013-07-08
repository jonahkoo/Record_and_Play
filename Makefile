all: record play

record: record.c
	gcc record.c -o record -lasound

play: play.c
	gcc play.c -o play -lasound

clean:
	rm record play

clean-data:
	rm sound.raw

