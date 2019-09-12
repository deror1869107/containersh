package main

import (
	"context"
	"fmt"
	"os"
	"os/user"

	"github.com/docker/docker/api/types"
	"github.com/docker/docker/api/types/container"
	"github.com/docker/docker/api/types/filters"
	"github.com/docker/docker/api/types/mount"
	"github.com/docker/docker/client"
)

func main() {
	ctx := context.Background()
	cli, err := client.NewClientWithOpts(client.FromEnv)
	if err != nil {
		panic(err)
	}

	u, err := user.Current()
	if err != nil {
		panic(err)
	}

	var filter = filters.NewArgs(filters.Arg("name", fmt.Sprintf("%s$", u.Username)))

	var options = types.ContainerListOptions{
		Quiet:   true,
		All:     true,
		Filters: filter,
	}

	containers, err := cli.ContainerList(ctx, options)
	if err != nil {
		panic(err)
	}

	if len(containers) == 0 {
		source := fmt.Sprintf("/tmp/%s", u.Username)
		os.RemoveAll(source)
		os.Mkdir(source, os.ModePerm)

		resq, err := cli.ContainerCreate(ctx, &container.Config{
			Image: "ubuntu_socat",
			Cmd:   []string{u.Uid},
			Tty:   true,
		}, &container.HostConfig{
			RestartPolicy: container.RestartPolicy{
				Name: "always",
			},
			Mounts: []mount.Mount{mount.Mount{
				Type:     "bind",
				Source:   source,
				Target:   "/tmp/containersh",
				ReadOnly: false,
			}},
		}, nil, u.Username)
		if err != nil {
			panic(err)
		}

		cli.ContainerStart(ctx, resq.ID, types.ContainerStartOptions{})
	}
}
