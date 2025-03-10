DOCKER ?= docker
IMAGE_NAME ?= slate/os
DIR := $(CURDIR)

build:
	$(DOCKER) buildx create --use
	$(DOCKER) buildx build --platform linux/amd64 \
		-t $(IMAGE_NAME) \
		--cache-from=type=registry,ref=$(IMAGE_NAME):latest \
		--build-arg BUILDKIT_INLINE_CACHE=1 \
		--target final-image \
		--load .

run:
	MSYS_NO_PATHCONV=1 $(DOCKER) run --rm -v $(DIR):/build -w /build --platform linux/amd64 $(IMAGE_NAME) /bin/bash -c "dos2unix *.sh && ./clean.sh && ./headers.sh && ./iso.sh"
