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