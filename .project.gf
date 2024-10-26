[executable]
path=build/Snekken
arguments=-o Snekken src/main.c src/snake.c -I src -L /usr/local/lib -lraylib -lm -ldl -lpthread -lrt 
ask_directory=1
