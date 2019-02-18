Current status: __ALIVE__ but buggy

# Installing your own instance

To Get things running:

Install Docker from [here](https://docs.docker.com/install/linux/docker-ce/ubuntu/)

Installation and running:

```
git clone https://gitlab.com/bubthegreat/kbk.git
cd kbk
docker build -t 'kbk' .
docker run -d -p 8989:8989 kbk
```

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

I've also included the service and deployment descriptions for a kubernetes config, or alternatively the docker-compose yaml for a docker swarm deployment.

# NOTE

This is not a production configuration - there are default passwords and things that should *absolutely* be modified for production use.  Do - Not - Use - The - Defaults - In - Production
