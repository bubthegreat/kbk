Current status: __ALIVE__ but buggy

# Installing your own instance

To Get things running:

Install Docker from [here](https://docs.docker.com/install/linux/docker-ce/ubuntu/)

Download the repo:

`git clone https://gitlab.com/bubthegreat/kbk.git`

Run the docker compose command:
`docker-compose up`

Now you should be able to connect to port 8989 of your host it's running on. (It's running on 0.0.0.0:8989)

```
bub-MacBookPro11:kbk bub$ telnet localhost 8989
Trying ::1...
Connected to localhost.
Escape character is '^]'.

             Original DikuMUD by Hans Staerfeldt, Katja Nyboe,
             Tom Madsen, Michael Seifert, and Sebastian Hammer
             Based on MERC 2.1 code by Hatchet, Furey, and Kahn.
             ROM 2.4 copyright (c) 1993-1998 Russ Taylor.
             Tartarus 1.0 code by Daniel Graham.

By what name do you wish to be known?    

```

# How it works right now

Two docker containers - one for the SQL, and one for the POS code to run.  This'll let me scale it out for rolling updates with no downtime to players when we update code.

There's more for me to edit here, but It's early as fuck, and I just got it working, so I'mma wait.

# Redeploying after changes

So you've made some changes on your dev server - that's cool bro - but how do you get them to show up in production?  Right now, it means a brief moment of downtime, because I haven't automated the HA cluster containers yet.  There are two commands you'll need to know:

`docker-compose down --rmi all`

This one gracefully shuts down the containers and removes the old images.

`docker compose up --force-recreate --build`

This command will bring back up the containers, but will force them to be recreated (So you don't have the stale pos2 binary without your updates) with the `--recreate` flag, and the `--build` flag will make sure that any changes on the docker container layer take effect. 
