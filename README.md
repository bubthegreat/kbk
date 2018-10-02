Current status: __ALIVE__

To Get things running:

Download the repo

`git clone https://github.com/bubthegreat/kbk.git`

Build the docker image:

`docker build -t kbkdocker .`

Start the image:

`docker run -dt -p 8989:8989 -v ${PWD}:/opt/kbk --name kbk_container kbkdocker`

Get in that baby real deep:

`docker exec -it kbk_container bash`

Restart the mysql server because it hates being installed for some reason:

`chown -R mysql:mysql /var/lib/mysql /var/run/mysqld `

`service mysql restart`

Make the database and users:

`RUN mysql -u root -proot -e "CREATE DATABASE kbkdatabase;"`

`RUN mysql -u root -proot -e "CREATE USER kbkuser@localhost IDENTIFIED BY 'somepassword';"`

`RUN mysql -u root -proot -e "GRANT ALL PRIVILEGES ON kbkdatabase.* TO 'someuser'@'localhost';"`

`RUN mysql -u root -proot -e "FLUSH PRIVILEGES;"`

Enter into the mysql command line:

`mysql -u root -proot`

Select the database:

`use kbkdatabase`;

Copypasta the database.sql file to create the actual database tables

`kbk/area/startup`

