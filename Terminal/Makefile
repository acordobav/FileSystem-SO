BIND_DIR = ./bin

INCLUDE_DIR = ./include

ALLEGRO_FLAGS := $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_dialog-5 --libs --cflags)
DISK_JSON = json/disk0.json
JSON_FLAGS =  -lm -ljson-c
APP_NAME = FileSystem
P_THREAD = -pthread
REMOVE_F = rm -f
DISK = bin/disk1
CC = gcc

all: buildApp

buildApp: $(APP_NAME).c
		gcc -I $(INCLUDE_DIR) $(APP_NAME).c $(JSON_FLAGS) -o $(BIND_DIR)/$(APP_NAME) $(P_THREAD) $(ALLEGRO_FLAGS)

visualizer: visualizer.c
		gcc visualizer.c -o $(BIND_DIR)/visualizer -lallegro -lallegro_dialog -lallegro_font -lallegro_image -lallegro_ttf -lallegro_primitives -lm -ljson-c

clean:
	rm $(BIND_DIR)/$(APP_NAME) && $(REMOVE_F) $(DISK_JSON) && $(REMOVE_F) $(DISK)