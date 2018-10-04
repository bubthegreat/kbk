Current status: __ALIVE__ but buggy

# Installing your own instance

To Get things running:

Install Docker from [here](https://docs.docker.com/install/linux/docker-ce/ubuntu/)

Download the repo:

`git clone https://gitlab.com/bubthegreat/kbk.git`

Run the docker compose command:
`docker-compose up`

Now you should be able to connect to port 8989 of your host it's running on. (It's running on 0.0.0.0:8989)

# How it works right now

Two docker containers - one for the SQL, and one for the POS code to run.  This'll let me scale it out for rolling updates with no downtime to players when we update code.

There's more for me to edit here, but It's early as fuck, and I just got it working, so I'mma wait.