Current status: __BROKEN__

To get to where I'm stuck, here's where I'm at:

Download the repo

`git clone https://github.com/bubthegreat/kbk.git`

Build the docker image:

`docker build -t kbkdocker .`

Start the image:

`docker run -dt -p 8989:8989 --name kbk_container kbkdocker`

Get in that baby real deep:

`docker exec -it kbk_container bash`

Restart the mysql server because it hates being installed for some reason:

`service mysql restart`

Make the database and users:

`RUN mysql -u root -proot -e "CREATE DATABASE kbkdatabase;"`

`RUN mysql -u root -proot -e "CREATE USER kbkuser@localhost IDENTIFIED BY 'kbkpassword';"`

`RUN mysql -u root -proot -e "GRANT ALL PRIVILEGES ON kbkdatabase.* TO 'kbkuser'@'localhost';"`

`RUN mysql -u root -proot -e "FLUSH PRIVILEGES;"`

Enter into the mysql command line:

`mysql -u root -proot`

Select the database:

`use kbkdatabase`;

Copypasta the database.sql file to create the actual database tables

run the service and see the segmentation faults:

`kbk/area/startup`

root@0d2c8bdd4992:/kbk/log# cat 1007.log
Thu Sep 20 10:22:22 2018 :: Mysql_init: Established connection to MySQL database.
Thu Sep 20 10:22:22 2018 :: 34 materials loaded.
Thu Sep 20 10:22:22 2018 :: Loading object counts off players now...
Thu Sep 20 10:22:22 2018 :: ../player/Zzz.plr
Thu Sep 20 10:22:22 2018 :: Exits Fixed.
Thu Sep 20 10:22:22 2018 :: Area Update.
Thu Sep 20 10:22:22 2018 :: Cabal Items: Loaded.
Thu Sep 20 10:22:22 2018 :: Created item fix log.
Thu Sep 20 10:22:22 2018 :: Created race info file.
Thu Sep 20 10:22:22 2018 :: [*****] BUG: Fread_string: EOF
root@0d2c8bdd4992:/kbk/log#


Right now, I suspect it's because of the changes that I made to make it compile successfully - it was failing to compile due to conflicts in definitions for the linux calloc and another.  I'll see if I can get it to compile properly in another environment. 
