all: check build_image

GOBUILD=go build
GOGET=go get

build_image:
	docker build -t ubuntu_socat .

check: 
	$(GOBUILD) -o check

clean:
	rm -f check

deps:
	$(GOGET) github.com/docker/docker/api/types
	$(GOGET) github.com/docker/docker/client
