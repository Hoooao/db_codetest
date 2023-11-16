CC = g++
CFLAGS = -Wall -Wextra -std=c++17
DB_MAIN = db.cpp
.PHONY: all clean

all: db

db: $(DB_MAIN)
	$(CC) $(CFLAGS) $(DB_MAIN) -o db

clean:
	rm -f db
