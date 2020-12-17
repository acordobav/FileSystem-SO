BIND_DIR = ./bin

INCLUDE_DIR = ./include

ALLEGRO_FLAGS_V = -lallegro -lallegro_dialog -lallegro_font -lallegro_image -lallegro_ttf -lallegro_primitives
ALLEGRO_FLAGS := $(shell pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_dialog-5 --libs --cflags)
DISK_JSON = disks/disk0.json
VISUALIZER_NAME = visualizer
FILESYSTEM = bin/FileSystem
VISUALIZER = bin/visualizer
JSON_FLAGS =  -lm -ljson-c
APP_NAME = FileSystem
P_THREAD = -pthread
DISK = disks/disk1
REMOVE_F = rm -f
CC = gcc

all: buildApp visualizer

cc: clean buildApp visualizer

buildApp: $(APP_NAME).c
		$(CC) -I $(INCLUDE_DIR) $(APP_NAME).c $(JSON_FLAGS) -o $(BIND_DIR)/$(APP_NAME) $(P_THREAD) $(ALLEGRO_FLAGS)

visualizer: $(VISUALIZER_NAME).c
		$(CC) $(VISUALIZER_NAME).c -o $(VISUALIZER) $(ALLEGRO_FLAGS_V) $(JSON_FLAGS)

clean:
	$(REMOVE_F) $(BIND_DIR)/$(APP_NAME) && $(REMOVE_F) $(DISK_JSON) && $(REMOVE_F) $(DISK) $(VISUALIZER)