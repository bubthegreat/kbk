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

crypt is currently segfaulting things, so I have to figure out how to get crypt unfucked.
