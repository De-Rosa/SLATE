DOCKER ?= docker
IMAGE_NAME ?= slate/os
DIR := $(CURDIR)

build:
	$(DOCKER) --debug build --platform linux/amd64 -t $(IMAGE_NAME) .

run:
	$(DOCKER) run -it --rm -v $(DIR):/build -w /build $(IMAGE_NAME)
