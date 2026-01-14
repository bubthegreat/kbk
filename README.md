Current status: __ALIVE__ but buggy

# Installing your own instance

To Get things running:

Install Docker from [here](https://docs.docker.com/install/linux/docker-ce/ubuntu/)

Install tilt from **[here](https://docs.tilt.dev/install.html)**

Running:

```
git clone https://github.com/bubthegreat/kbk.git
cd kbk
tilt up
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

# NOTE

This is not a production configuration - there are default passwords and things that should *absolutely* be modified for production use.  Do - Not - Use - The - Defaults - In - Production

# Area Editing

Hacky, but works for getting areas off the PVC and onto local - if you're editing an area and want to pull it off the box, you can do this:

```
$POD=$(kubectl get pod -l app=kbk -o jsonpath="{.items[0].metadata.name}")
kubectl exec -it $POD -- bash -c 'cp /kbk/area/air.are /kbk/localxfer/air.are'
```

The problem with a local volume mount is that you have to have the source files in the same directory as your area file *CURRENTLY.  Once we can change that (Will take some work fixing the pathing) then we can just do a standard volume mount for the area files.

There's a LOT of relative pathing built in instead of absolute pathing since this is before the days of docker and consistent builds.
